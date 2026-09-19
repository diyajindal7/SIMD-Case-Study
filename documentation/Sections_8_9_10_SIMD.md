# 8. PERFORMANCE ANALYSIS

## 8.1 Data Size vs Execution Time

The performance of the sequential and SIMD implementations was evaluated using four different data sizes:

- 1,000,000 elements
- 5,000,000 elements
- 10,000,000 elements
- 20,000,000 elements

The same mathematical operation was performed on every element:

B[i] = A[i]^2 + 2A[i] + 1

Execution time was measured using `omp_get_wtime()`. Five runs were performed for each configuration and the mean execution time was calculated.

The sequential implementation processes the elements using a conventional loop, while the SIMD implementation uses the OpenMP `simd` directive to enable vectorization.

The relationship between data size and execution time is represented using the generated performance graphs.

### Observation

For the tested workloads, execution time generally increased as the data size increased.

However, the SIMD implementation did not provide a large performance improvement on the tested system. The measured SIMD speedup remained close to 1× for all tested data sizes.

This indicates that SIMD performance depends not only on the amount of data but also on factors such as compiler vectorization, memory access, processor architecture, and the characteristics of the workload.

---

## 8.2 SIMD Speedup Analysis

SIMD speedup was calculated using:

**SIMD Speedup = Sequential Execution Time / SIMD Execution Time**

A speedup greater than 1 indicates that SIMD was faster than the sequential implementation.

For this project, a SIMD speedup of **1.5× or greater** is defined as a significant performance advantage.

The measured results were:

| Data Size | Sequential Time (s) | SIMD Time (s) | SIMD Speedup | Significant |
|---:|---:|---:|---:|:---:|
| 1,000,000 | 0.00160 | 0.00148 | 1.081× | No |
| 5,000,000 | 0.00852 | 0.00776 | 1.098× | No |
| 10,000,000 | 0.01944 | 0.02080 | 0.935× | No |
| 20,000,000 | 0.03256 | 0.03228 | 1.009× | No |

### Observation

The highest measured SIMD speedup was approximately **1.098×** for the 5,000,000-element workload.

The 10,000,000-element workload produced a speedup below 1, meaning that the measured SIMD execution was slightly slower than the sequential execution.

None of the tested workloads reached the project-defined threshold of 1.5×.

Therefore, based on the measured experiments, SIMD did not provide a significant performance advantage for this particular workload on the tested system.

---

# 9. AI MODEL FOR SIMD PERFORMANCE PREDICTION

## 9.1 AI Model

An AI-based prediction model was developed to predict whether SIMD execution would provide a significant performance advantage for a given workload.

A **Decision Tree Classifier** was used for this task.

The input features used by the model were:

- **Data Size** – Number of elements in the numerical dataset.
- **Operation Complexity** – Represents the complexity of the mathematical operation.
- **Memory Footprint** – Memory required for the input and output arrays.
- **Available CPU Cores** – Number of CPU cores available for computation.

The target variable was:

- **0** – SIMD performance advantage is not significant.
- **1** – SIMD performance advantage is significant.

The project-defined significance threshold is:

**SIMD Speedup ≥ 1.5× → Significant**

**SIMD Speedup < 1.5× → Not Significant**

---

## 9.2 AI Model Training

A synthetic dataset containing 60 workload samples was used to train the Decision Tree model.

The dataset was divided into training and testing sets using an 80:20 split.

The Decision Tree classifier was configured with a maximum depth of 3.

The trained model was saved as:

**`simd_model.pkl`**

The model achieved **100% accuracy on the 12 held-out samples from the synthetic test dataset**.

This accuracy only describes performance on the held-out samples from the synthetic dataset. It should not be interpreted as 100% accuracy on real-world workloads or on the actual computer used for benchmarking.

---

## 9.3 AI Prediction and Execution Integration

The trained AI model was integrated with the execution program.

The workflow is:

**Workload Characteristics → Decision Tree → Prediction → Strategy Selection → Actual Execution → Performance Measurement**

For a new workload, the user provides the data size.

The system calculates the corresponding workload features and provides them to the Decision Tree model.

The model predicts whether SIMD is expected to provide a significant advantage.

Based on the prediction:

- If the prediction is **1**, the SIMD implementation is selected.
- If the prediction is **0**, the sequential implementation is selected.

The selected implementation is then executed by the C program and its execution time is measured.

This provides an actual AI-based strategy selection mechanism rather than only displaying a prediction.

---

# 10. EXPERIMENTAL RESULTS

## 10.1 Final SIMD Benchmark Results

The final benchmark was performed using four data sizes.

Five runs were performed for each configuration and the mean execution time was calculated.

The results are:

| Data Size | Mean Sequential Time (s) | Mean SIMD Time (s) | SIMD Speedup | SIMD Significant |
|---:|---:|---:|---:|:---:|
| 1,000,000 | 0.00160 | 0.00148 | 1.081× | No |
| 5,000,000 | 0.00852 | 0.00776 | 1.098× | No |
| 10,000,000 | 0.01944 | 0.02080 | 0.935× | No |
| 20,000,000 | 0.03256 | 0.03228 | 1.009× | No |

