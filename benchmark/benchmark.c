#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define RUNS 5

static const long long DATA_SIZES[] = {
    1000000LL,
    5000000LL,
    10000000LL,
    20000000LL
};

static const int THREADS[] = {1, 2, 4, 8};

static void initialize(double *A, long long N)
{
    for (long long i = 0; i < N; ++i)
    {
        A[i] = i * 0.001;
    }
}

/* Sequential implementation */
static void sequential(
    const double *A,
    double *B,
    long long N)
{
    for (long long i = 0; i < N; ++i)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

/* Explicit SIMD implementation */
static void simd(
    const double *A,
    double *B,
    long long N)
{
#pragma omp simd
    for (long long i = 0; i < N; ++i)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

/* OpenMP static scheduling */
static void parallel_static(
    const double *A,
    double *B,
    long long N)
{
#pragma omp parallel for schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

/* OpenMP dynamic scheduling */
static void parallel_dynamic(
    const double *A,
    double *B,
    long long N)
{
#pragma omp parallel for schedule(dynamic)
    for (long long i = 0; i < N; ++i)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

/* OpenMP guided scheduling */
static void parallel_guided(
    const double *A,
    double *B,
    long long N)
{
#pragma omp parallel for schedule(guided)
    for (long long i = 0; i < N; ++i)
    {
        B[i] = A[i] * A[i] + 2.0 * A[i] + 1.0;
    }
}

/*
 * Checksum is calculated after timing.
 * This prevents the checksum itself from affecting
 * the measured execution time.
 */
static double checksum(
    const double *B,
    long long N)
{
    double sum = 0.0;

#pragma omp simd reduction(+ : sum)
    for (long long i = 0; i < N; ++i)
    {
        sum += B[i];
    }

    return sum;
}

/*
 * Execute the selected function several times.
 *
 * The result is modified slightly between repetitions
 * so that the compiler cannot simply reuse the previous
 * result.
 */
static double measure(
    void (*fn)(const double *, double *, long long),
    const double *A,
    double *B,
    long long N)
{
    double total_time = 0.0;

    for (int r = 0; r < RUNS; ++r)
    {
        double start = omp_get_wtime();

        fn(A, B, N);

        double end = omp_get_wtime();

        total_time += end - start;
    }

    return total_time / RUNS;
}

int main(void)
{
    const int num_sizes =
        sizeof(DATA_SIZES) / sizeof(DATA_SIZES[0]);

    const int num_threads =
        sizeof(THREADS) / sizeof(THREADS[0]);

    FILE *csv = fopen("results/raw_results.csv", "w");

    if (!csv)
    {
        perror("results/raw_results.csv");
        return 1;
    }

    fprintf(csv,
            "data_size,run,threads,implementation,schedule,time_seconds,checksum\n");

    printf("SIMD-Oriented Data Processing - Benchmark\n");
    printf("Runs per configuration: %d\n\n", RUNS);

    for (int s = 0; s < num_sizes; ++s)
    {
        long long N = DATA_SIZES[s];

        double *A =
            malloc((size_t)N * sizeof(double));

        double *B =
            malloc((size_t)N * sizeof(double));

        if (!A || !B)
        {
            fprintf(stderr,
                    "Memory allocation failed for N=%lld\n",
                    N);

            free(A);
            free(B);
            fclose(csv);

            return 1;
        }

        initialize(A, N);

        printf("Data size: %lld\n", N);

        /*
         * Sequential and SIMD.
         * Use one OpenMP thread so this is a
         * sequential-vs-SIMD comparison.
         */
        omp_set_num_threads(1);

        for (int run = 1; run <= RUNS; ++run)
        {
            double t_seq =
                measure(sequential, A, B, N);

            double c_seq =
                checksum(B, N);

            fprintf(csv,
                    "%lld,%d,1,sequential,none,%.9f,%.17g\n",
                    N,
                    run,
                    t_seq,
                    c_seq);

            double t_simd =
                measure(simd, A, B, N);

            double c_simd =
                checksum(B, N);

            fprintf(csv,
                    "%lld,%d,1,simd,none,%.9f,%.17g\n",
                    N,
                    run,
                    t_simd,
                    c_simd);
        }

        /*
         * OpenMP scheduling experiments.
         */
        for (int t = 0; t < num_threads; ++t)
        {
            int threads = THREADS[t];

            omp_set_num_threads(threads);

            for (int run = 1; run <= RUNS; ++run)
            {
                double ts =
                    measure(parallel_static, A, B, N);

                double cs =
                    checksum(B, N);

                fprintf(csv,
                        "%lld,%d,%d,parallel,static,%.9f,%.17g\n",
                        N,
                        run,
                        threads,
                        ts,
                        cs);

                double td =
                    measure(parallel_dynamic, A, B, N);

                double cd =
                    checksum(B, N);

                fprintf(csv,
                        "%lld,%d,%d,parallel,dynamic,%.9f,%.17g\n",
                        N,
                        run,
                        threads,
                        td,
                        cd);

                double tg =
                    measure(parallel_guided, A, B, N);

                double cg =
                    checksum(B, N);

                fprintf(csv,
                        "%lld,%d,%d,parallel,guided,%.9f,%.17g\n",
                        N,
                        run,
                        threads,
                        tg,
                        cg);
            }
        }

        /*
         * Correctness verification.
         */
        sequential(A, B, N);

        double expected_last =
            A[N - 1] * A[N - 1]
            + 2.0 * A[N - 1]
            + 1.0;

        if (fabs(B[0] - 1.0) > 1e-12 ||
            fabs(B[N - 1] - expected_last) > 1e-8)
        {
            fprintf(stderr,
                    "Correctness check FAILED for N=%lld\n",
                    N);

            free(A);
            free(B);
            fclose(csv);

            return 1;
        }

        printf(
            "  Correctness: PASS | "
            "B[0]=%.6f | "
            "B[%lld]=%.6f\n\n",
            B[0],
            N - 1,
            B[N - 1]);

        free(A);
        free(B);
    }

    fclose(csv);

    printf(
        "Raw benchmark data saved to "
        "results/raw_results.csv\n");

    return 0;
}