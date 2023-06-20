#include "d2q9_bgk.h"
#include <immintrin.h> // AVX and other SIMD

#define LOOP_BLOCK_SIZE 8

/* The main processes in one step */
int collision(const int param_nx, const int param_ny, const float param_omega, t_speed* restrict cells, t_speed* restrict tmp_cells, int* restrict obstacles);
int streaming(const int param_nx, const int param_ny, t_speed* restrict cells, t_speed* restrict tmp_cells);
int obstacle(const int param_nx, const int param_ny, t_speed* restrict cells, t_speed* restrict tmp_cells, int* restrict obstacles);
int boundary(const int param_nx, const int param_ny, t_speed* restrict cells, t_speed* restrict tmp_cells, float* restrict inlets);

/*
** The main calculation methods.
** timestep calls, in order, the functions:
** collision(), obstacle(), streaming() & boundary()
*/
int timestep(const t_param params, t_speed* restrict cells, t_speed* restrict tmp_cells, float* restrict inlets, int* restrict obstacles)
{
  /* The main time overhead, you should mainly optimize these processes. */
  collision(params.nx, params.ny, params.omega, cells, tmp_cells, obstacles);
  // obstacle(params.nx, params.ny, cells, tmp_cells, obstacles);
  streaming(params.nx, params.ny, cells, tmp_cells);
  boundary(params.nx, params.ny, cells, tmp_cells, inlets);
  return EXIT_SUCCESS;
}

