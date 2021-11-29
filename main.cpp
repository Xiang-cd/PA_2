#include <iostream>
#include "crc32.h"
#include <string.h>

#define Debug false
#define Time false
#define Mod 9999991
#define Size 10000000

using namespace std;
typedef long l;
clock_t startT, endT;
int N;
unsigned char gtmp[10] = "";
char queue[9] = {};
int queue_len = 0;
l inputindex;
l table[Size] = {0};
unsigned positions[Size] = {0};
bool Di[Size] = {false};
unsigned char salt[6] = "";
string alpha = "0123456789tsinghua";

inline void push(char a) {
    if (queue_len < 8) {
        queue[queue_len] = a;
        queue_len++;
    } else {
        memmove(queue, queue + 1, 7);
        queue[7] = a;
    }
}


inline void read() {
    cin >> N;
    cin >> salt;
}

inline int ctoi(char a) {
    if (a <= '9' and a >= '0')return a + 1 - '0';
    switch (a) {
        case 't':
            return 11;
        case 's':
            return 12;
        case 'i':
            return 13;
        case 'n':
            return 14;
        case 'g':
            return 15;
        case 'h':
            return 16;
        case 'u':
            return 17;
        case 'a':
            return 18;
        default:
            return 0;
    }
}

inline char itoc(int x) {
    if (x > 0 and x <= 18)return alpha[x - 1];
    else return '\0';
}

inline l stol(char *a) {
    l ans = 0;
    l pow = 1;
    for (int i = 0; i < 8; ++i) {
        ans += (l)ctoi(a[i]) * pow;
        pow = pow * 19;
    }
    return ans;
}

inline void ltos(l x) {
    memset(gtmp, 0, 9);
    for (int i = 0; i < 8; ++i) {
        gtmp[i] = itoc(int(x % 19));
        x = x / 19;
    }
}

inline void put(unsigned pos, l i) {
    ltos(i);
    unsigned posi = pos % Mod;
    while (table[posi]) {
        if (positions[posi] == pos and i != table[posi]) { //位置相同，但代表但字符串不同，则为重复
            Di[posi] = true;
            return;
        } else if (positions[posi] == pos and i == table[posi]) return; //重复插入，置之不理
        posi++;
        if (posi >= Mod) posi = 2;
    }
    table[posi] = i;
    positions[posi] = pos;
}

inline int get(unsigned pos) {
    unsigned posi = pos % Mod;
    while (positions[posi] and posi <= Mod) {
        if (Di[posi] and pos == positions[posi]) return -1; // 已经重复的单词位置，并且对应的单词hash一致
        else if (positions[posi] == pos) return (int) posi; //找到了
        posi++;
        if (posi >= Mod) posi = 2;
    }
    return 0;
}

bool is_enmpty(l i) {
    while (i > 0) {
        if (i % 19 == 0) return true;
        i = i / 19;
    }
    return false;
}

void init() {
    for (l i = 0; i < 19 * 19 * 19 * 19 * 19; ++i) {
        if (is_enmpty(i))continue;
        ltos(i);
        unsigned pos = crc32(0,  gtmp, strlen((char *) gtmp));
        pos = crc32(pos, salt, strlen((char*)salt));
        put(pos, i);
    }
}

inline void add() {
    char temp[9] = {'\0'};
    if (queue_len >= 6) {
        for (int i = 0; i < queue_len - 5; ++i) {
            memset(temp, 0, 9);
            memcpy(temp, queue + 2 - i - (8 - queue_len), 6 + i);
            if (Debug)cout << temp << endl;
            l value = stol(temp);
            unsigned pos = crc32(0, (unsigned char *) temp, strlen((char *) temp));
            pos = crc32(pos, salt, strlen((char *) salt));
            put(pos, value);
        }
    }
}

int main() {
#ifndef _OJ_
    //    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
//    freopen("/Users/xxy/CLionProjects/DSA/PA_2/output.txt", "w", stdout);
#endif
    if (Time) startT = clock();
    read();
    init();
    for (int i = 0; i < N; ++i) {
        cin >> hex >> inputindex;
        unsigned posi = get(inputindex);
        if (posi == -1)cout << "Duplicate" << endl;
        else if (posi == 0) cout << "No" << endl;
        else {
            ltos(table[posi]);
            cout << gtmp << endl;
            push(gtmp[0]);
            add();
        }
    }
    if (Time) {
        endT = clock();
        cout << "cost" << (double) (endT - startT) / CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}
