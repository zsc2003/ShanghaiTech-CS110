#include "types.h"

/* utility functions */
void die(const char* restrict message, const int line, const char* restrict file)
{
  fprintf(stderr, "Error at line %d of file %s:\n", line, file);
  fprintf(stderr, "%s\n", message);
  fflush(stderr);
  exit(EXIT_FAILURE);
}

/* load params, allocate memory, load obstacles & initialise fluid particle densities */
int initialise(const char* restrict paramfile, const char* restrict obstaclefile,
               t_param* restrict  params, t_speed** restrict cells_ptr, t_speed** restrict tmp_cells_ptr,
               int** restrict obstacles_ptr, float** restrict inlets_ptr)
{
  char   message[1024];  /* message buffer */
  FILE*   fp;            /* file pointer */
  int    xx, yy;         /* generic array indices */
  int    blocked;        /* indicates whether a cell is blocked by an obstacle */
  int    retval;         /* to hold return value for checking */

  /* open the parameter file */
  fp = fopen(paramfile, "r");

  if (fp == NULL)
  {
    sprintf(message, "could not open input parameter file: %s", paramfile);
    die(message, __LINE__, __FILE__);
  }

  /* read in the parameter nx */
  retval = fscanf(fp, "nx: %d\n", &(params->nx));
  if (retval != 1) die("could not read param file: nx", __LINE__, __FILE__);

  /* read in the parameter ny */
  retval = fscanf(fp, "ny: %d\n", &(params->ny));
  if (retval != 1) die("could not read param file: ny", __LINE__, __FILE__);

  /* read in the parameter maxIters */
  retval = fscanf(fp, "iters: %d\n", &(params->maxIters));
  if (retval != 1) die("could not read param file: maxIters", __LINE__, __FILE__);

  /* read in the parameter density */
  retval = fscanf(fp, "density: %f\n", &(params->density));
  if (retval != 1) die("could not read param file: density", __LINE__, __FILE__);

  /* read in the parameter viscosity */
  retval = fscanf(fp, "viscosity: %f\n", &(params->viscosity));
  if (retval != 1) die("could not read param file: viscosity", __LINE__, __FILE__);

  /* read in the parameter velocity */
  retval = fscanf(fp, "velocity: %f\n", &(params->velocity));
  if (retval != 1) die("could not read param file: velocity", __LINE__, __FILE__);

  /* read in the parameter type */
  retval = fscanf(fp, "type: %d\n", &(params->type));
  if (retval != 1) die("could not read param file: type", __LINE__, __FILE__);

  /* and close up the file */
  fclose(fp);

  /* calculation of relaxtion parameter */
  params->omega=1./(3.*params->viscosity+0.5);

  /* Check the calculation stability */
  if(params->velocity>0.2)
    printf("Warning: There maybe computational instability due to compressibility.\n");
  if((2-params->omega) < 0.15)
    printf("Warning: Possible divergence of results due to relaxation time.\n");

  /* Allocate memory. */

  /* main grid */
  *cells_ptr = (t_speed*)malloc(sizeof(t_speed) * (params->ny * params->nx));
  if (*cells_ptr == NULL) die("cannot allocate memory for cells", __LINE__, __FILE__);

  /* 'helper' grid, used as scratch space */
  *tmp_cells_ptr = (t_speed*)malloc(sizeof(t_speed) * (params->ny * params->nx));
  if (*tmp_cells_ptr == NULL) die("cannot allocate memory for tmp_cells", __LINE__, __FILE__);

  /* the map of obstacles */
  *obstacles_ptr = malloc(sizeof(int) * (params->ny * params->nx));
  if (*obstacles_ptr == NULL) die("cannot allocate column memory for obstacles", __LINE__, __FILE__);

  /* initialise densities */
  float w0 = params->density * 4.f / 9.f;
  float w1 = params->density      / 9.f;
  float w2 = params->density      / 36.f;
  
  #pragma omp parallel for
  for (int jj = 0; jj < params->ny; ++jj)
  {
    for (int ii = 0; ii < params->nx; ++ii)
    {
      size_t idx = ii + jj * params->nx;
      /* centre */
      (*cells_ptr)[idx].speeds[0] = w0;
      /* axis directions */
      (*cells_ptr)[idx].speeds[1] = w1;
      (*cells_ptr)[idx].speeds[2] = w1;
      (*cells_ptr)[idx].speeds[3] = w1;
      (*cells_ptr)[idx].speeds[4] = w1;
      /* diagonals */
      (*cells_ptr)[idx].speeds[5] = w2;
      (*cells_ptr)[idx].speeds[6] = w2;
      (*cells_ptr)[idx].speeds[7] = w2;
      (*cells_ptr)[idx].speeds[8] = w2;
    }
  }

  /* first set all cells in obstacle array to zero */
  #pragma omp parallel for
  for (int jj = 0; jj < params->ny; ++jj)
  {
    for (int ii = 0; ii < params->nx; ++ii)
    {
      (*obstacles_ptr)[ii + jj * params->nx] = 0;
    }
  }

  /* open the obstacle data file */
  fp = fopen(obstaclefile, "r");

  if (fp == NULL)
  {
    sprintf(message, "could not open input obstacles file: %s", obstaclefile);
    die(message, __LINE__, __FILE__);
  }

  /* read-in the blocked cells list */
  while ((retval = fscanf(fp, "%d %d %d\n", &xx, &yy, &blocked)) != EOF)
  {
    /* Center the obstacle on the y-axis. */
    yy = yy + params->ny / 2;
    
    /* some checks */
    if (retval != 3) die("expected 3 values per line in obstacle file", __LINE__, __FILE__);

    if (xx < 0 || xx > params->nx - 1) die("obstacle x-coord out of range", __LINE__, __FILE__);

    if (yy < 0 || yy > params->ny - 1) die("obstacle y-coord out of range", __LINE__, __FILE__);

    if (blocked != 1) die("obstacle blocked value should be 1", __LINE__, __FILE__);

    /* assign to array */
    (*obstacles_ptr)[xx + yy*params->nx] = blocked;
  }

  /* and close the file */
  fclose(fp);

  /* allocate space to hold the velocity of the cells at the inlet. */
  *inlets_ptr = (float*)malloc(sizeof(float) * params->ny);

  return EXIT_SUCCESS;
}

