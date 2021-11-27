#include <iostream>
#include <string.h>

#define Debug true
#define Len  200000
#define Mid(x, y) ((x)+(y))/2
#define LSon(x) 2*(x)+1
#define RSon(x) 2*(x)+2
#define Parent(x)  ((x)-1)/2

using namespace std;
int N, M, num_of_seg;

struct seg {
    seg() : begin(0), len(0) {}

    int begin, len;
};

bool hq[Len] = {0}; // H 为1，Q为0
int startpoints[Len] = {0};
int endpoints[Len] = {0};
long total_points[2 * Len] = {0};
int int_start[Len * 2] = {0};
int int_len[Len * 2] = {0};
seg segments[Len * 2] = {};
int st_len[4 * Len] = {0};
int st_value[4 * Len] = {0};
int st[4 * Len] = {0};


void quicksort(long a[], int start, int end) {
    if (start >= end - 1)return;
    int pivot = a[start];
    int index = start;
    int right = end - 1;
    for (int i = 0; i < end - start - 1; ++i) {
        if (a[index + 1] > pivot) {
            int tmp = a[index + 1];
            a[index + 1] = a[right];
            a[right] = tmp;
            right--;
        } else {
            int tmp = a[index];
            a[index] = a[index + 1];
            a[index + 1] = tmp;
            index++;
        }
    }
    quicksort(a, start, index);
    quicksort(a, index + 1, end);
}

int construct(int start, int end, int si) {
    if (left == end) {
        st[si] = segments[start].begin;
        st_len[si] = segments[start].len;
        return st_len[si];
    }
    st[si] = segments[start].begin;
    int mid = Mid(start, end);
    return st_len[si] = construct(start, mid, si * 2 + 1) + construct(start + 1, end, si * 2 + 2);
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
    for (int i = 0; i < num_of_seg; ++i) {
        printf("%d %d\n", segments[i].begin,segments[i].len);
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

int main() {
#ifndef _OJ_
    //    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
//    freopen("/Users/xxy/CLionProjects/DSA/PA_2/output.txt", "w", stdout);
#endif
    read();
    quicksort(total_points, 0, 2 * M);
    if (Debug) {
        for (int i = 0; i < 2 * M; ++i) printf("%ld ", total_points[i]);
        cout << endl;
    }

    build(2 * M);
    if (Debug) {
        printf("%d %d\n", st[0], st_len[0]);
        printf("%d %d    %d %d        \n", st[LSon(0)], st_len[LSon(0)], st[RSon(0)], st_len[RSon(0)]);
        printf("%d %d    %d %d    %d %d    %d %d", st[LSon(1)], st_len[RSon(1)], st[RSon(1)], st[RSon(1)], st[LSon(2)],
               st_len[
                       LSon(2)], st[RSon(2)], st_len[RSon(2)]);

    }

    return 0;
}
