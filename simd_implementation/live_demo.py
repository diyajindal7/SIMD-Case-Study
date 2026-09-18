import pandas as pd
import pickle

# Load trained AI model
with open("simd_model.pkl", "rb") as file:
    model = pickle.load(file)

# Get input
data_size = int(input("Enter data size: "))

# Workload information
operation_complexity = 4
available_cores = 8
memory_footprint = data_size * 8 * 2

# Prepare input
workload = pd.DataFrame({
    "data_size": [data_size],
    "operation_complexity": [operation_complexity],
    "memory_footprint": [memory_footprint],
    "available_cores": [available_cores]
})

# AI prediction
prediction = model.predict(workload)[0]

# Simple output
if prediction == 1:
    print("AI Prediction: SIMD Performance Advantage is SIGNIFICANT")
else:
    print("AI Prediction: SIMD Performance Advantage is NOT SIGNIFICANT")