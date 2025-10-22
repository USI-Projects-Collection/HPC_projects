#!/usr/bin/env python3
"""Generate timing and speedup plots for the π midpoint integration benchmark."""

import math
import sys
from pathlib import Path

import matplotlib.pyplot as plt

LOG_DIR = Path("logs")
OUTPUT_DIR = Path("plots")
OUTPUT_DIR.mkdir(exist_ok=True)


def parse_logs():
    data = []
    for log_path in sorted(LOG_DIR.glob("pi_T*.log")):
        threads = int(log_path.stem.split("T")[1])
        serial = parallel = None
        with log_path.open() as fh:
            for line in fh:
                if "time_srl=" in line:
                    serial = float(line.split("time_srl=")[1].split()[0])
                elif "time_plr=" in line:
                    parallel = float(line.split("time_plr=")[1].split()[0])
        if serial is None or parallel is None:
            raise ValueError(f"Missing serial/parallel timing in {log_path}")
        speedup = serial / parallel if parallel > 0 else math.nan
        data.append((threads, serial, parallel, speedup))
    return data


def plot_times(data):
    threads = [d[0] for d in data]
    serial = [d[1] for d in data]
    parallel = [d[2] for d in data]

    plt.figure(figsize=(5.5, 3.2))
    plt.plot(threads, serial, marker="o", label="Serial")
    plt.plot(threads, parallel, marker="s", label="Parallel")
    plt.xscale("log", base=2)
    plt.yscale("log", base=2)
    plt.xticks(threads, threads)
    plt.xlabel("OMP threads")
    plt.ylabel("Time [s]")
    plt.title("π Midpoint Rule Timings (N = 10¹⁰)")
    plt.grid(True, which="both", linestyle="--", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    out_path = OUTPUT_DIR / "pi_times.png"
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Saved {out_path}")


def plot_speedup(data):
    threads = [d[0] for d in data]
    speedups = [d[3] for d in data]

    plt.figure(figsize=(5.5, 3.2))
    plt.plot(threads, speedups, marker="o", label="Parallel speedup vs serial")
    plt.xscale("log", base=2)
    plt.yscale("log", base=2)
    plt.xticks(threads, threads)
    plt.xlabel("OMP threads")
    plt.ylabel("Speedup vs. serial")
    plt.title("π Midpoint Rule Speedup (N = 10¹⁰)")
    plt.grid(True, which="both", linestyle="--", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    out_path = OUTPUT_DIR / "pi_speedup.png"
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Saved {out_path}")


def main():
    data = parse_logs()
    if not data:
        print("No logs found. Run run_pi.sh first.", file=sys.stderr)
        sys.exit(1)
    plot_times(data)
    plot_speedup(data)


if __name__ == "__main__":
    main()
