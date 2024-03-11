/*
	run_topicsketch.cpp

	运行topicsketch，支持传内存参数（KB），将准确率输出到 tmp/topicsketch.res，将吞吐量输出到 tmp/topicsketch_throughput.res
	usage: ./run_topicsketch mem
*/
#include "read_correct.h"
#include "util/read_dataset.h"
#include "sketch/topicsketch/TopicSketch.h"
#include "sketch/newsketch/param.h"

#include<bits/stdc++.h>
using namespace std;
// map<uint64_t, int> D;
// vector<pair<int, int> > S[2000000];
// map<uint64_t, int> R[100000];
// int w = 0;
//int nxtg;

// double _tm[10001000];
// int _window[10001000];

extern vector<Burstelem> Tc;
extern vector<DS_Pack> DS;

#include <sys/time.h>

// long long time_ms() {
//     struct timeval tv;
//     long long mstime;
//     gettimeofday(&tv, NULL);
//     mstime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
//     return mstime;
// }


double compare(vector<Burstelem> &ours, vector<Burstelem> &ans, FILE *file = NULL, int memory = 0, double mops = 0) {
    map<int, double> Mp;
    set <int> app;
    for (auto &jit : ours) {
        if (Mp.find (jit.id) == Mp.end ())
            Mp [jit.id] = jit.t;
    }
    double allt = DS[DS.size() - 1].tm;
    //printf("%.10f\n", allt);
    double sumerr = 0;
    int TP = 0, FP = 0, FN = 0;
    for (auto &v : ans) {
        if (app.find (v.id) != app.end ()) continue;
        app.insert (v.id);
        if (Mp.find (v.id) == Mp.end ()) {
            ++FN;
        }
        else {
            ++TP;
            sumerr += fabs (v.t - Mp [v.id]);
        }
    }
    FP = Mp.size () - TP;
    sumerr += (FN + FP) * allt;
    double R = (double) TP / (TP + FN), P = (double) TP / (TP + FP), F1 = 2 * R * P / (R + P);
    printf ("TP: %d, FP: %d, FN: %d\n", TP, FP, FN);
    printf ("Recall: %.6lf%%, Precision: %.6lf%%, F1 Score: %.6lf%%\n", R * 100, P * 100, F1 * 100);
    if (file != NULL) {
        fprintf (file, "%d,%d,%d,%d,%d,%.6lf,%.6lf,%.6lf,%.6lf,%.6lf\n",
            memory, (int) Mp.size (), TP, FP, FN, F1 * 100, P * 100, R * 100, sumerr / (TP + FN + FP), mops);
    }
    return sumerr / TP / allt;
}

FILE *open_print_header (const char *name) {
    FILE *file = fopen (name, "w");
    assert (file != NULL);
    fprintf (file, "Memory,Report_Count,TP,FP,FN,F1_Score,Precision,Recall,Delta,MOPS\n");
    return file;
}

void test_res () {
    FILE *file = open_print_header ("/root/burst/netsketch_test/result/main_topicsketch.csv");
	for (double sz = 1; sz <= 100; sz *= 2) {
		int mem = sz; // the size of memory
		int test_cycles = 10;
		timespec time1, time2;
		double mops = 0;
		for (int c = 0; c < test_cycles; ++c) {
			vector <Burstelem> burst;
			TopicSketch A (8, mem * 1024 / 16 / 8, 20000, 10000);
			int window = 0, cnt = 0;
			long long resns = 0;
			for (int i = 0; i < (int) DS.size(); i++) {
				int id = DS[i].id; // caida
				double tm = DS[i].tm;
				clock_gettime (CLOCK_MONOTONIC, &time1); 
				cnt++;
				if(cnt > 250)
				{
					cnt = 0;
					window++;
				}
				A.insert(id, i);
				if (A.query(id, i) >= 2e-7) {
					burst.push_back((Burstelem){(uint32_t) id, 1, tm});
				}
				clock_gettime (CLOCK_MONOTONIC, &time2);
				resns += (long long) (time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
			}
			double test_mops = (double) 1000.0 * DS.size () / (double) resns;
			mops += (test_mops - mops) / (c + 1);
			if (c == test_cycles - 1) {
				compare (burst, Tc, file, mem * 1024, mops);
			}
		}
	}
    fclose (file);
    return;
}

int main(int argc, char **argv)
{
	//int threshold = 50;
	// int mem = 50;
	// int mem;
	// sscanf(argv[1],"%d",&mem);
	// TopicSketch A(8, mem * 1024 / 16 / 8, 20000, 10000);
	//CorrectBurstDetector B(threshold);
	//nxtg = 40000 + (rand()%10 - 3) * 5000;
	// int window = 0, cnt = 0, total = 0;
	const int N = 10000000;
	// const int N = 200000;
    init_dataset(N);

	// long long sttime = time_ms();
	// for(int i = 0; i < N; i++)
	// { 
	// 	uint64_t id = DS[i].id; // caida
	// 	_tm[i] = DS[i].tm;
	// 	cnt++;
    //     if(cnt > 250)
    //     //if(cnt > nxtg) 
	// 	{
    //     	cnt = 0;
    //     	//nxtg = 40000 + (rand()%10 - 3) * 5000;
    //     	window++;
    //     }
    //     A.insert(id, i);
    //     if(A.query(id, i) >= 0.00000035)
	// 	{
	// 		if(R[i/10000][id] == 0)
	// 			total++;
	// 		R[i/10000][id] = 1;
	// 		//cout<<i<<" "<<id<<" "<<A.query(id, i)<<endl;
    // 	}
	// 	//B.insert(id, window);
	// }
	// long long edtime = time_ms();

	read_correct();

	test_res ();

	// double end_time;
	// uint64_t flow_id;
    // int correct = 0, total2 = 0;
	// for(int i = 0; i < (int)Tc.size(); i++) {
	// 	end_time = Tc[i].t;
	// 	flow_id = Tc[i].id;
	// 	//printf("%.10f\n",end_time);
	// 	int pos=lower_bound(_tm,_tm+N,end_time)-_tm;
	// 	//if(pos>200000) continue;
	// 	total2++;
	// 	if(R[pos/10000].count(flow_id) && R[pos/10000][flow_id] == 1) {
	// 		R[pos/10000][flow_id] = 0;
	// 		correct++;
	// 	}
	// }
	// cerr<<total<<' '<<total2<<' '<<correct<<endl;
	// FILE *fres = fopen("./tmp/topicsketch.res", "w");

	// double pr = 1.0 * correct / total;
	// double rr = 1.0 * correct / total2;
	// double f1 = 2 * pr * rr / (pr + rr);
	// fprintf(fres, "%.5lf\n",f1);

	// FILE* fthroughput = fopen("tmp/topicsketch_throughput.res", "w");
    // fprintf(fthroughput, "%lld\n", edtime - sttime);
    // fclose(fthroughput);
	return 0;
}