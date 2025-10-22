#!/usr/bin/env python3
"""Plot Mandelbrot runtimes for multiple image sizes and thread counts."""

import re
import sys
from pathlib import Path

import matplotlib.pyplot as plt

LOG_DIR = Path("logs")
OUTPUT_DIR = Path("plots")
OUTPUT_DIR.mkdir(exist_ok=True)

LOG_PATTERN = re.compile(r"mandel_(\d+x\d+)_T(\d+)\.log")


def parse_log(path: Path) -> float:
    """Return total time in seconds from a log file."""
    with path.open() as fh:
        for line in fh:
            if line.startswith("Total time"):
                return float(line.split()[-2])
    raise ValueError(f"Total time not found in {path}")


def collect_data():
    data = {}
    for log_path in sorted(LOG_DIR.glob("mandel_*_T*.log")):
        m = LOG_PATTERN.match(log_path.name)
        if not m:
            continue
        size = m.group(1)
        threads = int(m.group(2))
        total_time = parse_log(log_path)
        data.setdefault(size, []).append((threads, total_time))

    for size, runs in data.items():
        runs.sort(key=lambda x: x[0])
    return data


def plot_runtimes(data):
    plt.figure(figsize=(6, 4))
    for size, runs in sorted(data.items()):
        threads = [t for t, _ in runs]
        times = [time for _, time in runs]
        label = f"{size}"
        line, = plt.plot(threads, times, marker="o", label=label)
        serial_time = times[0]
        plt.plot(
            [min(threads), max(threads)],
            [serial_time, serial_time],
            linestyle="dashed",
            color=line.get_color(),
            alpha=0.6,
            label=f"{size} serial",
        )

    plt.xlabel("OMP threads")
    plt.ylabel("Total time [s]")
    plt.title("Mandelbrot Runtime vs Threads")
    plt.xscale("log", base=2)
    plt.xticks(sorted({t for runs in data.values() for t, _ in runs}))
    plt.grid(True, which="both", linestyle="--", alpha=0.3)
    plt.legend(title="Image size", fontsize="small")
    plt.tight_layout()

    out_path = OUTPUT_DIR / "mandel_runtime_scaling.png"
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Saved {out_path}")


def main():
    data = collect_data()
    if not data:
        print(f"No logs found in {LOG_DIR}", file=sys.stderr)
        sys.exit(1)
    plot_runtimes(data)


if __name__ == "__main__":
    main()
