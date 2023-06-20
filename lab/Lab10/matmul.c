#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MATRIX_SIZE 1024

void matmul_naive (double *a, double *b, double *c)
{
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      for (int k = 0; k < MATRIX_SIZE; k++) {
        c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
      }
    }
  }
}

void matmul_optimized_slices (double *a, double *b, double *c)
{
  #pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    for (int i = 0; i < MATRIX_SIZE; i += num_threads)
    {
      for (int j = 0; j < MATRIX_SIZE; j++)
      {
        for (int k = 0; k < MATRIX_SIZE; k++)
        {
          c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }
  }
}

void matmul_optimized_chunks (double *a, double *b, double *c)
{
  int num_threads = 1;
  int thread_ID = 0;
  int chunk_size = MATRIX_SIZE / num_threads;
  int remain = MATRIX_SIZE % num_threads;
  int start = 0;
  int end = chunk_size;

  #pragma omp parallel private(thread_ID), shared(num_threads), shared(chunk_size), private(start), private(end)
  {
    num_threads = omp_get_num_threads();
    // printf("num_threads = %d\n", num_threads);
    thread_ID = omp_get_thread_num();
    chunk_size = MATRIX_SIZE / num_threads;

    start = thread_ID * chunk_size;
    end = start + chunk_size;
    
    double* buffer = (double *) calloc(chunk_size * MATRIX_SIZE, sizeof(double));
    
    for (int i = start; i < end; ++i)
    {
      for (int j = 0; j < MATRIX_SIZE; ++j)
      {
        for (int k = 0; k < MATRIX_SIZE; ++k)
        {
          buffer[(i - start) * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }

    for (int i = start; i < end; ++i)
    {
      for (int j = 0; j < MATRIX_SIZE; ++j)
      {
        c[i * MATRIX_SIZE + j] = buffer[(i - start) * MATRIX_SIZE + j];
      }
    }

    if (remain != 0)
    {
      int start = num_threads * chunk_size;
      int end = MATRIX_SIZE;
      for (int i = start; i < end; ++i)
      {
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
          for (int k = 0; k < MATRIX_SIZE; ++k)
          {
            c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
          }
        }
      }
    }
  }

}

// void matmul_optimized_chunks (double *a, double *b, double *c)
// {
//   #pragma omp parallel
//   {
//     int total_threads = omp_get_num_threads();
//     int curr_thread = omp_get_thread_num();

//     int chunk_size = MATRIX_SIZE / total_threads;
//     int rest_num = MATRIX_SIZE - chunk_size * total_threads;
//     for (int i = curr_thread*chunk_size; i < ((curr_thread+1)*chunk_size); i++) {
//       for (int j = 0; j < MATRIX_SIZE; j++) {
//         for (int k = 0; k < MATRIX_SIZE; k++) {
//           c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
//         }
//       }
//     }


//     if(rest_num != 0) {
// 			for (int i = MATRIX_SIZE-rest_num; i < MATRIX_SIZE; i++) {
//         for (int j = 0; j < MATRIX_SIZE; j++) {
//           for (int k = 0; k < MATRIX_SIZE; k++) {
//             c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
//           }
//         }
//       }
// 		}
//   }
// }

int main()
{

for (int i = 1; i <= 8; ++i)
{
omp_set_num_threads(i);
printf("-----------------------------------------------\n");
printf("the number of threads is %d\n", i);

  double *a = (double *) malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *b = (double *) malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *c = (double *) calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));

  if (!a || !b || !c) {
    printf("Error: could not allocate memory.\n");
    return 1;
  }

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      a[i * MATRIX_SIZE + j] = (double) rand() / RAND_MAX;
      b[i * MATRIX_SIZE + j] = (double) rand() / RAND_MAX;
    }
  }

  double start_time = omp_get_wtime();
  matmul_naive(a, b, c);
  double end_time = omp_get_wtime();
  printf("Naive: %.5f seconds\n", end_time - start_time);

  start_time = omp_get_wtime();
  matmul_optimized_slices(a, b, c);
  end_time = omp_get_wtime();
  printf("Optimized (slices): %.5f seconds\n", end_time - start_time);

  start_time = omp_get_wtime();
matmul_optimized_chunks(a, b, c);
end_time = omp_get_wtime();
printf("Optimized (chunks): %.5f seconds\n", end_time - start_time);

free(a);
free(b);
free(c);

}
return 0;
}
