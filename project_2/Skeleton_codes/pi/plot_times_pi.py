#!/usr/bin/env python3
"""Plot serial vs. parallel runtimes for the π midpoint integration benchmark."""

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
            raise ValueError(f"Missing serial/parallel timings in {log_path}")
        data.append((threads, serial, parallel))
    return data


def plot_times(data):
    threads = [d[0] for d in data]
    serial_times = [d[1] for d in data]
    parallel_times = [d[2] for d in data]

    plt.figure(figsize=(5.5, 3.2))
    plt.plot(threads, serial_times, marker="o", label="Serial (re-run)")
    plt.plot(threads, parallel_times, marker="s", label="Parallel")
    plt.xlabel("OMP threads")
    plt.ylabel("Time [s]")
    plt.title("π Midpoint Rule Timings (N = 10¹⁰)")
    plt.xticks(threads)
    plt.grid(True, linestyle="--", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    out_path = OUTPUT_DIR / "pi_times.png"
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Saved {out_path}")


def main():
    data = parse_logs()
    if not data:
        raise SystemExit("No timing logs found. Run run_pi.sh first.")
    plot_times(data)


if __name__ == "__main__":
    main()
