import re
import matplotlib.pyplot as plt

# Use weak scaling results
logfile = "weak_scaling_results.txt"

# Regex for extracting: ### threads, n, nt, cg_iter, newton_iter, time ###
pattern = re.compile(r"### (\d+), (\d+), \d+, \d+, \d+, ([\d.]+) ###")

# Data grouped by base resolution: {base_resolution: [(threads, n, time)]}
data = {}

with open(logfile, "r") as f:
    current_base = None
    for line in f:
        if "Base resolution:" in line:
            current_base = int(line.strip().split('=')[1])
        m = pattern.search(line)
        if m and current_base is not None:
            threads = int(m.group(1))
            n = int(m.group(2))
            time = float(m.group(3))
            data.setdefault(current_base, []).append((threads, n, time))

# Sort by threads
for base in data:
    data[base].sort(key=lambda x: x[0])

# --- Plot 1: Time vs Threads ---
plt.figure(figsize=(8,6))
for base, values in sorted(data.items()):
    threads, ns, times = zip(*values)
    plt.plot(threads, times, marker='o', label=f"base n={base}")
    for t, n, time in values:
        plt.text(t + 0.1, time, f"{n}x{n}", fontsize=8, verticalalignment='bottom', horizontalalignment='left')

plt.yscale("log")
plt.xlabel("Number of Threads (NCPU)")
plt.ylabel("Time to Solution [s]")
plt.title("Weak Scaling: Time vs Threads for Different Base Resolutions")
plt.legend()
plt.grid(True, linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("weak_scaling_time_vs_threads.png", dpi=300)

# --- Plot 2: Efficiency ---
plt.figure(figsize=(8,6))
for base, values in sorted(data.items()):
    threads, ns, times = zip(*values)
    t1 = times[0]
    efficiency = [t1 / t for t in times]
    plt.plot(threads, efficiency, marker='o', label=f"base n={base}")
    for t, n, eff in zip(threads, ns, efficiency):
        plt.text(t + 0.1, eff, f"{n}x{n}", fontsize=8, verticalalignment='bottom', horizontalalignment='left')

plt.xlabel("Number of Threads (NCPU)")
plt.ylabel("Weak Scaling Efficiency (T1/Tn)")
plt.title("Weak Scaling Efficiency per Base Resolution")
plt.legend()
plt.grid(True, linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("weak_scaling_efficiency.png", dpi=300)

print("Plots saved: weak_scaling_time_vs_threads.png, weak_scaling_efficiency.png")