/*
** The collision of fluids in the cell is calculated using 
** the local equilibrium distribution and relaxation process
*/
int collision(const int param_nx, const int param_ny, const float param_omega, t_speed* restrict cells, t_speed* restrict tmp_cells, int* restrict obstacles) {
  const float c_sq = 1.f / 3.f; /* square of speed of sound */
  const float w0 = 4.f / 9.f;   /* weighting factor */
  const float w1 = 1.f / 9.f;   /* weighting factor */
  const float w2 = 1.f / 36.f;  /* weighting factor */

  /* loop over the cells in the grid
  ** the collision step is called before
  ** the streaming step and so values of interest
  ** are in the scratch-space grid */
  #pragma omp parallel for
  for (int jj = 0; jj < param_ny; ++jj)
  {
    for (int ii = 0; ii < param_nx; ++ii)
    {
      size_t idx = ii + jj * param_nx;
      if (!obstacles[idx])
      {
        /* compute local density total */
        float local_density = 0.f;

        // TODO: copy cells[ii + jj * param_nx].speeds

        // #pragma omp simd reduction(+:local_density)
        // NSPEEDS = 9
        // float speeds[9] = {cells[idx].speeds[0],
        //                   cells[idx].speeds[1],
        //                   cells[idx].speeds[2],
        //                   cells[idx].speeds[3],
        //                   cells[idx].speeds[4],
        //                   cells[idx].speeds[5],
        //                   cells[idx].speeds[6],
        //                   cells[idx].speeds[7],
        //                   cells[idx].speeds[8]};

        /* 
        #pragma omp simd reduction(+:local_density)
        for (int kk = 0; kk < NSPEEDS; ++kk)
        {
          local_density += cells[idx].speeds[kk];
        }

        // compute x velocity component 
        float u_x = (cells[idx].speeds[1]
                   + cells[idx].speeds[5]
                   + cells[idx].speeds[8]
                   -(cells[idx].speeds[3]
                   + cells[idx].speeds[6]
                   + cells[idx].speeds[7]))
                     / local_density;
        // compute y velocity component 
        float u_y = (cells[idx].speeds[2]
                   + cells[idx].speeds[5]
                   + cells[idx].speeds[6]
                   - (cells[idx].speeds[4]
                   + cells[idx].speeds[7]
                   + cells[idx].speeds[8]))
                     / local_density;
        */
        // __m128 A = _mm_loadu_ps(cells[idx].speeds);
        // sum = A[0] + A[1] + A[2] +A[3]


        // float sum = _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(A, A), A));

        // local_density = speeds[0] + speeds[1] + speeds[2] + speeds[3] + speeds[4] + speeds[5] + speeds[6] + speeds[7] + speeds[8];
        local_density = cells[idx].speeds[0] + cells[idx].speeds[1] + cells[idx].speeds[2] + cells[idx].speeds[3] + cells[idx].speeds[4] + cells[idx].speeds[5] + cells[idx].speeds[6] + cells[idx].speeds[7] + cells[idx].speeds[8];

        // compute x velocity component 
        /* float u_x = (speeds[1]
                   + speeds[5]
                   + speeds[8]
                  - (speeds[3]
                   + speeds[6]
                   + speeds[7]))
                  / local_density;
        // compute y velocity component 
        float u_y = (speeds[2]
                   + speeds[5]
                   + speeds[6]
                  - (speeds[4]
                   + speeds[7]
                   + speeds[8]))
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

        /* velocity squared */
        float u_sq = u_x * u_x + u_y * u_y;

        /* directional velocity components */
        // float u[NSPEEDS];
        // u[0] = 0;            /* zero */
        // u[1] =   u_x;        /* east */
        // u[2] =         u_y;  /* north */
        // u[3] = - u_x;        /* west */
        // u[4] =       - u_y;  /* south */
        // u[5] =   u_x + u_y;  /* north-east */
        // u[6] = - u_x + u_y;  /* north-west */
        // u[7] = - u_x - u_y;  /* south-west */
        // u[8] =   u_x - u_y;  /* south-east */

        __m256 tmp_x = _mm256_setr_ps(u_x, 0, -u_x, 0, u_x, -u_x, -u_x, u_x);
        __m256 tmp_y = _mm256_setr_ps(0, u_y, 0, -u_y, u_y, u_y, -u_y, -u_y);
        __m256 tmp_u = _mm256_add_ps(tmp_x, tmp_y);
        // _mm256_storeu_ps(u + 1, tmp_u);


        /* equilibrium densities */
        // float d_equ[NSPEEDS];
        /* zero velocity density: weight w0 */

        // d_equ[0] = w0 * local_density * (1.f + u[0] / c_sq
        //                                  + (u[0] * u[0]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // /* axis speeds: weight w1 */
        // d_equ[1] = w1 * local_density * (1.f + u[1] / c_sq
        //                                  + (u[1] * u[1]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[2] = w1 * local_density * (1.f + u[2] / c_sq
        //                                  + (u[2] * u[2]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[3] = w1 * local_density * (1.f + u[3] / c_sq
        //                                  + (u[3] * u[3]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[4] = w1 * local_density * (1.f + u[4] / c_sq
        //                                  + (u[4] * u[4]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // /* diagonal speeds: weight w2 */
        // d_equ[5] = w2 * local_density * (1.f + u[5] / c_sq
        //                                  + (u[5] * u[5]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[6] = w2 * local_density * (1.f + u[6] / c_sq
        //                                  + (u[6] * u[6]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[7] = w2 * local_density * (1.f + u[7] / c_sq
        //                                  + (u[7] * u[7]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));
        // d_equ[8] = w2 * local_density * (1.f + u[8] / c_sq
        //                                  + (u[8] * u[8]) / (2.f * c_sq * c_sq)
        //                                  - u_sq / (2.f * c_sq));

        // __m256 w0_vec = _mm256_set1_ps(w0 * local_density);
        // __m256 w1_vec = _mm256_set1_ps(w1 * local_density);
        // __m256 w2_vec = _mm256_set1_ps(w2 * local_density);
        // __m256 onef_vec = _mm256_set1_ps(1.f);



       /* __m256 double_c_sq_sq_vec = _mm256_set1_ps(2.f * c_sq * c_sq);
        __m256 u_sq_d_c_sq_vec = _mm256_set1_ps(1.f - u_sq / (2.f * c_sq));
        __m256 c_sq_vec = _mm256_set1_ps(c_sq);*/

        // __m256 u_vec = _mm256_loadu_ps(&u[1]);
        __m256 a0 = _mm256_set1_ps(1.f - u_sq / (2.f * c_sq));
        __m256 a1 = _mm256_set1_ps(1.f / c_sq);
        __m256 a2 = _mm256_set1_ps(0.5f / c_sq / c_sq);
        __m256 res = _mm256_mul_ps(a2, tmp_u);
        res = _mm256_add_ps(a1, res);
        res = _mm256_mul_ps(res, tmp_u);
        res = _mm256_add_ps(res, a0);

        __m256 w_vec = _mm256_set_ps(w2, w2, w2, w2, w1, w1, w1, w1);
        res = _mm256_mul_ps(res, w_vec);
        __m256 tmp_local_d = _mm256_set1_ps(local_density);
        res = _mm256_mul_ps(res, tmp_local_d);

       /* __m256 u_div_c_sq_vec = _mm256_div_ps(u_vec, c_sq_vec);
        __m256 square_u_vec = _mm256_mul_ps(u_vec, u_vec);
        __m256 tmp_dequ = _mm256_add_ps(u_sq_d_c_sq_vec, _mm256_add_ps(u_div_c_sq_vec, _mm256_div_ps(square_u_vec, double_c_sq_sq_vec)));

        __m256 d_equ_res_vec = _mm256_mul_ps(_mm256_mul_ps(w_vec, tmp_local_d), tmp_dequ);*/

        
        /* relaxation step */
        // #pragma omp simd
        // for (int kk = 0; kk < NSPEEDS; ++kk)
        // {
        //   tmp_cells[ii + jj*param_nx].speeds[kk] = speeds[kk]
        //                                           + param_omega
        //                                           * (d_equ[kk] - speeds[kk]);
        // }

        float d_equ_0 = w0 * local_density * (1.f 
                                        // + u_0 / c_sq
                                        //  + (u_0 * u_0) / (2.f * c_sq * c_sq)
                                         - u_sq / (2.f * c_sq));
        tmp_cells[idx].speeds[0] = cells[idx].speeds[0] + param_omega * (d_equ_0 - cells[idx].speeds[0]);

        __m256 tmp_speeds = _mm256_loadu_ps(cells[idx].speeds + 1);
        __m256 d_sub_s = _mm256_sub_ps(res, tmp_speeds);
        // __m256 tmp_d_equ = _mm256_loadu_ps(d_equ + 1);
        // __m256 d_sub_s = _mm256_sub_ps(tmp_d_equ, tmp_speeds);

        __m256 tmp_omega = _mm256_set1_ps(param_omega);
        __m256 tmp_omega_d_sub_s = _mm256_mul_ps(tmp_omega, d_sub_s);
        __m256 tmp_speeds_new = _mm256_add_ps(tmp_speeds, tmp_omega_d_sub_s);
        _mm256_storeu_ps(tmp_cells[idx].speeds + 1, tmp_speeds_new);
      }
      else
      {
        __m256 tmp_speeds = _mm256_setr_ps(cells[idx].speeds[3], cells[idx].speeds[4], cells[idx].speeds[1], cells[idx].speeds[2], cells[idx].speeds[7], cells[idx].speeds[8], cells[idx].speeds[5], cells[idx].speeds[6]);
        // __m256 tmp_speeds = _mm256_loadu_ps(cells[idx].speeds + 1);
        // tmp_speeds = _mm256_permutevar8x32_ps(tmp_speeds, _mm256_setr_epi32(2, 3, 0, 1, 6, 7, 4, 5));
        _mm256_storeu_ps(tmp_cells[idx].speeds + 1, tmp_speeds);

        // deal with 0
        tmp_cells[idx].speeds[0] = cells[idx].speeds[0];
      }
      // // single stream
      // int y_n = (jj + 1) & (param_ny - 1);
      // int x_e = (ii + 1) & (param_nx - 1);
      // int y_s = (jj == 0) ? (param_ny - 1) : (jj - 1);
      // int x_w = (ii == 0) ? (param_nx - 1) : (ii - 1);
      // __m256 tmp_speeds = _mm256_setr_ps(tmp_cells[x_w + jj  * param_nx].speeds[1], tmp_cells[ii  + y_s * param_nx].speeds[2], tmp_cells[x_e + jj  * param_nx].speeds[3], tmp_cells[ii  + y_n * param_nx].speeds[4], tmp_cells[x_w + y_s * param_nx].speeds[5], tmp_cells[x_e + y_s * param_nx].speeds[6], tmp_cells[x_e + y_n * param_nx].speeds[7], tmp_cells[x_w + y_n * param_nx].speeds[8]);
      // _mm256_storeu_ps(cells[idx].speeds + 1, tmp_speeds);
      // // deal with 0
      // cells[idx].speeds[0] = tmp_cells[idx].speeds[0];
    }
  }
  return EXIT_SUCCESS;
}

