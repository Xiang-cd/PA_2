//
// Created by Mac-2读者 on 2021/11/18.
//

#include <iostream>

#define  Debug false

using namespace std;

class Node {
public:
    Node() : left(NULL), right(NULL), elder_brother(NULL), parent(NULL), size(1), height(0), lastchild(NULL),
             max_back_brother_height(-1) {}
    Node *parent, *left, *right, *elder_brother, *lastchild;
    int height, index, size, max_back_brother_height;

    void get_size() {
        Node *s = NULL;
//        cout<<index<<endl;
        if (left) s = left;
        else return;
        while (s) {
            s->get_size();
            size += s->size;
            s = s->right;
        }
    }

    void get_height() {
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


class Tree {
public:
    Node *root;

    Tree() { root = NULL; }

    void update_height_above(Node *x) {
        while (x) {
            if (x->right)x->max_back_brother_height = max(x->right->height, x->right->max_back_brother_height);
            else x->max_back_brother_height = -1;
            // 如果有右兄弟，则可以继续维护最大后缀高度
            if (x->elder_brother) x = x->elder_brother;
            else {
                x = x->parent;
                if (!x) return;  //或许不用加 if x。left
                if (x->left)x->height = max(x->left->height, x->left->max_back_brother_height) + 1;
                else x->height = 0;
            }
        }
    }

    void update_size_above(Node *x, int size) {
        if (size == 0) return;
        while (x->parent) {
            x->parent->size += size;
            x = x->parent;
        }
    }

//    void update_size_init(Node *x) {
//        while (x->parent){
//            x->parent->size += x->size;
//            x =x->parent    ;
//        }
//    }

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
        if (!x->elder_brother) {
            x->parent->left = x->right;
            if (x->right) {
                x->right->elder_brother = NULL; //如果有右兄弟，则设置il，将兄弟指针清空
                update_height_above(x->right);
            } else {
                x->parent->height = 0;
                update_height_above(x->parent);
            }
        } else {
            x->elder_brother->right = x->right;
            if (x->right)x->right->elder_brother = x->elder_brother;
            update_height_above(x->elder_brother);
        }
        update_size_above(x, -x->size);//从父亲开始，减去被删去子树的规模
        return x;
    }

    void insert(Node *target, Node *src, int posi) {
        src->parent = target;
        if (posi == 0) {
            src->elder_brother = NULL;
            src->right = target->left;
            if (src->right) src->right->elder_brother = src;
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
        printf("%d, h: %d, size:%d, parent: %d, eld: %d, smaller: %d, mbh:%d\n", x->index, x->height, x->size,
               x->parent->index,
               (x->elder_brother) ? x->elder_brother->index : -999, (x->right) ? x->right->index : -999,
               x->max_back_brother_height);
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
        if (find_node) { start_node = find_node; }
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
        if (num_of_child == 0)continue;
        for (int j = 0; j < num_of_child; ++j) {
            scanf("%d", &position);
            total[position]->parent = total[i];
            if (j == 0) {
                total[i]->left = total[position];
                tmp = total[position];
            } else {
                tmp->right = total[position];
                total[position]->elder_brother = tmp;
                tmp = total[position];
            }
        }
        total[i]->lastchild = total[position];
//        tree->update_height_above(total[position]);
//        tree->update_size_above(total[i], total[i]->size - 1);
    }

    tree->root->get_size();
    tree->root->get_height();

    if (Debug) {
        for (int i = 1; i <= N; ++i) { print_Node(total[i]); }
        printf("build\n\n");
    }

    for (int i = 0; i < M; ++i) {
        scanf("%d", &type);
        if (type == 0) {
            int insert_posi;
            Node *src = find_node(tree);
            if (Debug) {
                printf("src:\n");
                print_Node(src);
            }
            tree->separate(src);
            Node *target = find_node(tree);
            if (Debug) {
                printf("dst:\n");
                print_Node(target);
                printf("\n");
            }
            scanf("%d", &insert_posi);
            tree->insert(target, src, insert_posi);

        } else if (type == 1) {
            Node *src = find_node(tree);
            if (Debug) { print_Node(src); }
            printf("%d\n", src->height);
            if (Debug) {
                for (int i = 1; i <= N; ++i) { print_Node(total[i]); }
                printf("q height\n\n");
            }
        } else {
            Node *src = find_node(tree);
            if (Debug) { print_Node(src); }
            printf("%d\n", src->size);
            if (Debug) {
                for (int i = 1; i <= N; ++i) { print_Node(total[i]); }
                printf("q size\n\n");
            }
        }
    }

    return 0;
}
