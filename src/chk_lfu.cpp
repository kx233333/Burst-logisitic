/*
 * chk_lfu.cpp
 *
 * 运行lfu算法，使用read_dataset读取数据集，直接在stdout中输出一个0到1之间的hitrate
 * 
 * usage: ./chk_lfu cache_size
 * 其中cache_size是缓存条目个数
 */

#include "util/BOBHash32.h"
#include "util/read_dataset.h"
#include <bits/stdc++.h>
using namespace std;

const int T=8;

extern vector<DS_Pack> DS;
int main(int argc, char **argv) {
    int sz;
    sscanf(argv[1], "%d", &sz);
    const int N=10000000;
    init_dataset(N);
    
    vector<vector<uint32_t> > id(sz/T, vector<uint32_t>(T, 0));
    vector<vector<uint32_t> > cnt(sz/T, vector<uint32_t>(T, 0));
    int hit=0;
    int write = 0;
    BOBHash32* bobhash = new BOBHash32(19);

    for(int i=0;i<N;i++) {
        uint32_t idv = DS[i].id;
        int hshv = bobhash->run((char*)&idv, sizeof(uint32_t)) % (sz / T);
        int pos = -1;
        for(int j=0;j<T;j++) {
            if(id[hshv][j]==idv) {
                hit++;
                cnt[hshv][j]++;
                pos = -2;
                break;
            }
            if(pos == -1 || cnt[hshv][j] < cnt[hshv][pos]) {
                pos = j;
            }
        }
        if(pos == -1 || pos == -2) continue;
        id[hshv][pos] = idv;
        cnt[hshv][pos] = 1;
        ++write;
    }
    printf("%.10f %d\n", 1.0*hit/N, write);
}