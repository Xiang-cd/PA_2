//
// Created by 项小羽 on 2021/11/26.
//

#include <iostream>
#include <stdlib.h>

#define Debug false
#define Time false
#define Len  500000
#define Mid(x, y) ((x)+(y))/2
#define LSon(x) 2*(x)+1
#define RSon(x) 2*(x)+2
#define Parent(x)  ((x)-1)/2

using namespace std;
typedef long long ll;
int N, M, num_of_seg;

struct seg {
    seg() : begin(0), len(0) {}

    int begin, len;
};

struct tseg {
    tseg() : begin(0), len(0), start_index(-1), lazy(0), size(0) {}

    int begin, len, start_index, lazy;
    ll  size;
};

bool hq[Len] = {false}; // H 为1，Q为0
int startpoints[Len] = {0};
int endpoints[Len] = {0};
long total_points[2 * Len] = {0};
seg segments[Len * 2] = {};
tseg segTree[Len * 4] = {};
clock_t startT, endT;

void prb() {
    for (int i = 0; i < 4 * M; ++i) {
        printf("i:%d p:%d st:%d len:%d si:%d  lz:%d sz:%lld\n", i, Parent(i), segTree[i].begin, segTree[i].len,
               segTree[i].start_index, segTree[i].lazy, segTree[i].size);
    }
    cout << endl;
}

int construct(int start, int end, int si) {
    if (start + 1 >= end) {
        segTree[si].begin = segments[start].begin;
        segTree[si].len = segments[start].len;
        segTree[si].start_index = start;
        return segTree[si].len;
    }
    segTree[si].begin = segments[start].begin;
    int mid = Mid(start, end);
    return segTree[si].len = construct(start, mid, si * 2 + 1) + construct(mid, end, si * 2 + 2);
}

void build(int len) {
    num_of_seg = 0;
    // 先确定区间，再组成平衡树
    for (int i = 0; i < len - 1; ++i) {
        if (total_points[i] == total_points[i + 1]) continue;
        segments[num_of_seg].begin = total_points[i];
        segments[num_of_seg].len = total_points[i + 1] - total_points[i];
        ++num_of_seg;
    }
    if (Debug) {
        for (int i = 0; i < num_of_seg; ++i) {
            printf("%d %d\n", segments[i].begin, segments[i].len);
        }
    }
    construct(0, num_of_seg, 0);
}

void read() {
    scanf("%d%d", &N, &M);
    char tmp;
    for (int i = 0; i < M; ++i) {
        cin >> tmp;
        scanf(" %d %d", &startpoints[i], &endpoints[i]);
        total_points[2 * i] = startpoints[i];
        total_points[2 * i + 1] = endpoints[i] + 1;
        if (tmp == 'H') hq[i] = true;
    }
}

ll  flip(int node, int start, int end) {
    int left = segTree[node].begin;
    int right = segTree[node].begin + segTree[node].len;
    if (segTree[node].len == 0)return 0;

    if (left >= start and right <= end) { //全包含,为祖先节点提供相应的值
        segTree[node].lazy++;
        if (Debug)printf("lazy %d rt %d\n",node,segTree[node].len);
        return (ll) segTree[node].len;
    } else if (left >= end or right <= start) { return 0; }
    else {
        ll sz = flip(LSon(node), start, end) +flip(RSon(node), start, end);
        segTree[node].size += sz;
        return sz;
    }
}

inline ll  qurey(int node, int start, int end) {
    int left = segTree[node].begin;
    int right = segTree[node].begin + segTree[node].len;
    if (segTree[node].len == 0) return 0;
    int lazy = segTree[node].lazy;

    if (lazy and segTree[node].start_index == -1) {
        if (Debug) { printf("lazy%d %d %d\n", node, LSon(node), RSon(node)); }
        segTree[RSon(node)].lazy += lazy;
        segTree[LSon(node)].lazy += lazy;
    }
    segTree[node].size += (ll) (ll) segTree[node].lazy * (ll) segTree[node].len;
    segTree[node].lazy = 0;

    if (left >= end or right <= start) { // 无重叠
        return 0;
    } else if (left >= start and right <= end) { // 全重叠
        return segTree[node].size + (ll)(ll)segTree[node].len*(ll)segTree[node].lazy;
    } else {
        ll  ln = qurey(LSon(node), start, end);
        ll  rn = qurey(RSon(node), start, end);
        if (Debug) {
            printf("node %d get from  nd%d %lld  nd%d %lld\n", node, LSon(node), ln, RSon(node), rn);
        }
        return ln + rn;
    }
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int main() {
#ifndef _OJ_
    //    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
    //    freopen("/Users/xxy/CLionProjects/DSA/PA_2/output.txt", "w", stdout);
#endif
if (Time) startT = clock();
read();
qsort(total_points, 2 * M, sizeof(long), cmpfunc);
if (Debug) {
    for (int i = 0; i < 2 * M; ++i) printf("%ld ", total_points[i]);
    cout << endl;
}

build(2 * M);
if (Debug) {
    prb();
}

for (int i = 0; i < M; ++i) {
    if (hq[i]) {
        flip(0, startpoints[i], endpoints[i] + 1);
        if (Debug) {
            cout << "fl" << startpoints[i] << " " << endpoints[i] << endl;
            prb();
        }
    } else {
        cout << qurey(0, startpoints[i], endpoints[i] + 1) << endl;
        if (Debug) {
            cout << "qr" << startpoints[i] << " " << endpoints[i] << endl;
            prb();
        }
    }
}
if (Time) {
    endT = clock();
    cout << "花费了" << (double) (endT - startT) / CLOCKS_PER_SEC << "秒" << endl;
}
return 0;
}
