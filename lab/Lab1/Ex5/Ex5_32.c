#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define PRINT_SIZEOF(type) printf("Size of " #type " is %d\n", sizeof(type))

int main() {
    PRINT_SIZEOF(char);
    PRINT_SIZEOF(short);
    PRINT_SIZEOF(short int);
    PRINT_SIZEOF(int);
    PRINT_SIZEOF(long int);
    PRINT_SIZEOF(unsigned int);
    PRINT_SIZEOF(void *);
    PRINT_SIZEOF(size_t);
    PRINT_SIZEOF(float);
    PRINT_SIZEOF(double);
    PRINT_SIZEOF(int8_t);
    PRINT_SIZEOF(int16_t);
    PRINT_SIZEOF(int32_t);
    PRINT_SIZEOF(int64_t);
    PRINT_SIZEOF(time_t);
    PRINT_SIZEOF(clock_t);
    PRINT_SIZEOF(struct tm);
    PRINT_SIZEOF(NULL);

    return 0;
}