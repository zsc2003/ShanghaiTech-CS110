#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>

#define NSPEEDS         9
#define NUM_BLOCK       4
#define NUM_THREADS     14

typedef struct
{
  int    nx;            /* no. of cells in x-direction */
  int    ny;            /* no. of cells in y-direction */
  int    maxIters;      /* no. of iterations */
  int    type;          /* inlet type */
  float  density;       /* density per cell */
  float  viscosity;     /* kinematic viscosity of fluid */
  float  velocity;      /* inlet velocity */
  float  omega;         /* relaxation parameter */
} t_param;

/* struct to hold the distribution of different speeds */
typedef struct
{
  float speeds[NSPEEDS];
} t_speed;

static const int REVERSE[9] = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
static const int TMP_REVERSE[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

#endif
