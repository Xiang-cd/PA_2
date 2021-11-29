# not found 报告
项晨东 2019011831

## 解题思路：

总体问题: 数据保存, 可以使用bitmap来保存01串, 具体的保存可以开长度为2\^21的字符串数组, 则有2\^24个位, 用位操作对每个位进行读和写。

### 思路1

1. 长度为n的二进制串共有2^n个, 并且在整个串中字符串出现的规律有如下性质:如果长度为n的串都出现过, 则长度小于n的串必然都出现了,  如果长度n的串没有全部出现, 则长度大于n 的串必然没有全部出现。
2. 根据如上性质则可以利用二分查找, 每次扫描保存的bitmap, 检查结果保存到check_bitmap中, 因为每个固定长度二进制串可以对应到唯一一个数字, 在相应的bit位进行标记即可。扫描的宽度就是当前查找的位宽, 如果扫描完后所有的字符串都出现了, 则将位宽扩大再次扫描, 否则将位宽缩小。用二分的方式查找, 找到最后的是最短的未全满的宽度, 此时线性查找check_bitmap第一次未出现的串对应的数字, 转化为二进制串输出即可。

### 思路2

1. 利用前缀的性质, 如果一个串出现, 则它的所有前缀都出现了。则构建一棵树, 每个节点的左孩子是在当前节点代表的串后添加0, 右孩子代表在当前节点后加1的串。 如果一个节点代表的串出现了, 则该节点的所有祖先都出现了, 递归向上标记祖先即可, 如果在中途已经遇到已标记的节点, 则可以停止继续向上标记。如此可以只扫描一次字符串, 不需要将字符串全部保存。
2. 由于树是满树, 所以可以用类似于堆的方式来存储, x的左孩子为x*\2+1, 右孩子为x\*2+2, x 的父亲为(x-1)/2 。



## 复杂度分析:

### 思路1:

1. 时间复杂度为O(log(24)*n) = O(n) 。n为串长。
2. 需要空间为一张2\^24b大小的bitmap和一张2\^24b大小的check_bitmap。 为O(n)

#### 思路2

1. 时间复杂度为O(n*log(24))  = O(n)。n为串长。
2. 需要空间为2\^25b大小的树表示和一个适当长度buffer。为O(n)

## 代码简析:

#### 思路1:

扫描函数:

```c++
int scan(int x) { //当前查找的串长为x
    memset(check_map, 0, sizeof(char) * Len);
    if (x >= len or x <= 0) return -1;
    int mod_value = 1 << x;
    int index;
    if (get_bit(0))index = 1;
    else index = 0;
    int posi = x;
    int count = 0;
    for (int i = 1; i < x; ++i) {//将串转化为数字
        index = index << 1;
        if (get_bit(i)) index ++;
    }
    check(index);
    count++;
    while (posi < len) {
        index = index << 1; //位移操作,相当于多取一位加入串之后
        if (get_bit(posi))index ++;
        index = index % mod_value; // 通过取模运算使得数字表示的串长度始终为x
        if (not get_check_bit(index)) {
            count++;
            check(index);
        }
        ++posi;
    }
    return count;
}
```

#### 思路2:

获取一个字符, 添加到当前的扫描窗口中

```c++
inline void push(char c) {
    if (length < 24) { //长度不足24时
        a[length] = c;
        length++;
        if (a[length - 1] == '1') global_index = RSon(global_index); // 右孩子
        else global_index = LSon(global_index); //左孩子
        mark(global_index); //标记该节点
    } else { //长度大于24, 则
        a[start] = c; 
        start = (start + 1) % 24; // 字符写入buffer,用于最后进行扫尾工作
        if (c == '1') global_index = RSon(global_index);
        else global_index = LSon(global_index); // 此时串长为25, 需要通过处理转化为24
        if (global_index >= (1 << 25) - 1) { // 通过观察树的特征通过平移方法回归到24位串
            int offset = (global_index + 1) % (1 << 24);
            global_index = offset - 1 + (1 << 24);
        }
        mark(global_index);
    }
}
```



## 其他说明

有关思路1和计04周松涛进行了交流, 有关思路2和计04李岩, 叶舟桐进行了交流。
感谢助教和老师的辛苦付出~

