/*
 * read_correct.cpp
 *
 * 读取正确burst的列表，文件格式如下
 *   每行三个数，表示发生burst的流id，burst期间包个数，burst结束时间
 *   类型分别为uint32_t int double
 */

#include "read_correct.h"
#include "sketch/newsketch/param.h"
#include <bits/stdc++.h>
using namespace std;
vector<Burstelem> Tc;

void read_correct() {
    Tc.clear();
    ifstream fin("tmp/correct.txt");
    Burstelem tmp;
    while(fin>>tmp.id>>tmp.cnt>>tmp.t) {
        Tc.push_back(tmp);
    }
    fin.close();
}