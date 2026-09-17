import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

INPUT = Path("results/summary.csv")
OUT = Path("graphs")
OUT.mkdir(parents=True, exist_ok=True)

df = pd.read_csv(INPUT)

# Main SIMD comparison.
main = df[
    (df["implementation"].isin(["sequential", "simd"])) &
    (df["threads"] == 1)
].copy()

# 1. Data size vs execution time.
plt.figure()
for name, group in main.groupby("implementation"):
    plt.plot(
        group["data_size"],
        group["mean_time"],
        marker="o",
        label=name
    )
plt.xscale("log")
plt.xlabel("Data Size (N)")
plt.ylabel("Mean Execution Time (seconds)")
plt.title("Data Size vs Execution Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(OUT / "data_size_vs_execution_time.png", dpi=200)
plt.close()

# 2. Data size vs SIMD speedup.
simd = main[main["implementation"] == "simd"].copy()

plt.figure()
plt.plot(
    simd["data_size"],
    simd["speedup_vs_sequential"],
    marker="o"
)
plt.axhline(1.5, linestyle="--", label="Significant threshold = 1.5x")
plt.xscale("log")
plt.xlabel("Data Size (N)")
plt.ylabel("SIMD Speedup")
plt.title("Data Size vs SIMD Speedup")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(OUT / "data_size_vs_simd_speedup.png", dpi=200)
plt.close()

# 3. Threads vs execution time for Member 2 strategies at largest N.
largest = df[
    (df["data_size"] == df["data_size"].max()) &
    (df["implementation"] == "parallel")
].copy()

plt.figure()
for name, group in largest.groupby("schedule"):
    plt.plot(
        group["threads"],
        group["mean_time"],
        marker="o",
        label=name
    )
plt.xlabel("Number of Threads")
plt.ylabel("Mean Execution Time (seconds)")
plt.title("Scheduling Strategy vs Thread Count")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(OUT / "threads_vs_scheduling_time.png", dpi=200)
plt.close()

# 4. Parallel speedup vs thread count for the largest workload.
plt.figure()
for name, group in largest.groupby("schedule"):
    plt.plot(
        group["threads"],
        group["speedup_vs_sequential"],
        marker="o",
        label=name
    )
plt.xlabel("Number of Threads")
plt.ylabel("Speedup vs Sequential")
plt.title("Thread Count vs Parallel Speedup")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(OUT / "threads_vs_parallel_speedup.png", dpi=200)
plt.close()

print("Graphs generated in graphs/")
