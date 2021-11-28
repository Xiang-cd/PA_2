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
#define Time true
clock_t startT, endT;

using namespace std;

char check_map[(Len) * 2] = {'\0'};
char a[25] = {0};
int length = 0;

void push(char c) {
    if (length < 24) {
        a[length] = c;
        length++;
    } else {
        memmove(a, a + 1, 23);
        a[23] = c;
    }
}

char get_check_bit(int index) {
    return (check_map[index >> 3] >> (index % 8)) & 1;
}

void check(int index) {
    check_map[index >> 3] = check_map[index >> 3] | (1 << (index % 8));
}

void mark(int index) {
    while (index) {
        if (get_check_bit(index))break;
        check(index);
        index = Parent(index);
    }
}

void scan() {
    int index = 0;
    for (int i = 0; i < length; ++i) {
        if (a[i] == '1') index = RSon(index);
        else index = LSon(index);
    }
    mark(index);
}

void getout(int i) {
    char tmp[25] = "";
    int length = 0;
    while (i) {
        if (i & 1) tmp[length] = '0';
        else tmp[length] = '1';
        length++;
        i = Parent(i);
    }
    for (int j = length - 1; j >= 0; --j) {
        printf("%c", tmp[j]);
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
        if (Debug)cout << a << endl;
        scan();
    }
    int round = length;
    for (int i = 0; i < round; ++i) {
        memmove(a, a + 1, length - 1);
        a[length - 1] = 0;
        if (Debug)cout << a << endl;
        length--;
        scan();
    }

    for (int i = 1; i < 1 << 24; ++i) {
        if (!get_check_bit(i)) {
            if (Debug) cout << i << endl;
            getout(i);
            break;
        }
    }
    cout<<endl;
    if (Time) {
        endT = clock();
        cout << "cost" << (double) (endT - startT) / CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}
