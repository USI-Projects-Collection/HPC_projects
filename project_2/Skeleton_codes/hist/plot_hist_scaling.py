#!/usr/bin/env python3
"""Plot histogram runtimes (serial and three OpenMP variants) vs threads.

Expects:
- One parallel scaling log like hist_scaling-*.out containing repeated blocks:
    Threads: <t>
    ...
    Time Merge Critical: <sec> sec
    Time Merge Out: <sec> sec
    Time Atomic: <sec> sec
- One serial log like hist_seq-*.out containing a line:
    Time: <sec> sec
"""
from __future__ import annotations

import re
from pathlib import Path
import sys

import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
LOG_PAR = sorted(BASE.glob("hist_scaling-*.out"))
LOG_SEQ = sorted(BASE.glob("hist_seq-*.out"))

if not LOG_PAR:
    print("No hist_scaling-*.out found", file=sys.stderr)
    sys.exit(1)

par_path = LOG_PAR[-1]

threads_re = re.compile(r"^Threads:\s*(\d+)")
crit_re = re.compile(r"^Time Merge Critical:\s*([0-9.]+) sec")
merge_re = re.compile(r"^Time Merge Out:\s*([0-9.]+) sec")
atomic_re = re.compile(r"^Time Atomic:\s*([0-9.]+) sec")

times = {}
cur_t = None
with par_path.open() as fh:
    for line in fh:
        line = line.strip()
        m = threads_re.match(line)
        if m:
            cur_t = int(m.group(1))
            times.setdefault(cur_t, {})
            continue
        if cur_t is None:
            continue
        m = crit_re.match(line)
        if m:
            times[cur_t]["crit"] = float(m.group(1))
            continue
        m = merge_re.match(line)
        if m:
            times[cur_t]["merge"] = float(m.group(1))
            continue
        m = atomic_re.match(line)
        if m:
            times[cur_t]["atomic"] = float(m.group(1))
            continue

# Serial baseline
serial_time = None
if LOG_SEQ:
    ser_path = LOG_SEQ[-1]
    m = re.search(r"Time:\s*([0-9.]+) sec", ser_path.read_text())
    if m:
        serial_time = float(m.group(1))

threads = sorted(times)
crit = [times[t]["crit"] for t in threads]
merge = [times[t]["merge"] for t in threads]
atomic = [times[t]["atomic"] for t in threads]

plt.figure(figsize=(6, 4))
plt.plot(threads, crit, marker="o", label="private + critical")
plt.plot(threads, merge, marker="s", label="private + merge-out")
plt.plot(threads, atomic, marker="^", label="atomic")

if serial_time is not None:
    plt.plot([threads[0], threads[-1]], [serial_time, serial_time],
             linestyle="dashed", color="black",
             label=f"serial ({serial_time:.3g}s)")

plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.title("Histogram runtime vs threads")
plt.xticks(threads, threads)

# Use logarithmic Y axis so 'atomic' does not dwarf the others
plt.yscale("log")
ax = plt.gca()
ax.grid(True, which="major", linestyle="--", alpha=0.3)
ax.grid(True, which="minor", linestyle=":", alpha=0.2)
plt.legend(fontsize="small")
plt.tight_layout()

out_dir = BASE / "plots"
out_dir.mkdir(exist_ok=True)
out_path = out_dir / "hist_runtime_scaling.png"
plt.savefig(out_path, dpi=150)
print(f"Saved {out_path}")
