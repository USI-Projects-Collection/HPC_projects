import re
import sys
from pathlib import Path
from typing import Dict, List

import matplotlib.pyplot as plt


def parse_time(line: str) -> float:
    """Extract the timing value in seconds from an output line."""
    match = re.search(r"= ([0-9.+-eE]+) sec", line)
    if not match:
        raise ValueError(f"Cannot parse time from line: {line}")
    return float(match.group(1))


def collect_results(log_dir: Path) -> Dict[int, Dict[str, List[float]]]:
    """Parse all log files in log_dir and return a nested dictionary of results."""
    pattern = re.compile(r"dotprod_N(\d+)_T(\d+)\.log$")
    results: Dict[int, Dict[str, List[float]]] = {}

    for log_path in sorted(log_dir.glob("dotprod_N*_T*.log")):
        match = pattern.match(log_path.name)
        if not match:
            continue

        N = int(match.group(1))
        threads = int(match.group(2))

        with log_path.open("r", encoding="utf-8") as fh:
            lines = [line.strip() for line in fh if line.strip()]

        if len(lines) < 3:
            raise ValueError(f"Unexpected format in {log_path}")

        serial_time = parse_time(lines[0])
        reduction_time = parse_time(lines[1])
        critical_time = parse_time(lines[2])

        entry = results.setdefault(
            N, {"threads": [], "serial": [], "reduction": [], "critical": []}
        )
        entry["threads"].append(threads)
        entry["serial"].append(serial_time)
        entry["reduction"].append(reduction_time)
        entry["critical"].append(critical_time)

    if not results:
        raise FileNotFoundError(
            f"No log files found in {log_dir}. Did you run run_jobs.sh?"
        )
    return results


def plot_results(results: Dict[int, Dict[str, List[float]]], output_dir: Path) -> None:
    """Generate and save a plot per problem size."""
    output_dir.mkdir(parents=True, exist_ok=True)

    for N in sorted(results.keys()):
        entry = results[N]
        indices = sorted(range(len(entry["threads"])), key=lambda i: entry["threads"][i])

        threads = [entry["threads"][i] for i in indices]
        reduction = [entry["reduction"][i] for i in indices]
        critical = [entry["critical"][i] for i in indices]
        # Serial time should be identical across runs; take the mean for robustness.
        serial_time = sum(entry["serial"]) / len(entry["serial"])

        plt.figure(figsize=(6, 4))
        plt.plot(threads, reduction, marker="o", label="Reduction")
        plt.plot(threads, critical, marker="s", label="Critical")
        plt.axhline(
            serial_time,
            linestyle="--",
            color="gray",
            label=f"Serial ({serial_time:.4g} s)",
        )

        plt.title(f"Dot Product Timing (N = {N:,})")
        plt.xlabel("OMP threads")
        plt.ylabel("Time [s]")
        plt.xticks(threads)
        plt.grid(True, linestyle="--", alpha=0.3)
        plt.legend()
        plt.tight_layout()

        out_path = output_dir / f"dotprod_N{N}.png"
        plt.savefig(out_path, dpi=150)
        plt.close()
        print(f"Saved {out_path}")


def main() -> None:
    base_dir = Path(__file__).resolve().parent

    log_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else base_dir / "logs"
    output_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else base_dir / "plots"

    results = collect_results(log_dir)
    plot_results(results, output_dir)


if __name__ == "__main__":
    main()
