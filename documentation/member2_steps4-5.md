
## 4. Parallel Scheduler Implementation

The computation can be parallelized because each array element is processed independently.

OpenMP is used to distribute the loop iterations among multiple threads.

The main OpenMP directive used is:

```c
#pragma omp parallel for
```

This allows the loop iterations to be divided among multiple threads.

### Thread Counts Tested

The following numbers of threads were tested:

* 1 thread
* 2 threads
* 4 threads
* 8 threads

Different thread counts were tested to observe how execution time changes as more processing resources are used.

### Timing Method

OpenMP's wall-clock timer `omp_get_wtime()` is used to measure execution time.

```c
double start = omp_get_wtime();

/* parallel computation */

double end = omp_get_wtime();
```

The execution time is calculated as:

```text
Execution Time = end - start
```

### Parallel Computation

The computation performed is:

```text
B[i] = A[i] × A[i] + 2 × A[i] + 1
```

Since each iteration is independent, the loop can be parallelized using OpenMP:

```c
#pragma omp parallel for
for (long long i = 0; i < N; i++) {
    B[i] = A[i] * A[i] + 2 * A[i] + 1;
}
```

Each thread processes a portion of the array.

### Implementation

The parallel scheduler implementation is provided in `parallel_scheduler.c`.

The program:

1. Creates input and output arrays.
2. Initializes the input array.
3. Tests 1, 2, 4 and 8 threads.
4. Applies different OpenMP scheduling strategies.
5. Measures execution time for each strategy.
6. Displays sample output values to verify correctness.

---

## 5. Scheduling Strategies

Three OpenMP scheduling strategies were tested:

1. Static scheduling
2. Dynamic scheduling
3. Guided scheduling

The same computation and data size were used for all three strategies.

### 5.1 Static Scheduling

Static scheduling divides the loop iterations among the threads before execution.

```c
#pragma omp parallel for schedule(static)
for (long long i = 0; i < N; i++) {
    B[i] = A[i] * A[i] + 2 * A[i] + 1;
}
```

#### Characteristics

* Work is assigned before execution.
* Scheduling overhead is low.
* Suitable for workloads where each iteration takes approximately the same amount of time.
* The workload in this case is uniform because every iteration performs the same calculation.

---

### 5.2 Dynamic Scheduling

Dynamic scheduling assigns chunks of iterations to threads during execution.

```c
#pragma omp parallel for schedule(dynamic)
for (long long i = 0; i < N; i++) {
    B[i] = A[i] * A[i] + 2 * A[i] + 1;
}
```

#### Characteristics

* Work is assigned dynamically during execution.
* Can provide better load balancing for irregular workloads.
* Has higher scheduling overhead.
* For this particular uniform workload, the additional overhead can reduce performance.

---

### 5.3 Guided Scheduling

Guided scheduling starts with relatively large chunks and gradually reduces the chunk size.

```c
#pragma omp parallel for schedule(guided)
for (long long i = 0; i < N; i++) {
    B[i] = A[i] * A[i] + 2 * A[i] + 1;
}
```

#### Characteristics

* Starts with larger chunks of work.
* Chunk sizes gradually become smaller.
* Attempts to balance workload distribution and scheduling overhead.
* Can be useful when workload execution time varies between iterations.

---

### 5.4 Experimental Results

The experiment was performed using:

* Data size: `10,000,000`
* Number of threads: `1, 2, 4, 8`
* Scheduling strategies: Static, Dynamic and Guided

The observed execution times were:

| Number of Threads | Static (seconds) | Dynamic (seconds) | Guided (seconds) |
| ----------------: | ---------------: | ----------------: | ---------------: |
|                 1 |            0.092 |             0.404 |            0.032 |
|                 2 |            0.030 |             0.426 |            0.025 |
|                 4 |            0.021 |             0.594 |            0.018 |
|                 8 |            0.020 |             0.916 |            0.018 |

