//
// Created by Mac-2读者 on 2021/11/28.
//
#include <iostream>
#include <string.h>

#define Debug false
#define LSon(x) 2*(x)+1
#define RSon(x) 2*(x)+2
#define Parent(x)  ((x)-1)/2
#define Len 1 << 21
#define Time false
clock_t startT, endT;

using namespace std;

char check_map[(Len) * 2] = {'\0'};
char a[25] = {0};
int length = 0;
int start = 0;
int global_index = 0;


inline char get_check_bit(int index) {
    return (check_map[index >> 3] >> (index % 8)) & 1;
}

inline void check(int index) {
    check_map[index >> 3] = check_map[index >> 3] | (1 << (index % 8));
}

inline void mark(int index) {
    while (index) {
        if (get_check_bit(index)) { break; }
        check(index);
        index = Parent(index);
    }
}

void getout(int i) {
    char tmp[25] = "";
    int tmplen = 0;
    while (i) {
        if (i & 1) tmp[tmplen] = '0';
        else tmp[tmplen] = '1';
        tmplen++;
        i = Parent(i);
    }
    for (int j = tmplen - 1; j >= 0; --j) {
        printf("%c", tmp[j]);
    }
    cout << endl;
}


inline void push(char c) {
    if (length < 24) {
        a[length] = c;
        length++;
        if (a[length - 1] == '1') global_index = RSon(global_index);
        else global_index = LSon(global_index);
        mark(global_index);
        if (Debug) {
            cout << a << endl;
            cout << global_index << endl;
        }

    } else {
        a[start] = c;
        start = (start + 1) % 24;
        if (Debug) {
            for (int i = 0; i < 24; ++i) {
                cout << a[(start + i) % 24];
            }
            cout << endl;
        }
        if (c == '1') global_index = RSon(global_index);
        else global_index = LSon(global_index);
        if (global_index >= (1 << 25) - 1) {
            int offset = (global_index + 1) % (1 << 24);
            global_index = offset - 1 + (1 << 24);
        }
        mark(global_index);
    }
}


int main() {
#ifndef _OJ_
    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
//    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
    //    freopen("/Users/xxy/CLionProjects/DSA/PA_2/output.txt", "w", stdout);
#endif
    if (Time) startT = clock();
    char ch;
    while (true) {
        ch = getchar();
        if (ch != '0' and ch != '1')break;
        push(ch);
    }

    int round = length;
    char b[25] = "";
    for (int i = 0; i < length; ++i) b[i] = a[(start + i) % 24];
    for (int i = 0; i < round; ++i) {
        memmove(b, b + 1, length - 1);
        b[length - 1] = 0;
        if (Debug)cout << b << endl;
        length--;
        int index = 0;
        for (int i = 0; i < length; ++i) {
            if (b[i] == '1') index = RSon(index);
            else index = LSon(index);
        }
        mark(index);
    }

    for (int i = 1; i < 1 << 24; ++i) {
        if (!get_check_bit(i)) {
            if (Debug) cout << i << endl;
            getout(i);
            break;
        }
    }

    if (Time) {
        endT = clock();
        cout << "cost" << (double) (endT - startT) / CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}
