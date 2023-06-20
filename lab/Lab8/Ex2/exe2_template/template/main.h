#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <algorithm>
#include <sys/time.h>
#include <time.h>
#include <immintrin.h>
//inplement dymanic



#define PI 3.14159

typedef struct FVec
{
    unsigned int length;
    unsigned int min_length;
    unsigned int min_deta;
    float* data;
    float* sum;
} FVec;

typedef struct Image
{
    unsigned int dimX, dimY, numChannels;
    float* data;
} Image;

Image gb_h(Image a, FVec gv);
Image gb_v(Image a, FVec gv);
Image img_sc(Image a);
Image apply_gb(Image a, FVec gv);

#endif