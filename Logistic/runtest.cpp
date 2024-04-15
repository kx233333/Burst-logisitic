#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
using namespace std;

void run_and_wait(const char *s) {
    printf("Start running %s\n", s);
    pid_t pid;
    pid = fork();
    assert(pid != -1);
    if(pid == 0) {
        system(s);
        exit(0);
    }
    int status;
    pid_t wt = waitpid(pid, &status, WUNTRACED);
    assert(wt == pid);
    printf("Finish running %s\n", s);
}

double get_res(const char *resfile) {
    FILE *fres = fopen(resfile, "r");
    double res;
    fscanf(fres, "%lf", &res);
    return res;
}

pair <double, int> get_ress (const char *resfile) {
    FILE *fres = fopen(resfile, "r");
    pair <double, int> ress;
    fscanf(fres, "%lf %d", &ress.first, &ress.second);
    return ress;
}

void print_list(double *t, const char *pr, int len, const char *name) {
    printf("%s",name);
    printf(" = [");
    for(int i = 0; i < len; i++) {
        printf(pr, t[i]);
        if(i == len - 1) {
            printf("]\n");
        }
        else printf(", ");
    }
}

void print_list(int *t, int len, const char *name) {
    printf("%s",name);
    printf(" = [");
    for(int i = 0; i < len; i++) {
        printf("%d", t[i]);
        if(i == len - 1) {
            printf("]\n");
        }
        else printf(", ");
    }
}
int main() {
    double all_alpha[7] = {0.1, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int limit_list[3] = {3, 5, 10};
    int columns_list[3] = {3, 5, 10};

    int mem[5];
    double f1[5];
    int count = 0;
    for (int i = 0;i < 7;i++) {
        for (int j = 0; j < 3;j++) {
            for (int k = 0; k < 3; ++k) {
                puts("-------------------------------------------------------");
                printf("Run Test %d\n", ++count);
                char cmdline[1000];
                sprintf(cmdline, "./data_generate %.5lf %d %d", all_alpha[i], limit_list[j], columns_list[k]);
                run_and_wait(cmdline);
                run_and_wait("python3 ../Logistic/solve.py");
            }
        }
    }
    print_list((double*)f1, "%.5lf", 5, "strawman_f1");
    print_list((int*)mem, 5, "strawman_mem");
}