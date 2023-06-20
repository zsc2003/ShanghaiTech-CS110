#include <stdio.h>
#include <omp.h>

int main ()
{
  // default: num_threads = 8
  // int num_threads = 9465; //9466 error？
  int num_threads = 20;
  omp_set_num_threads(num_threads);
  int thread_ID = 0;
  #pragma omp parallel private(thread_ID)
  {
    thread_ID = omp_get_thread_num();
    printf (" hello world %d\n", thread_ID);
  }
  return 0;
}
