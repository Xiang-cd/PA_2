



# hacker 报告

项晨东 2019011831

## 解题思路和代码简析

总体思路是，对crc值进行hash储存，然后进行相应的插入和查询操作。

初始化：将所有长度小于等于5的串放入hash表，对于每个字符串，可以将其表示为一个n位19进制数，随后可以映射到一个数字，每个串的数字表示唯一。根据框架, 按照如下方式加盐后, 随后进行取模的hash, 随后放入表中:

```c++
void init() {
    for (l i = 0; i < 19 * 19 * 19 * 19 * 19; ++i) {
        if (empty(i))continue; // 如果包含空字符, 则跳过, 因为这个字符已经被更短的字符串表示
        ltos(i);  // 从数字转化为字符串, 写入全局变量gtmp
        unsigned pos = crc32(0, gtmp, strlen((char *) gtmp));
        pos = crc32(pos, salt, strlen((char *) salt)); //获取crc值
        put(pos, i); // 加入字典
    }
}
```

其中put函数是取模hash, 冲突策略为向右线性探测(oj提交版本为链表存储), 并且只有当crc值一致, 原始串特征数字不一致时, 才会mark为重复插入, 在相应的数组中标记:

```c++
inline void put(unsigned pos, l i) {
    ltos(i);
    unsigned posi = pos % Mod;
    while (table[posi]) {
        if (positions[posi] == pos and i != table[posi]) { //位置相同，但代表但字符串不同，则为重复
            Di[posi] = true; // 重复标记
            return;
        } else if (positions[posi] == pos and i == table[posi]) return; //重复插入，置之不理
        posi++;
        if (posi >= Mod) posi = 2; //越界循环
    }
    table[posi] = i; //载入数据
    positions[posi] = pos;
}
```

对于查询, 照常循环, 判断是否是重复标记以及crc值是否一致:

```c++
inline int get(unsigned pos) {
    unsigned posi = pos % Mod;
    while (positions[posi] and posi <= Mod) {
        if (Di[posi] and pos == positions[posi]) return -1; // 已经重复的单词位置，并且对应的单词hash一致
        else if (positions[posi] == pos) return (int) posi; //找到了
        posi++;
        if (posi >= Mod) posi = 2; //越界循环
    }
    return 0; //没找到
}
```

对于每次查询后, 查询成功的第一个字符都会添加入定长队列中, 每次查询成功, 都会在队列中提取长度大于5的串添加入字典:

```c++
inline void add() {
    char temp[9] = {'\0'};
    if (queue_len >= 6) { //长度大于等于6
        for (int i = 0; i < queue_len - 5; ++i) { // 依次取长度为6,7,8的后缀
            memset(temp, 0, 9);
            memcpy(temp, queue + 2 - i - (8 - queue_len), 6 + i);
            l value = stol(temp);
            unsigned pos = crc32(0, (unsigned char *) temp, strlen((char *) temp));
            pos = crc32(pos, salt, strlen((char *) salt));
            put(pos, value); //放入
        }
    }
}
```



## 复杂度分析:

1. 初始化: O(n) , n为初始化的数量
2. 查询O(1) , 字典特性
3. 查询后添加元素:O(1)
4. 总复杂度O(n + m), m为查询次数, n为初始化次数

## 其他说明:

遇到一个bug, 由于19*8> 2^32 ,用int是装不下的, 但是在中间一个转化函数中:

```c++
inline l stol(char *a) { // 给一个string ,转化为对应的数字
    l ans = 0;
    int pow = 1;  // 这里出现了int ,但是由于int装不下会出现溢出
    for (int i = 0; i < 8; ++i) {
        ans += (l)ctoi(a[i]) * pow;
        pow = pow * 19;
    }
    return ans;
}
```

这个细节导致了几乎全盘的答案错误, 花了好久才找到。

感谢老师和助教的辛苦付出~
