//
// Created by Mac-2读者 on 2021/11/18.
//

#include <iostream>

#define  Debug false

using namespace std;

class Node {
public:
    Node() {
        left = NULL;
        right = NULL;
        height = 0;
        elder_brother = NULL;
        size = 1;
        num_child = 0;
        is_lc = false;
        parent = NULL;
        max_sub_tree_height = -1;
        max_back_brother_height = -1;
    }
    Node *parent;
    Node *left;
    Node *right;
    Node *elder_brother;
    int height;
    int index;
    int size;
    int num_child;
    bool is_lc;
    int max_sub_tree_height;
    int max_back_brother_height;
};


class Tree {
public:
    Node *root;
    Tree() { root = NULL; }

    void update_height_above(Node *x) {
        while (x->parent) {
            if (x->parent->max_sub_tree_height >= x->height) {
                return;
            } else {
                x->parent->max_sub_tree_height = x->height;
                x->parent->height = x->height + 1;
                x = x->parent;
            }
        }
    }

    void reupdate_height_above(Node *x) {
        int max;
        x = x->parent;
        while (x) {
            max = -1;
            for (int i = 0; i < x->num_child; ++i) {
                if (int tmp = find_index_child(x, i)->height > max)max = tmp;
            }
            x->height = max + 1;
            x = x->parent;
        }
    }

    void update_size_above(Node *x, int size) {
        if (size == 0) return;
        while (x->parent) {
            x->parent->size += size;
            x = x->parent;
        }
    }

    Node *find_index_child(Node *p, int x) {
        if (!p->left)return NULL;
        Node *c = p->left;
        for (int i = 0; i < x; ++i) {
            c = c->right;
            if (!c)break;
        }
        return c;
    }

    Node *separate(Node *x) {
        //如果是最长子，则将父亲的孩子指针指向它最大的兄弟
        if (x->is_lc) {
            x->parent->left = x->right;
            if (x->right) {
                //如果有右兄弟，则设置il，将兄弟指针清空
                x->right->is_lc = true;
                x->right->elder_brother = NULL;
            }
        } else {
            x->elder_brother->right = x->right;
            if (x->right) {
                x->right->elder_brother = x->elder_brother;
            }
        }
        update_size_above(x, -x->size);//从父亲开始，减去被删去子树的规模
        x->parent->num_child--;
        reupdate_height_above(x);
        return x;
    }

    void insert(Node *target, Node *src, int posi) {
        target->num_child++;
        src->is_lc = false;
        src->parent = target;
        if (posi == 0) {
            src->is_lc = true;
            src->elder_brother = NULL;
            src->right = target->left;
            target->left = src;
        } else {
            Node *elder = find_index_child(target, posi - 1);
            src->right = elder->right;
            if (src->right)src->right->elder_brother = src;
            elder->right = src;
            src->elder_brother = elder;
        }
        update_size_above(src, src->size);
        update_height_above(src);
    }
};

int N, M;
int num_of_child;
int type, position;
Node *total[1000010] = {NULL};


void print_Node(Node *x) {
    if (x->parent) {
        printf("%d, h: %d, size:%d, parent: %d, il: %d\n", x->index, x->height, x->size, x->parent->index, x->is_lc);
    } else { printf("%d, h: %d, size:%d, root\n", x->index, x->height, x->size); }
}

inline Node *find_node(Tree *tree) {
    int len, child_index;
    scanf("%d", &len);
    Node *start_node = tree->root;
    Node *find_node = tree->root;
    for (int i = 0; i < len; ++i) {
        scanf("%d", &child_index);
        find_node = tree->find_index_child(start_node, child_index);
        if (find_node) { start_node = find_node;}
        else {
            for (int j = i + 1; j < len; ++j) { scanf("%d", &child_index); }
            return start_node;
        }
    }
    return find_node;
}


int main() {
#ifndef _OJ_
//    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    Node *tmp;
    Tree *tree = new Tree();
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; ++i) {
        total[i] = new Node();
        total[i]->index = i;
    }
    tree->root = total[1];

    for (int i = 1; i <= N; ++i) {
        scanf("%d", &num_of_child);
        total[i]->num_child = num_of_child;
        if (num_of_child == 0)continue;
        for (int j = 0; j < num_of_child; ++j) {
            scanf("%d", &position);
            total[position]->parent = total[i];
            if (j == 0) {
                total[i]->left = total[position];
                total[position]->is_lc = true;
                tmp = total[position];
            } else {
                tmp->right = total[position];
                total[position]->elder_brother = tmp;
                tmp = total[position];
            }
        }
        tree->update_height_above(total[position]);
        total[i]->size += num_of_child;
        tree->update_size_above(total[i], num_of_child);
    }
//    if (Debug){for (int i = 1; i <= N; ++i) {print_Node(total[i]);}}



    for (int i = 0; i < M; ++i) {
        scanf("%d", &type);
        if (type == 0) {
            int insert_posi;
            Node *src = find_node(tree);
            if (Debug) { print_Node(src); }
            tree->separate(src);
            Node *target = find_node(tree);
            if (Debug) { print_Node(target); }
            scanf("%d", &insert_posi);
            tree->insert(target, src, insert_posi);

        } else if (type == 1) {
            Node *src = find_node(tree);
            if (Debug) { print_Node(src); }
            printf("%d\n", src->height);
        } else {
            Node *src = find_node(tree);
            if (Debug) { print_Node(src); }
            printf("%d\n", src->size);
        }
    }

    return 0;
}
