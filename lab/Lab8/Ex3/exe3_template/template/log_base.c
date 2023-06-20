#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define NUM_ENTRIES 409600

struct log_entry{
    int src_ip;
    char URL[128];
    long reference_time;
    char bro[128];
    int status;
}logs[NUM_ENTRIES];


void tranverse(){
    for(int i=0;i<NUM_ENTRIES;i++){
        if(logs[i].status){
            logs[i].reference_time = 0;
            logs[i].src_ip = 0;
        }
    }
}

void make_logs(){
    for(int i=0;i<NUM_ENTRIES;i++){
        logs[i].src_ip = i*123;
        logs[i].reference_time = i*123;
        logs[i].status = 1;
    }
}

int main(){
    make_logs();
    struct timeval start_time, stop_time, elapsed_time; 
    gettimeofday(&start_time,NULL);
    for(int i=0;i<100;i++)
        tranverse();
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("%f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
    return 0;
}