The results show that none of the tested workloads achieved the predefined 1.5× SIMD significance threshold.

---

## 10.2 OpenMP Scheduling Results

OpenMP parallel execution was also evaluated using:

- 1 thread
- 2 threads
- 4 threads
- 8 threads

and three scheduling strategies:

- Static
- Dynamic
- Guided

For the tested workload, dynamic scheduling produced substantially higher execution times than static and guided scheduling.

For example, for 20,000,000 elements using 8 threads:

| Schedule | Mean Time (s) |
|---|---:|
| Static | 0.03860 |
| Guided | 0.03868 |
| Dynamic | 1.54240 |

The large overhead of dynamic scheduling is expected for this workload because every loop iteration performs approximately the same amount of computation. Dynamic scheduling provides little load-balancing benefit when the workload is already uniform, while its scheduling overhead remains.

The experiment also showed that increasing the number of threads did not necessarily improve execution time. Thread-management overhead, memory access behavior, cache effects, and the relatively simple computation can limit the benefit of additional threads.

---

## 10.3 AI Prediction vs Actual Benchmark

The AI model was trained using a synthetic dataset and was then integrated into the execution system.

For example, when a workload of 1,000,000 elements was entered:

**AI prediction:**

SIMD Performance Advantage = Significant

**Selected strategy:**

SIMD

The selected SIMD implementation was then actually executed by the C program.

However, the independent benchmark results for 1,000,000 elements showed:

**SIMD Speedup = 1.081×**

Since:

**1.081× < 1.5×**

the measured result is classified as:

**SIMD Advantage = Not Significant**

Therefore, the AI prediction did not match the measured result for this workload.

This difference demonstrates an important limitation of the current model. The model was trained using synthetic data whose classification pattern does not fully represent the actual hardware performance behavior observed during benchmarking.

---

## 10.4 Performance Analysis

The experiments demonstrate that SIMD performance is workload- and hardware-dependent.

For the selected mathematical operation:

B[i] = A[i]^2 + 2A[i] + 1

the computation is highly data-parallel because each output element is independent.

However, the measured SIMD speedup was relatively small on the tested system.

The observed speedups ranged from approximately:

**0.935× to 1.098×**

None of the workloads reached the 1.5× significance threshold.

The OpenMP experiments also showed that scheduling overhead can have a major effect on performance. Dynamic scheduling was considerably slower than static and guided scheduling for the uniform workload because there was little need for dynamic load balancing.

---

## 10.5 Optimization Discussion

Based on the experimental results, several improvements could be considered.

### 1. Use representative training data

The current AI model uses a synthetic dataset. A stronger model could be trained using benchmark measurements collected from the actual target system.

### 2. Include more workload features

Additional features could include:

- CPU architecture
- vector instruction width
- cache size
- memory bandwidth
- compiler optimization level
- measured baseline execution time

### 3. Increase the number of benchmark samples

More data sizes and repeated measurements could provide a more representative training dataset.

### 4. Reduce scheduling overhead

For uniform workloads, static scheduling can avoid the additional scheduling overhead associated with dynamic scheduling.

### 5. Use compiler optimization

Compiler optimization and hardware vectorization support can significantly affect SIMD performance. Therefore, consistent compiler flags and experimental conditions should be maintained when comparing implementations.

---

## 10.6 Final Conclusion

This case study implemented and evaluated SIMD-oriented numerical data processing.

The same mathematical operation:

B[i] = A[i]^2 + 2A[i] + 1

was implemented using sequential and SIMD approaches.

The experiment demonstrated that the computation contains a high degree of data parallelism because each element can be processed independently.

Performance measurements were collected for multiple data sizes, and SIMD speedup was calculated relative to the sequential baseline.

For the tested workloads, SIMD speedup ranged from approximately 0.935× to 1.098×, and none reached the predefined 1.5× significance threshold.

OpenMP scheduling experiments demonstrated that scheduling strategy can significantly affect performance. Dynamic scheduling introduced considerable overhead for the uniform workload, while static and guided scheduling produced much lower measured times in the tested configurations.

A Decision Tree AI model was also developed to predict whether SIMD would provide a significant performance advantage. The model achieved 100% accuracy on the held-out samples of its synthetic test dataset. The model was then integrated with the execution system so that its prediction could select either SIMD or sequential execution.

The comparison between AI predictions and actual benchmark results showed that the current synthetic training data does not fully represent the behavior of the real hardware. This provides an opportunity for future improvement by training the model using larger and more representative real benchmark datasets.

Overall, the project demonstrates the complete workflow of **sequential computation, SIMD processing, OpenMP parallel scheduling, performance measurement, AI-based prediction, strategy selection, and experimental evaluation**.