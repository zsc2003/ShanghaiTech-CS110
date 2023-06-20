#include "main.h"

Image transpose(Image a){
    /* img_sc() will return a copy of the given image*/
    Image b = img_sc(a);
    b.dimX = a.dimY;
    b.dimY = a.dimX;

    for(int x=0;x<(int)a.dimX;x++){
        for(int y=0;y<(int)a.dimY;y++){
            b.data[x*b.dimX+y] = a.data[y*a.dimX+x];
        }
    }

    return b;
}

// use the function transpose() to implement the vertical gaussian blur in a fast way
Image apply_gb(Image a, FVec gv)
{
    struct timeval start_time, stop_time, elapsed_time; 
    gettimeofday(&start_time,NULL);

    Image b = gb_h(a, gv);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("horizontal gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    gettimeofday(&start_time,NULL);

    Image c = transpose(b);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("First transpose time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
    
    gettimeofday(&start_time,NULL);

    c = gb_h(c, gv);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("vertical gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
    
    gettimeofday(&start_time,NULL);

    c = transpose(c);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("Second transpose time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    free(b.data);
    return c;
}