/* finalise, including freeing up allocated memory */
int finalise(const t_param* restrict params, t_speed** restrict cells_ptr, t_speed** restrict tmp_cells_ptr,
             int** restrict obstacles_ptr, float** restrict inlets)
{
  /*
  ** free up allocated memory
  */
  free(*cells_ptr);
  *cells_ptr = NULL;

  free(*tmp_cells_ptr);
  *tmp_cells_ptr = NULL;

  free(*obstacles_ptr);
  *obstacles_ptr = NULL;

  free(*inlets);
  *inlets = NULL;

  return EXIT_SUCCESS;
}


/* write state of current grid */
int write_state(char* restrict filename, const t_param params, t_speed* restrict cells, int* restrict obstacles)
{
  FILE* fp;                    /* file pointer */
  // float local_density;         /* per grid cell sum of densities */
  // float u_x;                   /* x-component of velocity in grid cell */
  // float u_y;                   /* y-component of velocity in grid cell */
  float u;                     /* norm--root of summed squares--of u_x and u_y */

  fp = fopen(filename, "w");

  if (fp == NULL)
  {
    printf("%s\n",filename);
    die("could not open file output file", __LINE__, __FILE__);
  }

  int param_nx = params.nx;
  int param_ny = params.ny;

  /* loop on grid to calculate the velocity of each cell */
  // #pragma omp parallel for collapse(2) private(local_density,u_x,u_y,u)
  for (int jj = 0; jj < param_ny; ++jj)
  {
    for (int ii = 0; ii < param_nx; ++ii)
    {
      size_t idx = ii + jj * param_nx;
      if (obstacles[idx])
      { /* an obstacle cell */
        u = -0.05f; 
      }
      else
      { /* no obstacle */
        // local_density = 0.f;

        // omp_set_num_threads(NUM_THREADS);
        // #pragma omp simd reduction(+:local_density)
        /*for (register int kk = 0; kk < NSPEEDS; ++kk)
        {
          local_density += cells[idx].speeds[kk];
        }*/
        float local_density = cells[idx].speeds[0] + cells[idx].speeds[1] + cells[idx].speeds[2] + cells[idx].speeds[3] + cells[idx].speeds[4] + cells[idx].speeds[5] + cells[idx].speeds[6] + cells[idx].speeds[7] + cells[idx].speeds[8];
        
        
        /* compute x velocity component */
        /*u_x = (cells[idx].speeds[1]
               + cells[idx].speeds[5]
               + cells[idx].speeds[8]
               - (cells[idx].speeds[3]
                  + cells[idx].speeds[6]
                  + cells[idx].speeds[7]))
              / local_density;*/
        /* compute y velocity component */
        /*u_y = (cells[idx].speeds[2]
               + cells[idx].speeds[5]
               + cells[idx].speeds[6]
               - (cells[idx].speeds[4]
                  + cells[idx].speeds[7]
                  + cells[idx].speeds[8]))
              / local_density;*/
        /* compute norm of velocity */

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

        u = sqrtf((u_x * u_x) + (u_y * u_y));
      }

      /* write to file */
      fprintf(fp, "%d %d %.12E\n", ii, jj, u);
    }
  }
  
  /* close file */
  fclose(fp);

  return EXIT_SUCCESS;
}
