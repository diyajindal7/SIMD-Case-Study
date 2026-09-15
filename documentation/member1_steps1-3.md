# SIMD-Oriented Data Processing
## Steps 1–3: Problem Definition, Parallelism and Sequential Baseline

## 1. Problem Definition and Objectives

Large numerical datasets often require the same mathematical operation to be performed independently on every data element. Executing such operations sequentially can become time-consuming as the dataset size increases.

This case study implements a numerical computation using a conventional sequential approach and a SIMD/vector-oriented approach. The performance of the approaches will be evaluated for different data sizes.

The mathematical operation selected for this case study is:

B[i] = A[i]^2 + 2A[i] + 1

where each input element A[i] is processed independently to produce the corresponding output element B[i].

### Objectives

1. Implement the computation using a sequential approach.
2. Implement the same computation using SIMD/vector-oriented processing.
3. Evaluate performance for different data sizes.
4. Measure execution time and speedup.
5. Analyze the performance benefits of SIMD.
6. Develop an AI model to predict whether SIMD will provide a significant performance advantage.
7. Use the prediction to select an appropriate execution strategy.

## 2. Parallelism and Dependencies

The computation is:

B[i] = A[i]^2 + 2A[i] + 1

Each output element B[i] depends only on the corresponding input element A[i].

Therefore, there are no dependencies between different iterations of the loop.

For example:

A[0] → B[0]
A[1] → B[1]
A[2] → B[2]
A[3] → B[3]

Each element can be calculated independently. Therefore, this problem has a high degree of data parallelism and is suitable for SIMD/vector processing.

## 3. Sequential Baseline

The sequential implementation processes one array element at a time using a conventional for loop.

### Input Initialization

The input array is initialized as:

A[i] = i × 0.001

### Computation

For every element:

B[i] = A[i] × A[i] + 2 × A[i] + 1

### Baseline Test

Data size:

N = 10,000,000

Latest measured sequential execution time:

0.078 seconds

Sample output:

B[0] = 1.000000

B[9999999] = 100019980.998001

The sequential implementation serves as the baseline for comparison with the parallel and SIMD implementations.