/*
** For obstacles, mirror their speed.
*/
int obstacle(const int param_nx, const int param_ny, t_speed *restrict cells, t_speed *restrict tmp_cells, int *restrict obstacles)
{
  /* loop over the cells in the grid */
  #pragma omp parallel for schedule(static)
  for (int jj = 0; jj < param_ny; ++jj)
  {
    for (int ii = 0; ii < param_nx; ++ii)
    {
      size_t idx = ii + jj * param_nx;
      if (obstacles[idx])
      {

        /* if the cell contains an obstacle */
        /* called after collision, so taking values from scratch space
        ** mirroring, and writing into main grid */

//         #pragma omp simd
//         for (int kk = 0; kk < NSPEEDS; ++kk)
//         {
//           tmp_cells[idx].speeds[REVERSE[kk]] = cells[idx].speeds[TMP_REVERSE[kk]];
//         }
        // order is 0, 3, 4, 1, 2, 7, 8, 5, 6, do not load 0
        // static const int REVERSE[9] = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
        __m256 tmp_speeds = _mm256_setr_ps(cells[idx].speeds[3], cells[idx].speeds[4], cells[idx].speeds[1], cells[idx].speeds[2], cells[idx].speeds[7], cells[idx].speeds[8], cells[idx].speeds[5], cells[idx].speeds[6]);
        _mm256_storeu_ps(tmp_cells[idx].speeds + 1, tmp_speeds);

        // deal with 0
        tmp_cells[idx].speeds[0] = cells[idx].speeds[0];
        // __m256 tmp_speed = _mm256_set_ps(cells[idx].speeds[8], cells[idx].speeds[7], cells[idx].speeds[6], cells[idx].speeds[5], cells[idx].speeds[4], cells[idx].speeds[3], cells[idx].speeds[2], cells[idx].speeds[1]);
      }
    }
  }
  return EXIT_SUCCESS;
}

