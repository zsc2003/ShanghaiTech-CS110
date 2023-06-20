#include <emmintrin.h> /* header file for the SSE intrinsics */
#include <time.h>
#include <stdio.h>
#include <math.h>

float **a;
float **b;
float **c;
float **c2;

float **transform;

int n = 40000;
int p = 20000;

void init(void) {
    a = malloc(n * sizeof(float *));
    b = malloc(4 * sizeof(float *));
    c = malloc(n * sizeof(float *));
    c2 = malloc(n * sizeof(float *));
    transform = malloc(p * sizeof(float *));
    
    for (int i = 0; i < p; ++i) 
        transform[i] = malloc(4 * sizeof(float));
    
    for (int i = 0; i < n; ++i) {
        a[i] = malloc(4 * sizeof(float));
        c[i] = malloc(p * sizeof(float));
        c2[i] = malloc(p * sizeof(float));
    }
    for (int i = 0; i < 4; ++i) {
        b[i] = malloc(p * sizeof(float));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 4; ++j) {
            a[i][j] = (float) rand() / (float) RAND_MAX;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < p; ++j) {
            b[i][j] = (float) rand() / (float) RAND_MAX;
        }
    }

    for(int i = 0; i < p; ++i)
        for(int j = 0; j < 4; ++j)
            transform[i][j] = b[j][i];
}
#define eps 1e-6
void check_correctness(char *msg) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            if (fabsf(c[i][j] - c2[i][j]) > eps) {
                printf("%s incorrect!\n", msg);
                return;
            }
        }
    }
}

void naive_matmul(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // c = a * b
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            c[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("naive: %f\n", (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f);
}

void loop_unroll_matmul(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // c2 = a * b
    // TODO: implement me!

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            c2[i][j] = a[i][0] * b[0][j];
            c2[i][j] += a[i][1] * b[1][j];
            c2[i][j] += a[i][2] * b[2][j];
            c2[i][j] += a[i][3] * b[3][j];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("unroll: %f\n", (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f);
    check_correctness("loop_unroll_matmul");
}

void simd_matmul(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    // c2 = a * b
    // TODO: implement me!

    for (int i = 0; i < n; ++i)
    {
        __m128 A = _mm_loadu_ps(a[i]);
        for (int j = 0; j < p; ++j)
        {
            __m128 B = _mm_loadu_ps(transform[j]);
            __m128 C = _mm_mul_ps(A, B);
            c2[i][j] = C[0] + C[1] + C[2] + C[3];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("simd: %f\n", (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f);
    check_correctness("simd_matmul");
}

void loop_unroll_simd_matmul(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    // c2 = a * b
    // TODO: implement me!
    
    for (int i = 0; i < n; ++i)
    {
        __m128 A = _mm_loadu_ps(a[i]);
        for (int j = 0; j < p; j += 4)
        {
            __m128 B0 = _mm_loadu_ps(transform[j]);
            __m128 C0 = _mm_mul_ps(A, B0);
            c2[i][j] = C0[0] + C0[1] + C0[2] + C0[3];

            __m128 B1 = _mm_loadu_ps(transform[j+ 1] );
            __m128 C1 = _mm_mul_ps(A, B1);
            c2[i][j + 1] = C1[0] + C1[1] + C1[2] + C1[3];

            __m128 B2 = _mm_loadu_ps(transform[j+ 2] );
            __m128 C2 = _mm_mul_ps(A, B2);
            c2[i][j + 2] = C2[0] + C2[1] + C2[2] + C2[3];

            __m128 B3 = _mm_loadu_ps(transform[j+ 3]);
            __m128 C3 = _mm_mul_ps(A, B3);
            c2[i][j + 3] = C3[0] + C3[1] + C3[2] + C3[3];
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("unroll+simd: %f\n", (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f);
    check_correctness("loop_unroll_simd_matmul");
}


int main(void) {
    init();

    naive_matmul();
    simd_matmul();
    loop_unroll_matmul();
    loop_unroll_simd_matmul();

    return 0;
}

