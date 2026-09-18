#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{

    long long N = 10000000;

    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));

    if (A == NULL || B == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize input array
    for (long long i = 0; i < N; i++)
    {
        A[i] = i * 0.001;
    }

    int thread_counts[] = {1, 2, 4, 8};
    int num_tests = 4;

    printf("SIMD-Oriented Data Processing\n");
    printf("Data size: %lld\n\n", N);

    for (int t = 0; t < num_tests; t++)
    {

        int threads = thread_counts[t];

        omp_set_num_threads(threads);

        printf("====================================\n");
        printf("Number of Threads: %d\n", threads);
        printf("====================================\n");

        // ---------------- STATIC ----------------

        double start = omp_get_wtime();

#pragma omp parallel for schedule(static)
        for (long long i = 0; i < N; i++)
        {
            B[i] = A[i] * A[i] + 2 * A[i] + 1;
        }

        double end = omp_get_wtime();

        printf("Static  : %.6f seconds\n", end - start);

        // ---------------- DYNAMIC ----------------

        start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic)
        for (long long i = 0; i < N; i++)
        {
            B[i] = A[i] * A[i] + 2 * A[i] + 1;
        }

        end = omp_get_wtime();

        printf("Dynamic : %.6f seconds\n", end - start);

        // ---------------- GUIDED ----------------

        start = omp_get_wtime();

#pragma omp parallel for schedule(guided)
        for (long long i = 0; i < N; i++)
        {
            B[i] = A[i] * A[i] + 2 * A[i] + 1;
        }

        end = omp_get_wtime();

        printf("Guided  : %.6f seconds\n\n", end - start);
    }

    // Display sample results
    printf("Sample Results\n");
    printf("B[0] = %.6f\n", B[0]);
    printf("B[%lld] = %.6f\n", N - 1, B[N - 1]);

    free(A);
    free(B);

    return 0;
}