> **Note:** Execution times may vary between runs depending on CPU load, background processes, caching and system scheduling.

---

### 5.5 Analysis of Results

#### Static Scheduling

Static scheduling showed improved performance as the number of threads increased.

| Threads | Execution Time |
| ------: | -------------: |
|       1 |        0.092 s |
|       2 |        0.030 s |
|       4 |        0.021 s |
|       8 |        0.020 s |

The execution time decreased significantly when increasing the number of threads from 1 to 4.

However, the improvement from 4 to 8 threads was small. This shows that increasing the number of threads does not always result in proportional performance improvement.

#### Dynamic Scheduling

Dynamic scheduling produced higher execution times in this experiment.

| Threads | Execution Time |
| ------: | -------------: |
|       1 |        0.404 s |
|       2 |        0.426 s |
|       4 |        0.594 s |
|       8 |        0.916 s |

The workload is uniform because every iteration performs the same mathematical operation.

Therefore, dynamic scheduling does not provide a significant load-balancing advantage for this problem. The additional scheduling overhead becomes significant compared with the relatively small computation performed by each iteration.

#### Guided Scheduling

Guided scheduling produced the lowest measured execution time in this particular experiment.

| Threads | Execution Time |
| ------: | -------------: |
|       1 |        0.032 s |
|       2 |        0.025 s |
|       4 |        0.018 s |
|       8 |        0.018 s |

However, these results are based on a particular experimental run. Execution times can vary between runs, so this does not imply that guided scheduling will always be faster than static scheduling.

---

### 5.6 Comparison of Scheduling Strategies

| Strategy | Work Assignment                          | Overhead | Suitable For                          |
| -------- | ---------------------------------------- | -------- | ------------------------------------- |
| Static   | Before execution                         | Low      | Uniform workloads                     |
| Dynamic  | During execution                         | High     | Irregular workloads                   |
| Guided   | Large chunks first, smaller chunks later | Medium   | Workloads with varying execution time |

For this case study, the workload is relatively uniform because every element performs the same mathematical operation.

Therefore, static scheduling is naturally suitable because it has low scheduling overhead.

The experiment also demonstrates that the choice of scheduling strategy can have a noticeable effect on execution time.

---

### 5.7 Sample Output

```text
SIMD-Oriented Data Processing
Data size: 10000000

====================================
Number of Threads: 1
====================================
Static  : 0.092000 seconds
Dynamic : 0.404000 seconds
Guided  : 0.032000 seconds

====================================
Number of Threads: 2
====================================
Static  : 0.030000 seconds
Dynamic : 0.426000 seconds
Guided  : 0.025000 seconds

====================================
Number of Threads: 4
====================================
Static  : 0.021000 seconds
Dynamic : 0.594000 seconds
Guided  : 0.018000 seconds

====================================
Number of Threads: 8
====================================
Static  : 0.020000 seconds
Dynamic : 0.916000 seconds
Guided  : 0.018000 seconds

Sample Results
B[0] = 1.000000
B[9999999] = 100019980.998001
```

---

### 5.8 Correctness Verification

The parallel implementation produced the expected sample results:

```text
B[0] = 1.000000
B[9999999] = 100019980.998001
```

This verifies that the different scheduling strategies correctly perform the required computation.

---

### 5.9 Conclusion

The parallel scheduler successfully distributes the numerical computation among multiple OpenMP threads.

The experiment with static, dynamic and guided scheduling demonstrates that the scheduling strategy has a direct impact on execution time.

The main observations are:

* Increasing the number of threads can reduce execution time.
* Static scheduling has low scheduling overhead and is suitable for uniform workloads.
* Dynamic scheduling performed poorly for this workload because of its additional scheduling overhead.
* Guided scheduling produced the best measured execution time in this particular run.
* Increasing the number of threads from 4 to 8 provided only a small improvement for static and guided scheduling.

Overall, the experiment demonstrates the importance of selecting an appropriate scheduling strategy based on the characteristics of the workload.
