/*
    run_correct.cpp

    计算正确的burst，输出到 tmp/correct.txt 中
    
    usage: ./run_correct p(ratio) stdlim(burstlim) threshold
    分别为比例系数，burst加权平均的阈值，包个数的要求（论文中定义为1，即没有要求）

*/

#include "../src/util/read_dataset.h"
#include "../src/sketch/newsketch/correct.h"
#include "../src/sketch/newsketch/param.h"
#include <bits/stdc++.h>
#include "../src/util/BOBHash32.h"
using namespace std;

/*
struct Param {
    double _stdlim;
    double _p;
    double _outv;
    int _threshold;
    double _chklim;
};*/
//需要调的参

extern vector<Burstelem> Tc;
extern vector<DS_Pack> DS;


//limit 限制出现次数小于limit次的元素不统计
//columns 生成的前驱元素的维数
//数据规模N=100000时运行时间太长，改成10000 f1score很低
//
int main(int argc, char **argv) {
    struct Param P = Param{0.005, 0.5, 0.5, 1, 0.05, 50000, 10000};
    int limit = 2, columns = 10;
    // struct Param P = Param{0.005, 0.9, 0.5, 50, 0.05, 50000, 10000};
    if(argc > 1) {
        sscanf(argv[1], "%lf", &P._p);
    }
    if(argc > 2) {
        sscanf(argv[2], "%d", &limit);
    }
    if(argc > 3) {
        sscanf(argv[3], "%d", &columns);
    }
    load_param(P);
    CorrectSketch T;
    

    vector<int> pre;
    unordered_map<id_type, int> Dict;
    const int N = 50000;
    init_dataset(N);
    cerr << "awsl " << DS.size () << endl;
    cerr << DS [0].id << ' ' << DS [0].tm << endl;
    cerr << DS [DS.size () - 1].id << ' ' << DS [DS.size () - 1].tm << endl;
    FILE *fdata = fopen("../Logistic/data/data.csv", "w");
    for(int i=0;i<N;i++) {
        int tmp = T.insert(DS[i].id, DS[i].tm, 1);
        if(Dict.find(DS[i].id) != Dict.end()) pre.push_back(Dict[DS[i].id]);
        else pre.push_back(-1);
        Dict[DS[i].id] = i;
        int ooo = limit, now = i;
        bool flag = 0;
        while(ooo--){
            if(pre[now] == -1) {
                flag = 1;
                break;
            }
            now = pre[now];
        }
        if(flag) continue;
        int vector_num = columns;
        now = i;
        vector<double> tim_list;
        while(vector_num--) {
            if(now == -1){
                fprintf(fdata, "%.10f,", 0.0);
                continue;
            }
            fprintf(fdata, "%.10f,", DS[now].tm);
            now = pre[now];
        }
        fprintf(fdata, "%d\n", tmp);
        if (i % 100000 == 0) {
            cerr << i << ' ' << DS[i].id << ' ' << DS[i].tm << endl;
        }
    }
    fclose(fdata);

    cerr << T.Burst.size () << endl;
    FILE* fcorrect = fopen("../Logistic/data/correct.txt", "w");
    for(int i = 0; i < (int)T.Burst.size(); i++) {
        fprintf(fcorrect, "%u %d %.10f\n", T.Burst[i].id, T.Burst[i].cnt, T.Burst[i].t);
    }
    fclose(fcorrect);
    cerr << T.Burst.size () << endl;

    FILE* fcount = fopen("tmp/burstcount.txt", "w");
    fprintf(fcount, "%d\n", (int)T.Burst.size());
    fclose(fcount);
    cerr << T.Burst.size () << endl;
    FILE* fcountu = fopen("tmp/burstcount_unique.txt", "w");
    set<unsigned> S;
    for(int i = 0; i < (int)T.Burst.size(); i++) {
        S.insert(T.Burst[i].id);
    }
    fprintf(fcountu, "%d\n", (int)S.size());
    fclose(fcountu);

    cerr << T.Burst.size () << endl;

    printf("hash size: %d\n", (int)T.cnt.size() * 4 + (int)T.lsttm.size() * 8 + (int)T.val.size() * 8);
}