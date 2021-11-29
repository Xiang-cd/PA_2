//
// Created by 项小羽 on 2021/11/29.
//

#include <iostream>
#include <string.h>
#include "crc32.h"

using namespace std;
#define Debug false
#define Time false
#define datagen false
clock_t sT;
#define NN cout<<endl;
#define PD(x) printf("%d ",(x));
#define Tik if(Time)sT=clock();
#define Tok if(Time)cout<<(double)(clock() - sT)/CLOCKS_PER_SEC<<"s"<< endl;
typedef long  l;
typedef unsigned us;

struct container {
    l data;
    us crc;
    container *next;

    container() : data(0), crc(0), next(NULL) {}
};

#define Mod 5999947
#define Size 6000000
container datas[Size] = {};
unsigned char gtmp[10] = "";
unsigned char salt[5] = "";
int N;
int queue_len = 0;
char queue[19] = "";
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
        ans += (l) ((l)ctoi(a[i]) * (l) pow);
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


bool empty(l x) {
    while (x) {
        if (x % 19 == 0) return true;
        x = x / 19;
    }
    return false;
}

inline us getcrc() {
    unsigned pos = crc32(0, gtmp, strlen((char *) gtmp));
    return crc32(pos, salt, strlen((char *) salt));
}

void put(us cv, l data) {
    container *target = &datas[cv % Mod];
    if (!target) target = new container();
    while (target->data) {
        if (target->crc == cv) {
            if (target->data == data)return;
            else {
                target->data = -1;
                return;
            }
        }
        if (!target->next)target->next = new container();
        target = target->next;
    }
    target->crc = cv;
    target->data = data;
//    cout<<target->crc<<target->data<<endl;
}

container *get(us cv) {
    container *target = &datas[cv % Mod];
    while (target) {
        if (target->crc == cv) return target;
        target = target->next;
    }
    return target;
}


void init() {
    for (l i = 0; i < 19 * 19 * 19 * 19 * 19; ++i) {
        if (empty(i))continue;
        ltos(i);
        put(getcrc(), i);
    }
}

inline void add() {
    unsigned char temp[9] = {'\0'};
    if (queue_len >= 6) {
        for (int i = 0; i < queue_len - 5; ++i) {
            memset(temp, 0, 9);
            memcpy(temp, queue + 2 - i - (8 - queue_len), 6 + i);
            l value = stol((char *) temp);
            unsigned pos = crc32(0, temp, strlen((char *) temp));
            pos = crc32(pos, salt, strlen((char *) salt));
            put(pos, value);
            if (Debug) {
                printf("%u %lld %s \n", pos, value, temp);
            }
        }
    }
}


void read() {
    cin >> N;
    cin >> salt;
}

int main() {
#ifndef _OJ_
    //    freopen("/Users/iMac-2/CLionProjects/PA_2/input.txt", "r", stdin);
    if (datagen) {
        freopen("/Users/xxy/CLionProjects/DSA/PA_2/input2.txt", "r", stdin);
        freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "w", stdout);
    } else {
        freopen("/Users/xxy/CLionProjects/DSA/PA_2/input.txt", "r", stdin);
    }

#endif
    Tik
    if (datagen) {
        cin >> N;
        cin >> salt;
        cout << N << endl;
        cout << salt << endl;
        for (int i = 0; i < N; ++i) {
            memset(gtmp, 0, 9);
            cin >> gtmp;
            cout << hex << getcrc() << endl;
        }
    } else {
        read();
        init();
        l input;
        for (int i = 0; i < N; ++i) {
            cin >> hex >> input;
            container *target = get(input);
            if (!target) {
                cout << "No";
                NN
            } else if (target->data == 0) {
                cout << "No" << endl;
            } else {
                if (target->data == -1) cout << "Duplicate" << endl;
                else {
                    ltos(target->data);
                    cout << gtmp << endl;
                    push((char) gtmp[0]);
                    add();
                }
            }
        }
    }


    Tok
    return 0;
}