#ifndef READ_DATASET_H
#define READ_DATASET_H

#include <cstdint>

struct DS_Pack {
    double tm;
    uint32_t id;
    int pksz;
    double delay;
};

void init_dataset(int);


#endif