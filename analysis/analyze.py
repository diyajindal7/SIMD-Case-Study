import pandas as pd
from pathlib import Path

INPUT = Path("results/raw_results.csv")
OUT = Path("results/summary.csv")

df = pd.read_csv(INPUT)

# Average repeated measurements for each configuration.
summary = (
    df.groupby(["data_size", "threads", "implementation", "schedule"], as_index=False)
      .agg(
          mean_time=("time_seconds", "mean"),
          std_time=("time_seconds", "std"),
          min_time=("time_seconds", "min"),
          max_time=("time_seconds", "max"),
      )
)

summary["std_time"] = summary["std_time"].fillna(0)

# Sequential baseline for each data size.
seq = (
    summary[
        (summary["implementation"] == "sequential") &
        (summary["threads"] == 1)
    ][["data_size", "mean_time"]]
    .rename(columns={"mean_time": "sequential_time"})
)

summary = summary.merge(seq, on="data_size", how="left")

summary["speedup_vs_sequential"] = (
    summary["sequential_time"] / summary["mean_time"]
)

summary["efficiency"] = summary["speedup_vs_sequential"] / summary["threads"]

# For the main case-study question:
# SIMD is marked significant when it reaches at least 1.5x speedup.
simd_speedup = (
    summary[summary["implementation"] == "simd"]
    [["data_size", "speedup_vs_sequential"]]
    .rename(columns={"speedup_vs_sequential": "simd_speedup"})
)

summary = summary.merge(simd_speedup, on="data_size", how="left")

summary["simd_advantage_percent"] = (
    (summary["sequential_time"] - summary["mean_time"])
    / summary["sequential_time"] * 100.0
)

summary["simd_significant"] = (
    summary["simd_speedup"] >= 1.5
).astype(int)

summary.to_csv(OUT, index=False)

print("\n=== SIMD PERFORMANCE SUMMARY ===")
print(
    summary[
        (summary["implementation"].isin(["sequential", "simd"])) &
        (summary["threads"] == 1)
    ][
        [
            "data_size",
            "implementation",
            "mean_time",
            "speedup_vs_sequential",
            "simd_advantage_percent",
            "simd_significant",
        ]
    ].to_string(index=False)
)

print(f"\nSaved: {OUT}")
