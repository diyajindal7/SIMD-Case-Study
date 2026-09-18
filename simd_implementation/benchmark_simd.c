#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define RUNS 7
#define INNER_REPETITIONS 100

int main(int argc, char *argv[])
{
    // =======================================
    // 1. Check data size
    // =======================================
    if (argc < 2)
    {
        printf("Error: Data size not provided.\n");
        return 1;
    }

    long long N = atoll(argv[1]);

    if (N <= 0)
    {
        printf("Error: Invalid data size.\n");
        return 1;
    }

    // =======================================
    // 2. Allocate arrays
    // =======================================
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));

    if (A == NULL || B == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // =======================================
    // 3. Initialize input data
    // =======================================
    for (long long i = 0; i < N; i++)
    {
        A[i] = i * 0.001;
    }

    double sequential_total = 0.0;
    double simd_total = 0.0;

    // =======================================
    // 4. Sequential benchmark
    // =======================================
    for (int run = 0; run < RUNS; run++)
    {
        double start = omp_get_wtime();

        for (int repeat = 0; repeat < INNER_REPETITIONS; repeat++)
        {
            for (long long i = 0; i < N; i++)
            {
                B[i] = A[i] * A[i] + 2 * A[i] + 1;
            }
        }

        double end = omp_get_wtime();

        sequential_total += (end - start);
    }

    // =======================================
    // 5. SIMD benchmark
    // =======================================
    for (int run = 0; run < RUNS; run++)
    {
        double start = omp_get_wtime();

        for (int repeat = 0; repeat < INNER_REPETITIONS; repeat++)
        {
            #pragma omp simd
            for (long long i = 0; i < N; i++)
            {
                B[i] = A[i] * A[i] + 2 * A[i] + 1;
            }
        }

        double end = omp_get_wtime();

        simd_total += (end - start);
    }

    // =======================================
    // 6. Calculate mean time
    // =======================================
    double sequential_mean =
        sequential_total / RUNS / INNER_REPETITIONS;

    double simd_mean =
        simd_total / RUNS / INNER_REPETITIONS;

    // =======================================
    // 7. Calculate SIMD speedup
    // =======================================
    double simd_speedup = sequential_mean / simd_mean;

    // =======================================
    // 8. Determine SIMD significance
    // =======================================
    int simd_significant;

    if (simd_speedup >= 1.5)
    {
        simd_significant = 1;
    }
    else
    {
        simd_significant = 0;
    }

    // =======================================
    // 9. Display results
    // =======================================
    printf("Data Size: %lld\n", N);
    printf("Runs: %d\n", RUNS);
    printf("Inner Repetitions: %d\n", INNER_REPETITIONS);

    printf("Mean Sequential Time: %.9f\n",
           sequential_mean);

    printf("Mean SIMD Time: %.9f\n",
           simd_mean);

    printf("SIMD Speedup: %.4f\n",
           simd_speedup);

    printf("SIMD Significant: %d\n",
           simd_significant);

    // =======================================
    // 10. Verify output
    // =======================================
    printf("B[0]: %f\n", B[0]);

    printf("B[%lld]: %f\n",
           N - 1,
           B[N - 1]);

    // =======================================
    // 11. Free memory
    // =======================================
    free(A);
    free(B);

    return 0;
}