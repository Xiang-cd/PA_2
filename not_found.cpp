//
// Created by 项小羽 on 2021/11/21.
//

#include <iostream>
#include <string.h>

#define Debug false

#define Len 1 << 21

using namespace std;


char list[Len] = {'\0'};
char check_map[Len] = {'\0'};
int len = 0;
char a[24] = {'0'};

char get_bit(int index) {
    return (list[index >> 3] >> (7 - (index % 8))) & 1;
}

char get_check_bit(int index) {
    return (check_map[index >> 3] >> (index % 8)) & 1;
}

void check(int index) {
    check_map[index >> 3] = check_map[index >> 3] | (1 << (index % 8));
}

int scan(int x) {
    memset(check_map, 0, sizeof(char) * Len);
    if (x >= len or x <= 0) return -1;
    int mod_value = 1 << x;
    int index;
    if (get_bit(0))index = 1;
    else index = 0;
    int posi = x;
    int count = 0;
    for (int i = 1; i < x; ++i) {
        index = index << 1;
        if (get_bit(i)) index ++;
    }
    check(index);
    count++;
    while (posi < len) {
        index = index << 1;
        if (get_bit(posi))index ++;
        index = index % mod_value;
        if (not get_check_bit(index)) {
            count++;
            check(index);
        }
        ++posi;
    }
    return count;
}

void print(int num, int length) {
    memset(a, '0', sizeof(char) * 24);
    int i = 0;
    while (num != 0) {
        if (num % 2) { a[i++] = '1'; }
        else a[i++] = '0';
        num = num / 2;
    }
    for (int j = length - 1; j >= 0; --j) {
        printf("%c", a[j]);
    }
    printf("\n");
}

void binary_search() {
    int low = 1;
    int high = 24;
    int mid, ans;
    while (low < high) {
        mid = (low + high) / 2;
        ans = scan(mid);
        if (ans == -1) {
            high = mid;
            continue;
        } else {
            if (ans == 1 << mid) {
                low = mid + 1;
                continue;
            } else {
                high = mid;
            }
        }
    }
    if (Debug) {
        printf("%d %d %d\n", low, mid, high);
    }
    scan(high);
    for (int i = 0; i < 1 << high; ++i) {
        if (not get_check_bit(i)) {
            if (Debug) { printf("%d\n", i); }
            print(i, high);
            break;
        }
    }
}

int main() {
#ifndef _OJ_
    //    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
    //    freopen("/Users/xxy/CLionProjects/DSA/PA_2/output.txt", "w", stdout);
#endif
char ch;
memset(list, 0, sizeof(char) * Len);
while (true) {
    ch = getchar();
    if (ch != '0' and ch != '1')break;
    list[len / 8] = char(list[len / 8] << 1);
    if (ch - '0')list[len / 8] = char(list[len / 8] | 1);
    ++len;
}
if (len % 8) {
    list[len / 8] = list[len / 8] << (8 - len % 8);
}
if (Debug) {
    for (int i = 0; i < len; ++i) {
        printf("%d", get_bit(i));
    }
    cout << endl;
}
if (len == 0) {
    printf("0");
    return 0;
}
if (len == 1) {
    if (get_bit(0)) {
        printf("0");
        return 0;
    } else {
        printf("1");
        return 0;
    }
}
binary_search();
return 0;
}
