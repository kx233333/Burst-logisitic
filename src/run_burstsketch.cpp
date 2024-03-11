/*
	run_burstsketch.cpp

	运行burstsketch算法，支持传内存参数(KB)，将准确率输出到 tmp/burstsketch.res，将吞吐量输出到 tmp/burstsketch_throughput.res

	usage: ./run_burstsketch mem

	其他可调整参数：两个阶段内存比例（目测已调到最优），burst阈值，位于main函数开头
*/


#include "read_correct.h"
#include "util/read_dataset.h"
#include "sketch/burstsketch/BurstDetector.h"
#include "sketch/newsketch/param.h"
#include<bits/stdc++.h>
using namespace std;

// map<uint64_t, int> D;
// int w = 0;

map<int,pair<double,double> > Windows;

// void print_burst_to_txt(ofstream &ofs, struct Burst b) {
// 	pair<double,double> window_range = Windows[b.end_window];
// 	char s[200];

// 	sprintf(s,"%.10f %.10f %lu", window_range.first, window_range.second, b.flow_id);
// 	int len=strlen(s);
// 	string S;
// 	for(int i=0;i<len;i++) S+=s[i];
// 	ofs<<S<<endl;
// }

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

double compare(vector<Burst> &ours, vector<Burstelem> &ans, FILE *file = NULL, int memory = 0, double mops = 0) {
    map<int, int> Mp;
    set <int> app;
    for (auto &jit : ours) {
        if (Mp.find ((int) jit.flow_id) == Mp.end ())
            Mp [(int) jit.flow_id] = (int) jit.start_window;
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
            sumerr += fabs (Windows [Mp [v.id]].second - v.t);
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
    FILE *file = open_print_header ("/root/burst/netsketch_test/result/main_burstsketch.csv");
	for (double sz = 1.25; sz <= 1000; sz *= 2) {
		int mem = sz; // the size of memory
		double l = 45; // the ratio of the Running Track threshold to the burst threshold
		double r12 = 3.75; // the ratio of the size of Stage 1 to the size of Stage 2
		int screen_layer_threshold = l * threshold; // Running Track threshold
		int log_size = mem * 1024 / (12 * r12 + 20) / bucket_size; // number of buckets in Stage 2
		int screen_layer_size = log_size * r12 * bucket_size; // number of buckets in Stage 1
		int total_memory = screen_layer_size * (sizeof (uint32_t) + sizeof (uint64_t)) + 
		log_size * (3 * sizeof (uint32_t) + sizeof (uint64_t)) * bucket_size;

		int test_cycles = 10;
		timespec time1, time2;
		double mops = 0;
		for (int c = 0; c < test_cycles; ++c) {
			BurstDetector A(screen_layer_size, screen_layer_threshold, log_size, threshold);
			Windows.clear ();
			int window = 0, cnt = 0;
			long long resns = 0;
			for (int i = 0; i < (int) DS.size(); i++) {
				uint64_t id = DS[i].id;
				double tt = DS[i].tm;
				clock_gettime (CLOCK_MONOTONIC, &time1); 
				cnt++;
				if(cnt > 40000)
				{
					cnt = 0;
					window++;
					Windows[window].first=tt;
				}
				Windows[window].second=tt;
				A.insert(id, window);   
				clock_gettime (CLOCK_MONOTONIC, &time2);
				resns += (long long) (time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
			}
			double test_mops = (double) 1000.0 * DS.size () / (double) resns;
			mops += (test_mops - mops) / (c + 1);
			if (c == test_cycles - 1) {
				compare (A.log.Record, Tc, file, total_memory, mops);
			}
		}
	}
    fclose (file);
    return;
}
int main(int argc, char **argv)
{
// 	int mem = 10; // the size of memory
// 	if(argc > 1) {
// 		sscanf(argv[1], "%d", &mem);
// 	}
// 	double l = 0.3; // the ratio of the Running Track threshold to the burst threshold
// 	double r12 = 3.75; // the ratio of the size of Stage 1 to the size of Stage 2
// //#define READ_PARAM

// //#ifdef READ_PARAM
// //		sscanf(argv[1], "%d", &mem);
// //		sscanf(argv[2], "%lf", &l);
// //		sscanf(argv[3], "%lf", &r12);
// //#endif
// 	int screen_layer_threshold = l * threshold; // Running Track threshold
// 	int log_size = mem * 1024 / (12 * r12 + 20) / bucket_size; // number of buckets in Stage 2
// 	int screen_layer_size = log_size * r12 * bucket_size; // number of buckets in Stage 1
// 	BurstDetector A(screen_layer_size, screen_layer_threshold, log_size, threshold);
// 	int window = 0, cnt = 0;
// 	int total_memory = screen_layer_size * (sizeof (uint32_t) + sizeof (uint64_t)) + 
// 	log_size * (3 * sizeof (uint32_t) + sizeof (uint64_t)) * bucket_size;

	int N = 10000000;
	// int N = 200000;
	init_dataset(N);
	// long long sttime = time_ms();
	// for(int i = 0; i < N; i++)
	// {
	// 	uint64_t id = DS[i].id;
	// 	double tt = DS[i].tm;
	// 	cnt++;
    //     if(cnt > 40000)
    //     {
    //     	cnt = 0;
    //     	window++;
	// 		Windows[window].first=tt;
    //     }
	// 	Windows[window].second=tt;
    //     A.insert(id, window);
	// }
	// long long edtime = time_ms();

	// int cntnew=0,cntold=0,cntboth=0;

	read_correct();
	test_res ();
	// map<int,set<double> > S;
	// for(int i = 0; i < (int) Tc.size(); i++) {
    //     S[Tc[i].id].insert(Tc[i].t);
    //     cntnew++;
    // }
	// for(int i=0;i<(int)A.log.Record.size();i++) {
	// 	cntold++;
	// 	struct Burst bst = A.log.Record[i];
	// 	pair<double,double> window_range = Windows[bst.end_window];
	// 	uint32_t id = bst.flow_id;
	// 	if(S[id].lower_bound(window_range.first)==S[id].end()) continue;
    //     int v=*S[id].lower_bound(window_range.first);
    //     if(v<=window_range.second) {
    //         cntboth++;
    //     }
	// }

	// double preci = 1.0 * cntboth / cntnew;
    // double recal = 1.0 * cntboth / cntold;
    // double f1 = 2 * preci * recal / (preci + recal);
	// //printf("%.10f %.10f\n", preci, recal);

	// FILE *f1res = fopen("tmp/burstsketch.res", "w");
    // fprintf(f1res, "%.10f\n", f1);
    // fclose(f1res);

	
	// FILE* fthroughput = fopen("tmp/burstsketch_throughput.res", "w");
    // fprintf(fthroughput, "%lld\n", edtime - sttime);
    // fclose(fthroughput);
    // //printf("%.10f\n", 1.0*A.cache_hit/N);
	// return 0;
}
/*
F1 score
similarity 交集/并集




*/