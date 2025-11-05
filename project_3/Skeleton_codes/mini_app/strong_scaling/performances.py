import re
import matplotlib.pyplot as plt

# Percorso al file dei risultati
logfile = "strong_scaling_results.txt"

# Regex per estrarre i dati delle righe con formato:
# ### threads, n, nt, cg_iter, newton_iter, time ###
pattern = re.compile(r"### (\d+), (\d+), \d+, \d+, \d+, ([\d.]+) ###")

data = {}  # {n: [(threads, time), ...]}

with open(logfile, "r") as f:
    for line in f:
        m = pattern.search(line)
        if m:
            threads = int(m.group(1))
            n = int(m.group(2))
            time = float(m.group(3))
            data.setdefault(n, []).append((threads, time))

# Ordina i dati per numero di thread
for n in data:
    data[n].sort(key=lambda x: x[0])

# --- Plot: tempo vs numero di thread per ogni n ---
plt.figure(figsize=(8,6))

for n, values in sorted(data.items()):
    threads, times = zip(*values)
    plt.plot(threads, times, marker='o', label=f"n={n}")

plt.yscale("log")
plt.xlabel("Number of Threads (NCPU)")
plt.ylabel("Time to Solution [s]")
plt.title("Strong Scaling: Time vs Threads for Different Grid Sizes")
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("strong_scaling_time_vs_threads.png", dpi=300)



# Speedup = tempo_1_thread / tempo_N_thread
plt.figure(figsize=(8,6))
for n, values in sorted(data.items()):
    values.sort()
    threads, times = zip(*values)
    t1 = times[0]
    speedup = [t1 / t for t in times]
    plt.plot(threads, speedup, marker='o', label=f"n={n}")

# plt.xscale("log", base=2)
plt.xlabel("Number of Threads (NCPU)")
plt.ylabel("Speedup")
plt.title("Strong Scaling Speedup (T1/Tn)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("strong_scaling_speedup.png", dpi=300)