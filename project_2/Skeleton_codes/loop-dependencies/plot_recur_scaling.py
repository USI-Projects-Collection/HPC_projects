#!/usr/bin/env python3
"""Plot runtime scaling for recurrence example (static chunk vs dynamic check)."""

from __future__ import annotations

import re
from pathlib import Path

import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parent
PAR_PATTERN = re.compile(r"recur_par_T(\d+)\.log")

LOG_STATIC = BASE_DIR / "logs"
LOG_DYNAMIC = BASE_DIR / "logs2"


def parse_directory(directory: Path):
    records = []
    for log in sorted(directory.glob("recur_par_T*.log")):
        m = PAR_PATTERN.match(log.name)
        if not m:
            continue
        threads = int(m.group(1))
        chunk = dyn = None
        with log.open() as fh:
            for line in fh:
                if "Parallel RunTime (Chunk Division):" in line:
                    chunk = float(line.split(":")[1].split()[0])
                elif "Parallel RunTime (Dynamic Check):" in line:
                    dyn = float(line.split(":")[1].split()[0])
        records.append((threads, chunk, dyn))
    records.sort()
    return records


records_static = parse_directory(LOG_STATIC)
records_dynamic = parse_directory(LOG_DYNAMIC)
if not records_static:
    raise SystemExit("No recur_par_T*.log files found in logs/")
if not records_dynamic:
    raise SystemExit("No recur_par_T*.log files found in logs2/")

threads_static = [r[0] for r in records_static]
chunk_times_static = [r[1] for r in records_static]
threads_dynamic = [r[0] for r in records_dynamic]
dyn_times = [r[2] for r in records_dynamic]

serial_time = None
seq_file = LOG_STATIC / "recur_seq_T1.log"
if seq_file.exists():
    with seq_file.open() as fh:
        for line in fh:
            if "Sequential RunTime" in line:
                serial_time = float(line.split(":")[1].split()[0])
                break

plt.figure(figsize=(7, 4.5))
plt.plot(threads_static, chunk_times_static, marker="o", color="tab:blue",
         label="Chunk division (static)")
plt.plot(threads_dynamic, dyn_times, marker="s", color="tab:orange",
         label="Dynamic with check")

if serial_time is not None:
    xmin = min(min(threads_static), min(threads_dynamic))
    xmax = max(max(threads_static), max(threads_dynamic))
    plt.plot([xmin, xmax], [serial_time, serial_time],
             linestyle="dashed", color="black",
             label=f"Serial baseline ({serial_time:.3f}s)")

all_threads = sorted(set(threads_static) | set(threads_dynamic))
plt.xscale("log", base=2)
plt.xticks(all_threads, [str(t) for t in all_threads])
plt.yscale("log")
plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.title("Recurrence runtime vs threads")
plt.grid(True, which="major", linestyle="--", alpha=0.3)
plt.grid(True, which="minor", linestyle=":", alpha=0.2)
plt.legend(fontsize="small")
plt.tight_layout()

out_dir = BASE_DIR / "plots"
out_dir.mkdir(exist_ok=True)
out_path = out_dir / "recur_runtime_scaling.png"
plt.savefig(out_path, dpi=150)
print(f"Saved {out_path}")
