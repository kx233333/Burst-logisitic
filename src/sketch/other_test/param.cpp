#include "param.h"
double stdlim = 0.01;
double p = 0.1;
double outv = 0.5;
int threshold = 50;
double chklim = 0.05;

void load_param(struct Param x) {
    chklim = x._chklim;
    outv = x._outv;
    threshold = x._threshold;
    p = x._p;
    stdlim = x._stdlim;
}