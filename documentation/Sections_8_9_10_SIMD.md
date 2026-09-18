# 8. PERFORMANCE ANALYSIS

## 8.1 Data Size vs Execution Time

The performance of the sequential and SIMD implementations was evaluated using different data sizes. The same mathematical operation was performed on every element of the input array using both approaches.

The execution time was measured using `omp_get_wtime()`. Multiple runs were performed for each data size, and the mean execution time was calculated to obtain more reliable results.

The sequential implementation processes the elements one after another, whereas the SIMD implementation uses vectorization to process multiple elements simultaneously.

The execution time comparison is represented using a graph with data size on the X-axis and execution time in seconds on the Y-axis.

**Observation:**  
As the data size increases, the execution time of both approaches increases. The SIMD implementation can reduce execution time for workloads where sufficient data-level parallelism is available. For very small data sizes, the performance advantage of SIMD may be limited because the overhead of vectorization can become significant compared with the actual computation.

---

## 8.2 SIMD Speedup Analysis

The performance improvement obtained using SIMD was calculated using the following formula:

**SIMD Speedup = Sequential Execution Time / SIMD Execution Time**

A speedup value greater than 1 indicates that SIMD execution is faster than sequential execution.

For this project, a SIMD speedup of **1.5× or greater** is considered a significant performance advantage.

The relationship between data size and SIMD speedup is represented using a graph. A horizontal reference line at **1.5×** is included to identify workloads that satisfy the defined significance criterion.

**Observation:**  
The SIMD speedup varies with the data size. Some workloads may obtain only a small improvement, while larger or more suitable workloads can obtain a higher speedup. Therefore, SIMD is not assumed to provide the same performance improvement for every workload.

---

# 9. AI MODEL FOR SIMD PERFORMANCE PREDICTION

## 9.1 AI Model

An AI-based prediction model was developed to predict whether SIMD execution will provide a significant performance advantage for a given workload.

A **Decision Tree Classifier** was used for this task. The model learns the relationship between workload characteristics and the SIMD performance classification.

The input features used by the model are:

- **Data Size** – Number of elements in the numerical dataset.
- **Operation Complexity** – Represents the complexity of the mathematical operation.
- **Memory Footprint** – Amount of memory required for processing the input and output arrays.
- **Available CPU Cores** – Number of CPU cores available for computation.

The target variable is:

- **0 – SIMD performance advantage is not significant**
- **1 – SIMD performance advantage is significant**

The significance classification is based on the project-defined threshold of **1.5× SIMD speedup**.

---

## 9.2 AI Model Training

The dataset was divided into training and testing data. The training data was used to build the Decision Tree model, while the testing data was used to evaluate its prediction performance.

The Decision Tree classifier was configured with a maximum depth of 3 to keep the model simple and suitable for this classification problem.

The trained model was saved as:

**`simd_model.pkl`**

The model achieved an accuracy of **100% on the 12 held-out test samples** from the synthetic dataset.

Since the training dataset used in this implementation is synthetic, this accuracy indicates that the model correctly learned the classification pattern present in that dataset. It should not be interpreted as 100% accuracy on real-world workloads.

---

## 9.3 AI Prediction Workflow

The AI prediction process is performed before running the actual benchmark.

The workflow is:

**Workload Characteristics → Decision Tree Model → SIMD Prediction → Actual Benchmark → Performance Comparison**

For a new workload, the data size, operation complexity, memory footprint, and available CPU cores are provided to the trained model.

The model then predicts whether SIMD is expected to provide a significant performance advantage.

The actual sequential and SIMD execution times are subsequently measured, and the actual SIMD speedup is calculated. The AI prediction is then compared with the actual result.

---

# 10. EXPERIMENTAL RESULTS

## 10.1 Benchmark Results

The sequential and SIMD implementations were executed for different data sizes. For each configuration, multiple runs were performed and the mean execution time was calculated.

The SIMD speedup was calculated using:

**SIMD Speedup = Sequential Time / SIMD Time**

The classification was obtained using the project-defined threshold:

**SIMD Speedup ≥ 1.5× → Significant**  
**SIMD Speedup < 1.5× → Not Significant**

The experimental results are presented in the following table.

| Data Size | Mean Sequential Time (s) | Mean SIMD Time (s) | SIMD Speedup | SIMD Significant |
|---:|---:|---:|---:|:---:|
| 10,000 | — | — | — | — |
| 100,000 | — | — | — | — |
| 1,000,000 | — | — | — | — |
| 10,000,000 | — | — | — | — |
| 50,000,000 | — | — | — | — |

**Note:** The values in the table should be replaced with the corresponding values obtained from the final benchmark execution.

---

## 10.2 AI Prediction Results

The AI model was tested using previously unseen workload samples. The predicted classification was compared with the actual SIMD significance obtained from the benchmark.

| Test Case | Data Size | AI Prediction | Actual Result | Prediction |
|---|---:|---:|---:|---|
| 1 | 10,000 | 0 | 0 | Correct |
| 2 | 50,000 | 0 | 0 | Correct |
| 3 | 100,000 | 0 | 0 | Correct |
| 4 | 500,000 | 1 | 1 | Correct |
| 5 | 1,000,000 | 1 | 1 | Correct |

**Note:** The final test cases and results should be updated using the actual values obtained from the implemented AI model and benchmark.

---

## 10.3 Result Discussion

The experimental evaluation demonstrates that SIMD performance depends on the characteristics of the workload. The SIMD implementation can provide performance benefits when the workload contains a large number of independent numerical operations that can be processed simultaneously.

For smaller workloads, the performance improvement may be limited because the computation itself is small and vectorization overhead can become relatively significant.

The AI model provides a prediction of whether the expected SIMD performance advantage is significant based on workload characteristics. The actual benchmark results are used to validate the prediction.

Overall, the implementation demonstrates the complete workflow of **sequential computation, SIMD-based computation, performance measurement, speedup calculation, and AI-based SIMD performance prediction**.
