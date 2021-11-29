

# hacker 报告

## 解题思路

总体思路是，对crc值进行hash储存，然后进行相应的插入和查询操作。

初始化：将所有长度小于等于5的串放入hash表，对于每个字符串，可以将其表示为一个n位19进制数，随后可以映射到一个数字，每个串的数字表示唯一。

根据框架, 按照如下方式加盐后, 随后进行取模的hash, 随后放入表中:

```c++
for (l i = 0; i < 19 * 19 * 19 * 19 * 19; ++i) {
        if (is_enmpty(i))continue;
        ltos(i);
        unsigned pos = crc32(0,  gtmp, strlen((char *) gtmp));
        pos = crc32(pos, salt, strlen((char*)salt));
        put(pos, i);
    }
```

其中put函数是取模hash, 冲突策略为向右线性探测, 并且只有当crc值一致, 原始串特征数字不一致时, 才会mark为重复插入, 在相应的数组中标记:

```c++
ltos(i); 
    unsigned posi = pos % Mod;
    bool flag = true;
    while (table[posi]) {
        if (positions[posi] == pos and i != table[posi]) { //位置相同，但代表但字符串不同，则为重复
            Di[posi] = true;
            return;
        } else if (positions[posi] == pos and i == table[posi]) return; //重复插入，置之不理
        posi++;
        if (posi >= Mod) posi = 2;
    }
    table[posi] = i;
    positions[posi] = pos;
```

对于查询, 照常循环, 判断是否是重复标记以及crc值是否一致。

```c++
inline int get(unsigned pos) {
    unsigned posi = pos % Mod;
    while (positions[posi] and posi <= Mod) {
        if (Di[posi] and pos == positions[posi]) return -1; // 已经重复的单词位置，并且对应的单词hash一致
        else if (positions[posi] == pos) return (int) posi; //找到了
        posi++;
        if (posi >= Mod) posi = 2; //越界循环
    }
    return 0;
}
```

