import pandas as pd
import pickle
import subprocess
import time
from pathlib import Path

# Project directory
script_dir = Path(__file__).resolve().parent

# Load trained AI model
model_path = script_dir / "simd_model.pkl"

with open(model_path, "rb") as file:
    model = pickle.load(file)

# Get input
data_size = int(input("Enter data size: "))

# Workload information
operation_complexity = 4
available_cores = 8
memory_footprint = data_size * 8 * 2

# Prepare input for AI model
workload = pd.DataFrame({
    "data_size": [data_size],
    "operation_complexity": [operation_complexity],
    "memory_footprint": [memory_footprint],
    "available_cores": [available_cores]
})

# AI prediction
prediction = model.predict(workload)[0]

print("\n========================================")
print("       AI-BASED EXECUTION DECISION")
print("========================================")

if prediction == 1:
    strategy = "SIMD"
    print("AI Prediction: SIMD Performance Advantage is SIGNIFICANT")
    print("Selected Strategy: SIMD")
else:
    strategy = "SEQUENTIAL"
    print("AI Prediction: SIMD Performance Advantage is NOT SIGNIFICANT")
    print("Selected Strategy: Sequential")

print("========================================")

# -------------------------------------------------
# Execute the selected strategy
# -------------------------------------------------

project_root = Path(__file__).resolve().parent.parent
benchmark_program = project_root / "simd_implementation" / "benchmark_simd.exe"

if not benchmark_program.exists():
    print("\nError: benchmark_simd.exe not found.")
    print("Compile it first using:")
    print("gcc -O3 -fopenmp simd_implementation/benchmark_simd.c "
          "-o simd_implementation/benchmark_simd.exe")
    raise SystemExit(1)

print("\nRunning selected strategy...")

start = time.perf_counter()

result = subprocess.run(
    [str(benchmark_program), str(data_size), strategy.lower()],
    capture_output=True,
    text=True
)

end = time.perf_counter()

print("\nExecution completed.")
print(f"Total program time: {end - start:.6f} seconds")

print("\nBenchmark output:")
print(result.stdout)

if result.returncode != 0:
    print("Execution failed.")
    print(result.stderr)