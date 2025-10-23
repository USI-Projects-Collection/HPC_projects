#!/usr/bin/env python3
"""Plot runtime scaling for recurrence example (chunk vs dynamic)."""
import re
from pathlib import Path
import matplotlib.pyplot as plt

LOG_DIR = Path(__file__).resolve().parent / "logs"
PAR_PATTERN = re.compile(r"recur_par_T(\d+)\.log")
SEQ_FILE = LOG_DIR / "recur_seq_T1.log"

records = []
for log in sorted(LOG_DIR.glob("recur_par_T*.log")):
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
    if chunk is not None and dyn is not None:
        records.append((threads, chunk, dyn))

if not records:
    raise SystemExit("No recur_par_T*.log files found in logs/")

records.sort()
threads = [r[0] for r in records]
chunk_times = [r[1] for r in records]
dyn_times = [r[2] for r in records]

serial_time = None
if SEQ_FILE.exists():
    with SEQ_FILE.open() as fh:
        for line in fh:
            if "Sequential RunTime" in line:
                serial_time = float(line.split(":")[1].split()[0])
                break

plt.figure(figsize=(6, 4))
plt.plot(threads, chunk_times, marker="o", label="Chunk division")
plt.plot(threads, dyn_times, marker="s", label="Dynamic with check")

if serial_time is not None:
    plt.plot([threads[0], threads[-1]], [serial_time, serial_time],
             linestyle="dashed", color="black",
             label=f"Serial baseline ({serial_time:.3f}s)")

plt.xscale("log", base=2)
plt.yscale("log")
plt.xlabel("Threads")
plt.ylabel("Time [s]")
plt.title("Recurrence runtime vs threads")
plt.xticks(threads, [str(t) for t in threads])
plt.grid(True, which="major", linestyle="--", alpha=0.3)
plt.grid(True, which="minor", linestyle=":", alpha=0.2)
plt.legend(fontsize="small")
plt.tight_layout()

out_dir = LOG_DIR.parent / "plots"
out_dir.mkdir(exist_ok=True)
out_path = out_dir / "recur_runtime_scaling.png"
plt.savefig(out_path, dpi=150)
print(f"Saved {out_path}")
