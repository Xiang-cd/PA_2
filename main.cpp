#include <iostream>
#include <string.h>

#define Len 1 << 21

using namespace std;


char list[Len] = {'\0'};
char check_map[Len] = {'\0'};
int len = 0;
char a[24] = {'0'};

char get_bit(int index, int position) {
    return  list[index] & (1 << position);
}

int scan(int x) {
    memset(check_map, 0, sizeof(char) * Len);
    if (x >= len or x <= 0) return -1;
    int mod_value = 1 << x;
    int index = int();
    int posi = x;
    int count = 0;
    for (int i = 1; i < x; ++i) {
        index *= 2;
        if (get_bit(i/8,i%8)) index++;
    }
    check_map[index] = true;
    count++;
    while (posi < len) {
        index = index * 2;
        if (list[posi])index++;
        index = index % mod_value;
        if (not check_map[index]) {
            count++;
            check_map[index] = true;
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
}

void binary_search() {
    int low = 1;
    int high = 24;
    int mid;
    int ans, ans_before;
    while (low <= high) {
        mid = (low + high) / 2;
        ans_before = scan(mid - 1);
        ans = scan(mid);
        if (ans == -1) {
            high = mid - 1;
            continue;
        } else {
            if (ans == 1 << mid) {
                low = mid + 1;
                continue;
            } else {
                if (ans_before == 1 << (mid - 1)) {
                    for (int i = 0; i < 2 << mid; ++i) {
                        if (not check_map[i]) {
                            print(i, mid);
                            break;
                        }
                    }
                    break;
                } else {
                    high = mid - 1;
                }
            }
        }
    }
}

int main() {
#ifndef _OJ_
//    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    char ch;
    memset(list, 0, sizeof(char) * Len);
    while ((ch = getchar()) != EOF) {
        list[len / 8] = list[len / 8] << 1;
        if (ch - '0')list[len / 8] += 1;
        ++len;
    }

    binary_search();
    return 0;
}
