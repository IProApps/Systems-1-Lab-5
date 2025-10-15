#include <stdlib.h>
#include <stddef.h>
#include <stdio.h> 
#include <string.h>
#include <time.h>
#include "quicksort.h"


static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

int cmp_ints(const void *x, const void *y) {

    int a = *(const int*)x;
    int b = *(const int*)y;

    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    } else {
        return 0;
    }
}

int check_sorted(int *a, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) {
            printf("Out of order at index %zu: %d > %d\n", i, a[i], a[i+1]);
            return 0; 
        }
    }
    return 1; 
}


static void fill_random(int *a, size_t n) {
    srand((unsigned)time(NULL)); 
    for (size_t i = 0; i < n; ++i) a[i] = rand(); 
}

int main() {
    int *randomData1 = calloc(10000000, sizeof(int));
    int *randomData2 = calloc(10000000, sizeof(int));
    int *randomData3 = calloc(10000000, sizeof(int));

    fill_random(randomData1, 10000000);
    memcpy(randomData2, randomData1, sizeof(int) * 10000000);
    memcpy(randomData3, randomData1, sizeof(int) * 10000000);

    double t0 = now_ms(); 
    quicksort(randomData1, 10000000); 
    double t1 = now_ms();

    double t2 = now_ms(); 
    qsort(randomData2, 10000000, sizeof(int), cmp_ints); 
    double t3 = now_ms();

    double t4 = now_ms(); 
    quicksort2(randomData3, 10000000); 
    double t5 = now_ms();

    printf("My quicksort: %.3f ms\n", t1 - t0);
    printf("qsort: %.3f ms\n", t3 - t2);
    printf("My OPTIMIZED quicksort: %.3f ms\n", t5 - t4);

    int error1 = check_sorted(randomData1, 10000000);
    int error2 = check_sorted(randomData3, 10000000);

    error1 == 0 ? printf("Correctness of my quicksort: FAIL\n") : printf("Correctness: PASS\n");
    error2 == 0 ? printf("Correctness of my optimized quicksort: FAIL\n") : printf("Correctness: PASS\n");
    
    free(randomData1);
    free(randomData2);
    free(randomData3);

}