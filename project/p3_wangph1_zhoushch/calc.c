#include "calc.h"

/* set inlets velocity there are two type inlets*/
int set_inlets(const t_param params, float* restrict inlets) {
  const int param_ny = params.ny;
  float param_velocity = params.velocity;

  if(params.type)
  {
    #pragma omp parallel for
    for (int jj = 0; jj < param_ny; ++jj)
    {
      inlets[jj] = param_velocity * (4 * (param_ny - jj) * (jj + 1)) / (param_ny * param_ny); // parabolic
    }
  }
  else
  {
    #pragma omp parallel for
    for (int jj = 0; jj < param_ny; ++jj)
    {
      inlets[jj] = param_velocity; // homogeneous
    }
  }
  // #pragma omp parallel for
  // for (int jj = 0; jj <param_ny; ++jj)
  // {
  //   if(!params.type)
  //     inlets[jj] = param_velocity; // homogeneous
  //   else
  //     inlets[jj] = param_velocity * 4.0 *((1 - ((float)jj) / param_ny) * ((float)(jj + 1)) / param_ny); // parabolic
  // }
  return EXIT_SUCCESS;
}

/* compute average velocity of whole grid, ignore grids with obstacles. */
float av_velocity(const t_param params, t_speed* restrict cells, int* restrict obstacles)
{
  int    tot_cells = 0;  /* no. of cells used in calculation */
  float  tot_u = 0.f;          /* accumulated magnitudes of velocity for each cell */

  int param_nx = params.nx;
  int param_ny = params.ny;

  /* loop over all non-blocked cells */ 

  #pragma omp parallel for reduction(+:tot_u,tot_cells)
  for (int jj = 0; jj < param_ny; ++jj)
  {
    for (int ii = 0; ii < param_nx; ++ii)
    {
      /* ignore occupied cells */
      size_t idx = ii + jj * param_nx;

      if (!obstacles[idx])
      {
        /* local density total */
        float local_density = 0.f;

        // for (int kk = 0; kk < NSPEEDS; ++kk)
        // {
        //   local_density += cells[idx].speeds[kk];
        // }
        local_density = cells[idx].speeds[0] + cells[idx].speeds[1] + cells[idx].speeds[2] + cells[idx].speeds[3] + cells[idx].speeds[4] + cells[idx].speeds[5] + cells[idx].speeds[6] + cells[idx].speeds[7] + cells[idx].speeds[8];

        /* x-component of velocity */
        /*float u_x = (cells[idx].speeds[1]
                   + cells[idx].speeds[5]
                   + cells[idx].speeds[8]
                   - (cells[idx].speeds[3]
                   + cells[idx].speeds[6]
                   + cells[idx].speeds[7]))
                   / local_density;*/
        /* compute y velocity component */
        /*float u_y = (cells[idx].speeds[2]
                   + cells[idx].speeds[5]
                   + cells[idx].speeds[6]
                   - (cells[idx].speeds[4]
                   + cells[idx].speeds[7]
                   + cells[idx].speeds[8]))
                   / local_density;*/

        float u_x = cells[idx].speeds[1];
        float u_y = cells[idx].speeds[2];
        u_x -= cells[idx].speeds[3];
        u_y -= cells[idx].speeds[4];
        u_x += cells[idx].speeds[5];
        u_y += cells[idx].speeds[5];
        u_x -= cells[idx].speeds[6];
        u_y += cells[idx].speeds[6];
        u_x -= cells[idx].speeds[7];
        u_y -= cells[idx].speeds[7];
        u_x += cells[idx].speeds[8];
        u_y -= cells[idx].speeds[8];

        u_x /= local_density;
        u_y /= local_density;


        /* accumulate the norm of x- and y- velocity components */
        tot_u += sqrtf((u_x * u_x) + (u_y * u_y));
        /* increase counter of inspected cells */
        ++tot_cells;
      }
    }
  }

  return tot_u / (float)tot_cells;
}

/* calculate reynold number */
float calc_reynolds(const t_param params, t_speed* restrict cells, int* restrict obstacles)
{
  return av_velocity(params, cells, obstacles) * (float)(params.ny) / params.viscosity;
}
