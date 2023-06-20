#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* This file contains utility functions for initialise, finalise, and outputting the corresponding files */
void die(const char* message, const int line, const char* file);

/* Load parameters, allocate memory, initialize grids and obstacle. */
int initialise(const char* paramfile, const char* obstaclefile,
               t_param* params, t_speed** cells_ptr, t_speed** tmp_cells_ptr,
               int** obstacles_ptr, float** inlets_ptr);

/* Finalise, including freeing up allocated memory */
int finalise(const t_param* params, t_speed** cells_ptr, t_speed** tmp_cells_ptr,
             int** obstacles_ptr, float** inlets);

/* Output current grid state -- each cells' velocity. */
int write_state(char* filename, const t_param params, t_speed* cells, int* obstacles);

#endif