/*
** Particles flow to the corresponding cell according to their speed direaction.
*/
int streaming(const int param_nx, const int param_ny, t_speed* restrict cells, t_speed* restrict tmp_cells) {

  /* loop over _all_ cells */

  int mody = param_ny - 1;
  int modx = param_nx - 1;


  // int chunk_size = 64;
 
  // #pragma omp parallel for
  // for (int j = 0; j < param_ny; j += chunk_size)
  // {
  //   for (int i = 0; i < param_nx; i += chunk_size)
  //   {
  // for(int chunk_i = 0; chunk_i < chunk_size; ++chunk_i)
  // {
  //   for(int chunk_j = 0; chunk_j < chunk_size; ++chunk_j)
  //   {
  //     int ii = i + chunk_i;
  //     int jj = j + chunk_j;
  #pragma omp parallel for
  for (int jj = 0; jj < param_ny; ++jj)
  {
    for (int ii = 0; ii < param_nx; ++ii)
    {
      size_t idx = ii + jj * param_nx;
      /* determine indices of axis-direction neighbours
      ** respecting periodic boundary conditions (wrap around) */
      // int y_n = (jj + 1) % param_ny;
      // int x_e = (ii + 1) % param_nx;

      int y_n = (jj + 1) & mody;
      int x_e = (ii + 1) & modx;
      int y_s = (jj == 0) ? (param_ny - 1) : (jj - 1);
      int x_w = (ii == 0) ? (param_nx - 1) : (ii - 1);
      /* propagate densities from neighbouring cells, following
      ** appropriate directions of travel and writing into
      ** scratch space grid */
      // cells[ii  + jj  * param_nx].speeds[0] = tmp_cells[idx].speeds[0]; /* central cell, no movement */
      // cells[x_e + jj  * param_nx].speeds[1] = tmp_cells[idx].speeds[1]; /* east */
      // cells[ii  + y_n * param_nx].speeds[2] = tmp_cells[idx].speeds[2]; /* north */
      // cells[x_w + jj  * param_nx].speeds[3] = tmp_cells[idx].speeds[3]; /* west */
      // cells[ii  + y_s * param_nx].speeds[4] = tmp_cells[idx].speeds[4]; /* south */
      // cells[x_e + y_n * param_nx].speeds[5] = tmp_cells[idx].speeds[5]; /* north-east */
      // cells[x_w + y_n * param_nx].speeds[6] = tmp_cells[idx].speeds[6]; /* north-west */
      // cells[x_w + y_s * param_nx].speeds[7] = tmp_cells[idx].speeds[7]; /* south-west */
      // cells[x_e + y_s * param_nx].speeds[8] = tmp_cells[idx].speeds[8]; /* south-east */

      // cells[idx].speeds[0] = tmp_cells[idx].speeds[0]; /* central cell, no movement */
      // cells[idx].speeds[1] = tmp_cells[x_w + jj  * param_nx].speeds[1]; /* east */
      // cells[idx].speeds[2] = tmp_cells[ii  + y_s * param_nx].speeds[2]; /* north */
      // cells[idx].speeds[3] = tmp_cells[x_e + jj  * param_nx].speeds[3]; /* west */
      // cells[idx].speeds[4] = tmp_cells[ii  + y_n * param_nx].speeds[4]; /* south */
      // cells[idx].speeds[5] = tmp_cells[x_w + y_s * param_nx].speeds[5]; /* north-east */
      // cells[idx].speeds[6] = tmp_cells[x_e + y_s * param_nx].speeds[6]; /* north-west */
      // cells[idx].speeds[7] = tmp_cells[x_e + y_n * param_nx].speeds[7]; /* south-west */
      // cells[idx].speeds[8] = tmp_cells[x_w + y_n * param_nx].speeds[8]; /* south-east */
      __m256 tmp_speeds = _mm256_setr_ps(tmp_cells[x_w + jj  * param_nx].speeds[1], tmp_cells[ii  + y_s * param_nx].speeds[2], tmp_cells[x_e + jj  * param_nx].speeds[3], tmp_cells[ii  + y_n * param_nx].speeds[4], tmp_cells[x_w + y_s * param_nx].speeds[5], tmp_cells[x_e + y_s * param_nx].speeds[6], tmp_cells[x_e + y_n * param_nx].speeds[7], tmp_cells[x_w + y_n * param_nx].speeds[8]);
      _mm256_storeu_ps(cells[idx].speeds + 1, tmp_speeds);

      // deal with 0
      cells[idx].speeds[0] = tmp_cells[idx].speeds[0];
    }
  }
    // }
  // }
  return EXIT_SUCCESS;
}

