#include "main.h"

Image apply_gb(Image a, FVec gv)
{
    struct timeval start_time, stop_time, elapsed_time; 
    gettimeofday(&start_time,NULL);
    
    Image b = gb_h(a, gv);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("horizontal gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    gettimeofday(&start_time,NULL);

    Image c = gb_v(b,gv);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("vertical gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    free(b.data);
    return c;
}
