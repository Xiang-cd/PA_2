# kidd 报告
项晨东 2019011831

## 解题思路:

1. 数据离散化, 由于牌的数量非常对, 但是询问和翻转次数有限, 所以不是所以有的牌都需要保存和记录, 只需要记录每次操作的端点即可。读取所有出现的节点, 进行排序后, 整理为一个个左闭右开的线段, 实现数据离散化, 节省存储空间。
2. 线段树, 这些线段可以组织成一棵线段树, 对于线段树的构造, 按照线段数组的下标中位数二分区域进行节点的封装(不是线段表示总范围的中点, 而是将线段按序存入数组, 数组下标的中点)为树的节点, 递归向下, 直到节点内只包含一个线段, 对于树的表示, 可以用类似于堆的存储, 节点的左孩子下标为x\*2+1, 又孩子下标为x\*2 +2 , x 的父亲下标为(x-1)/2。
3. 翻转时, 如果线段完全在区间内, 则标记为懒惰标记, 不继续向下递归, 同时返回当前区间长度, 告诉祖先自己更新后对于祖先来说的翻转次数。对于孩子节点返回的翻转次数, 直接加入到该节点的翻转次数中。
4. 查询时, 递归向下调用查询函数, 每次访问节点, 都将懒惰标记下放, 同时将自己的懒惰标记转为自己节点代表的线段翻转次数, 并添加到自己记录的翻转次数。

## 复杂度分析:

n为所有出现的端点个数

1. 线段整理, 一次快速排序, 为O(n*log(n))
2. 树的构建: 每次访问一个节点且访问一次, 为O(n), 因为总的节点数不超过2n, 如果烤炉最坏情况, 总节点数不超过4n。
3. 翻转操作: 最多不超过树高, 并且结合懒惰标记, 只有区间边缘的部分会递归到叶节点, 所以复杂度为O(log(n))
4. 查询操作: 由于查询的是翻转次数, 而只有区间边缘的部分可能会递归到叶节点所以复杂度为O(log(n)), 因为不存在线性探测报告的部分, 所以没有r的部分。
5. 总体复杂度为O(n\*logn + M\*log(n))
6. 空间复杂度为O(n)

## 代码简析:

翻转:

```c++
ll  flip(int node, int start, int end) {
    int left = segTree[node].begin;
    int right = segTree[node].begin + segTree[node].len;
    if (segTree[node].len == 0)return 0;
    if (left >= start and right <= end) { //全包含,为祖先节点提供相应的值
        segTree[node].lazy++;
        return (ll) segTree[node].len;
    } else if (left >= end or right <= start) { return 0; } // 无重叠, 返回0
    else {
        ll sz = flip(LSon(node), start, end) +flip(RSon(node), start, end); // 半重叠, 递归向下
        segTree[node].size += sz;
        return sz;
    }
}
```

查询:

```c++
inline ll  qurey(int node, int start, int end) {
    int left = segTree[node].begin;
    int right = segTree[node].begin + segTree[node].len;
    if (segTree[node].len == 0) return 0;
    int lazy = segTree[node].lazy;
 // 下放lazy
    if (lazy and segTree[node].start_index == -1) { // 可下放, 并且不是叶节点
        segTree[RSon(node)].lazy += lazy;
        segTree[LSon(node)].lazy += lazy;
    }
    segTree[node].size += (ll) (ll) segTree[node].lazy * (ll) segTree[node].len; // 将lazy计入自己节点
    segTree[node].lazy = 0; 
    if (left >= end or right <= start) { // 无重叠
        return 0;
    } else if (left >= start and right <= end) { // 全重叠
        return segTree[node].size + (ll)(ll)segTree[node].len*(ll)segTree[node].lazy;
    } else { // 递归向下
        ll  ln = qurey(LSon(node), start, end);
        ll  rn = qurey(RSon(node), start, end);
        return ln + rn;
    }
}
```

构建:

```c++
int construct(int start, int end, int si) { // 线段数组的开始下标和结束下标, 当前要构建节点的编号
    if (start + 1 >= end) { //如果是叶节点
        segTree[si].begin = segments[start].begin;
        segTree[si].len = segments[start].len;
        segTree[si].start_index = start;
        return segTree[si].len;
    }
    segTree[si].begin = segments[start].begin;
    int mid = Mid(start, end);
    return segTree[si].len = construct(start, mid, si * 2 + 1) + construct(mid, end, si * 2 + 2);
  //切分数组向下递归
}
```



## 其他说明:

参考了:  https://www.geeksforgeeks.org/segment-tree-set-1-sum-of-given-range/

感谢助教和老师的辛苦付出~

