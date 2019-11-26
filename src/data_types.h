#ifndef data_types_H
#define data_types_H

#include <iostream>

typedef struct{
    int x;
    int y;
} coordinate;

typedef struct{
    float R;
    float G;
    float B;
} rgb_float;

typedef enum{
    svd_m,
    lbp_m
} method;


std::ostream& operator << (std::ostream& os,  coordinate &c);

#endif
