#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#define RUNS 5

static void sequential(
    const double *A,
    double *B,
    long long N)
{
    for (long long i = 0; i < N; i++)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

static void simd(
    const double *A,
    double *B,
    long long N)
{
#pragma omp simd
    for (long long i = 0; i < N; i++)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

static double checksum(
    const double *B,
    long long N)
{
    double sum = 0.0;

    for (long long i = 0; i < N; i++)
    {
        sum += B[i];
    }

    return sum;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: benchmark_simd.exe <data_size> <strategy>\n");
        printf("Strategy: sequential or simd\n");
        return 1;
    }

    long long N = atoll(argv[1]);
    const char *strategy = argv[2];

    if (N <= 0)
    {
        printf("Error: Invalid data size.\n");
        return 1;
    }

    if (strcmp(strategy, "sequential") != 0 &&
        strcmp(strategy, "simd") != 0)
    {
        printf("Error: Strategy must be sequential or simd.\n");
        return 1;
    }

    double *A =
        malloc((size_t)N * sizeof(double));

    double *B =
        malloc((size_t)N * sizeof(double));

    if (A == NULL || B == NULL)
    {
        printf("Memory allocation failed.\n");
        free(A);
        free(B);
        return 1;
    }

    for (long long i = 0; i < N; i++)
    {
        A[i] = i * 0.001;
    }

    double total_time = 0.0;
    double final_checksum = 0.0;

    for (int run = 0; run < RUNS; run++)
    {
        double start = omp_get_wtime();

        if (strcmp(strategy, "simd") == 0)
        {
            simd(A, B, N);
        }
        else
        {
            sequential(A, B, N);
        }

        double end = omp_get_wtime();

        total_time += end - start;
        final_checksum = checksum(B, N);
    }

    double mean_time = total_time / RUNS;

    printf("Data Size: %lld\n", N);
    printf("Strategy: %s\n", strategy);
    printf("Runs: %d\n", RUNS);
    printf("Mean Execution Time: %.9f seconds\n", mean_time);
    printf("Checksum: %.6f\n", final_checksum);

    printf("B[0]: %.6f\n", B[0]);
    printf("B[%lld]: %.6f\n", N - 1, B[N - 1]);

    free(A);
    free(B);

    return 0;
}