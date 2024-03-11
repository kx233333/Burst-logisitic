/*
 * chk_cache.cpp
 *
 * 给定缓存日志，计算缓存效率并在stdout中输出一个0到1之间的hitrate
 * usage: ./chk_cache log_name
 * 
 * 缓存日志格式如下：
 *     每行一个条目，可能为insert %id/evict %id/visit %id，表示一次缓存插入/删除/访问操作
 *     id是32位无符号整数
 */

#include <bits/stdc++.h>
using namespace std;

map<uint32_t, int> Cache;


int main(int argc, char **argv) {
    ifstream fin(argv[1]);
    string s;
    uint32_t id;
    int tot=0, hit=0;
    int write = 0;
    while(fin>>s>>id) {
        if(s=="insert") {
            ++write;
            Cache[id]=1;
        }
        if(s=="evict") {
            Cache[id]=0;
        }
        if(s=="visit") {
            tot++;
            hit+=Cache[id];
        }
    }
    printf("%.10f %d\n", 1.0 * hit / tot, write);
}