# PA2_1 报告
项晨东 2019011831

## 解题思路：

1. 多叉树的表示采用长子兄弟法，主要涉及的过程包括多叉树的构建，子树移动，规模更新和查询，高度查询。
2. 对于树的构建，因为构建的树不是有序的，所以如果每次寻找相应index的节点必然十分繁琐，考虑直接用指针数组来维护这些节点的指针，这样构建树的速度或许会更快。此外在构建过程中对树规模的更新也需要考虑, 如果一边构建树一边更新规模, 则在树的形态是一条长链时, 复杂度会上升到O(n^2), 所以在树的构建过程中不时时更行树的规模和高度, 等树的链接构建完毕后, 再通过类似宽度优先搜索向下递归的方式来更新高度和规模, 复杂度控制在O(n).
5. 实现树的构建后, 实现节点的查找, 根据节点路径进行寻路。
5. 在子树分离方面, 依据寻路路径找到相应节点, 分离出子树即可, 处理好父亲和接替者关系可以简单的实现。
5. 对于子树插入, 同样依据寻路路径找到相应的节点位置, 插入即可。
5. 对于分离子树和插入子树的规模更新, 直接递归向上更新集合, 复杂度为O(h)。
5. 对于分离子树和插入子树的高度更新, 因为父亲的高度还涉及到兄弟之间的高度, 如果遍历兄弟来寻找高度, 最坏情况下会到O(n)的复杂度, 所以考虑记录最大后缀高度, 子树分离时, 只要从后继节点向长兄更新和维护最大后缀高度, 同时递归向上更新父亲高度; 插入子树时同理; 虽然在最坏情况下依然到达O(n),但是在题目背景下, 复杂度不会超过O(cost), 在题目背景下是有效的。

## 复杂度分析:

### 树的构建

1. 树结构的构建: O(n), 因为根据输入来进行构建树, 链接边的次数和节点数正相关。
2. 高度和规模初始化: O(n) , 遍历所有节点且只遍历一次.

### 树的查询和子树移动

1. 高度和规模查询: O(cost) , 主要计算量在寻找节点上, 寻找节点的过程与cost正相关。
2. 子树移动: O(cost), 子树分离和子树插入的总寻路消耗为O(cost ), 高度更新O(cost), 规模更新O(h), 总的为O(cost)

### 总体

1. 总体时间复杂度为O(N) + O(M * cost) 
2. 总体空间复杂度为O(N)



## 代码简析:

节点定义:

```c++
class Node {
public:
    Node() : left(NULL), right(NULL), elder_brother(NULL), parent(NULL), size(1), height(0), lastchild(NULL),
             max_back_brother_height(-1) {}  //节点定义 
    Node *parent, *left, *right, *elder_brother, *lastchild;//父亲, 儿子, 弟弟, 长兄, 最后一个孩子(方便初始化更新高度
    int height, index, size, max_back_brother_height;
    void get_size() { // 递归向下获取规模
        Node *s = NULL;
        if (left) s = left;
        else return;
        while (s) {
            s->get_size();
            size += s->size;
            s = s->right;
        }
    }
    void get_height() { // 递归向下获取高度
        Node *s;
        if (lastchild)s = lastchild;
        else return;
        while (s) {
            s->get_height();
            if (s->right) s->max_back_brother_height = max(s->right->height, s->right->max_back_brother_height);
            else s->max_back_brother_height = -1;
            s = s->elder_brother;
        }
        if (left) height = max(left->height, left->max_back_brother_height) + 1;
        else height = 0;
    }
};
```

寻路函数:

```c++
inline Node *find_node(Tree *tree) {
    int len, child_index;
    scanf("%d", &len);
    Node *start_node = tree->root;
    Node *find_node = tree->root;
    for (int i = 0; i < len; ++i) {
        scanf("%d", &child_index);
        find_node = tree->find_index_child(start_node, child_index);
        if (find_node) { start_node = find_node; }
        else {
            for (int j = i + 1; j < len; ++j) { scanf("%d", &child_index); }
            return start_node;
        }
    }
    return find_node;
}
```

单次寻路:

```c++
Tree::Node *find_index_child(Node *p, int x) { //查找一个节点第x个孩子
        if (!p->left)return NULL;
        Node *c = p->left;
        for (int i = 0; i < x; ++i) {
            c = c->right;
            if (!c)break;
        }
        return c;
    }
```

高度更新:

```c++
void update_height_above(Node *x) {
        while (x) {
            if (x->right)x->max_back_brother_height = max(x->right->height, x->right->max_back_brother_height);
            else x->max_back_brother_height = -1;
            // 如果有右兄弟，则可以继续维护最大后缀高度
            if (x->elder_brother) x = x->elder_brother; // 向左递归长兄维护最大后缀高度
            else {
                x = x->parent; // 无长兄则向上到父亲
                if (!x) return;  
                if (x->left)x->height = max(x->left->height, x->left->max_back_brother_height) + 1;
                else x->height = 0;
            }
        }
    }
```
## 其他说明
感谢助教和老师的辛苦付出~

