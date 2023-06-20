#ifndef CALC_H
#define CALC_H

#include "types.h"

/* This file contains some calculation functions that are not the main workload, and you can completely unoptimize them */

/* set inlets velocity */
int set_inlets(const t_param params, float* inlets);

/* compute average velocity */
float av_velocity(const t_param params, t_speed* cells, int* obstacles);

/* calculate reynold number */
float calc_reynolds(const t_param params, t_speed* cells, int* obstacles);

#endif