/*
** Work with boundary conditions. The upper and lower boundaries use the rebound plane, 
** the left border is the inlet of fixed speed, and 
** the right border is the open outlet of the first-order approximation.
*/
int boundary(const int param_nx, const int param_ny, t_speed* restrict cells,  t_speed* restrict tmp_cells, float* restrict inlets) {

  /* Set the constant coefficient */
  const float cst1 = 2.0 / 3.0;
  const float cst2 = 1.0 / 6.0;
  const float cst3 = 1.0 / 2.0;

  int ii, jj;
  float local_density;
  
  // top wall (bounce)
  jj = param_ny -1;
  #pragma omp parallel for
  for(ii = 0; ii < param_nx; ++ii){
    size_t idx = ii + jj * param_nx;
    cells[idx].speeds[4] = tmp_cells[idx].speeds[2];
    cells[idx].speeds[7] = tmp_cells[idx].speeds[5];
    cells[idx].speeds[8] = tmp_cells[idx].speeds[6];
  }

  // bottom wall (bounce)
  jj = 0;
  #pragma omp parallel for
  for(ii = 0; ii < param_nx; ++ii){
    size_t idx = ii + jj * param_nx;
    cells[idx].speeds[2] = tmp_cells[idx].speeds[4];
    cells[idx].speeds[5] = tmp_cells[idx].speeds[7];
    cells[idx].speeds[6] = tmp_cells[idx].speeds[8];
  }

  // left wall (inlet)
  ii = 0;
  #pragma omp parallel for
  for(jj = 0; jj < param_ny; ++jj)
  {
    size_t idx = ii + jj * param_nx;
    local_density = (   cells[idx].speeds[0]
                      + cells[idx].speeds[2]
                      + cells[idx].speeds[4]
                      + 2.0 * cells[idx].speeds[3]
                      + 2.0 * cells[idx].speeds[6]
                      + 2.0 * cells[idx].speeds[7]
                      )/(1.0 - inlets[jj]);

    float prefetch1 = cst3 * (cells[idx].speeds[2]-cells[idx].speeds[4]);
    float prefetch2 = cst2 * local_density * inlets[jj];
    cells[idx].speeds[1] = cells[idx].speeds[3]
                                        + cst1*local_density * inlets[jj];

    cells[idx].speeds[5] = cells[idx].speeds[7]
                                        - prefetch1
                                        + prefetch2;

    cells[idx].speeds[8] = cells[idx].speeds[6]
                                        + prefetch1
                                        + prefetch2;
  
  }

  // right wall (outlet)
  ii = param_nx - 1;
  // int kk = 0;
  // omp_set_num_threads(NUM_THREADS);
  // #pragma omp parallel for collapse(2)
  // for(jj = 0; jj < param_ny; ++jj)
  // {
  //   for (kk = 0; kk < NSPEEDS; kk++)
  //   {
  //     cells[ii + jj * param_nx].speeds[kk] = cells[ii - 1 + jj * param_nx].speeds[kk];
  //   }
  // }
  #pragma omp parallel for
  for(jj = 0; jj < param_ny; ++jj)
  {
    // cells[ii + jj * param_nx].speeds[0] = cells[ii - 1 + jj * param_nx].speeds[0];
    // cells[ii + jj * param_nx].speeds[1] = cells[ii - 1 + jj * param_nx].speeds[1];
    // cells[ii + jj * param_nx].speeds[2] = cells[ii - 1 + jj * param_nx].speeds[2];
    // cells[ii + jj * param_nx].speeds[3] = cells[ii - 1 + jj * param_nx].speeds[3];
    // cells[ii + jj * param_nx].speeds[4] = cells[ii - 1 + jj * param_nx].speeds[4];
    // cells[ii + jj * param_nx].speeds[5] = cells[ii - 1 + jj * param_nx].speeds[5];
    // cells[ii + jj * param_nx].speeds[6] = cells[ii - 1 + jj * param_nx].speeds[6];
    // cells[ii + jj * param_nx].speeds[7] = cells[ii - 1 + jj * param_nx].speeds[7];
    // cells[ii + jj * param_nx].speeds[8] = cells[ii - 1 + jj * param_nx].speeds[8];
    size_t idx = ii + jj * param_nx;
    cells[idx].speeds[0] = cells[idx - 1].speeds[0];
    __m256 tmp_speeds = _mm256_loadu_ps(cells[idx - 1].speeds + 1);
    _mm256_storeu_ps(cells[idx].speeds + 1, tmp_speeds);
  }
  return EXIT_SUCCESS;
}
