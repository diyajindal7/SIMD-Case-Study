#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    long long N = 10000000;

    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));

    if (A == NULL || B == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize input array
    for (long long i = 0; i < N; i++) {
        A[i] = i * 0.001;
    }

    // Start timer
    clock_t start = clock();

    // Sequential computation
    for (long long i = 0; i < N; i++) {
        B[i] = A[i] * A[i] + 2 * A[i] + 1;
    }

    // Stop timer
    clock_t end = clock();

    double execution_time =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Data size: %lld\n", N);
    printf("Sequential execution time: %.6f seconds\n",
           execution_time);

    // Display sample results
    printf("B[0] = %.6f\n", B[0]);
    printf("B[%lld] = %.6f\n", N - 1, B[N - 1]);

    free(A);
    free(B);

    return 0;
}