import re
import numpy as np
import matplotlib.pyplot as plt

# ----------------------------------------------------------
# Utility function: read data
# ----------------------------------------------------------
def read_data(path):
    P = []
    N = []
    iters = []
    theta = []
    times = []
    # This regex matches the output format from your C code
    pattern = re.compile(
        r"###\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*([0-9Ee+\-\.]+)\s*,\s*([0-9Ee+\-\.]+)\s*###"
    )
    try:
        with open(path) as f:
            for line in f:
                m = pattern.search(line)
                if m:
                    P.append(int(m.group(1)))
                    N.append(int(m.group(2)))
                    iters.append(int(m.group(3)))
                    theta.append(float(m.group(4)))
                    times.append(float(m.group(5)))
    except FileNotFoundError:
        print(f"Warning: File {path} not found. Generating empty data.")
        
    return {
        "P": np.array(P),
        "time": np.array(times),
    }

# ----------------------------------------------------------
# Efficiency Calculation
# Strong: E = T(1) / (P * T(P))
# Weak:   E = T(1) / T(P)
# ----------------------------------------------------------
def efficiency(data, kind="strong"):
    P = data["P"]
    T = data["time"]
    
    if len(P) == 0:
        return np.array([])
        
    if 1 not in P:
        # Fallback if P=1 is missing (though scaling studies usually require it)
        print("Warning: Dataset does not include P=1. Efficiency cannot be normalized to T(1).")
        return np.zeros_like(T)
        
    T1 = float(T[P == 1][0]) # Take the first occurrence if multiple exist
    
    if kind == "strong":
        # Strong Scaling: Speedup / P
        E = T1 / (P * T)
    elif kind == "weak":
        # Weak Scaling: Ideal efficiency is 1.0 (constant time)
        E = T1 / T
    else:
        raise ValueError("Efficiency kind must be 'strong' or 'weak'")
        
    return E

# ----------------------------------------------------------
# LOAD DATA
# ----------------------------------------------------------
# Ensure these paths match your actual directory structure
strong_single = read_data("std/strong_single.data")
strong_multi  = read_data("std/strong_multi.data")
weak_single   = read_data("std/weak_single.data")
weak_multi    = read_data("std/weak_multi.data")

# ----------------------------------------------------------
# Helper: create combined runtime+efficiency plot
# ----------------------------------------------------------
def make_combined_plot(d1, d2, label1, label2, title, outfile, kind="strong"):
    P1, T1 = d1["P"], d1["time"]
    P2, T2 = d2["P"], d2["time"]

    # Compute efficiency based on the scaling type (strong vs weak)
    E1 = efficiency(d1, kind=kind)
    E2 = efficiency(d2, kind=kind)

    if len(P1) == 0 or len(P2) == 0:
        print(f"Skipping plot {outfile} due to missing data.")
        return

    fig, ax1 = plt.subplots(figsize=(9, 6))

    # Runtime curves (left axis)
    ln1 = ax1.plot(P1, T1, "o-", label=f"{label1} runtime")
    ln2 = ax1.plot(P2, T2, "o-", label=f"{label2} runtime")
    ax1.set_xlabel("MPI processes (P)")
    ax1.set_ylabel("Runtime (s)")
    ax1.grid(True)
    
    # Optional: Set Y-limit for runtime to start at 0 for better perspective
    # ax1.set_ylim(bottom=0)

    # Efficiency curves (right axis)
    ax2 = ax1.twinx()
    ln3 = ax2.plot(P1, E1, "s--", label=f"{label1} efficiency")
    ln4 = ax2.plot(P2, E2, "s--", label=f"{label2} efficiency")
    ax2.set_ylabel("Efficiency")
    ax2.set_ylim(0, 1.1) # Efficiency typically maxes at 1.0 (or slightly above due to noise)

    # Combine legends from both axes
    lines = ln1 + ln2 + ln3 + ln4
    labs = [l.get_label() for l in lines]
    ax1.legend(lines, labs, loc="center right")

    plt.title(title)
    plt.tight_layout()
    plt.savefig(outfile, dpi=150)
    plt.close()
    print(f"Saved plot to {outfile}")

# ----------------------------------------------------------
# GENERATE FINAL TWO FIGURES
# ----------------------------------------------------------

# 1. Strong Scaling (uses kind="strong")
make_combined_plot(
    strong_single, strong_multi,
    "Strong single-node", "Strong multi-node",
    "Strong Scaling – Runtime + Efficiency (Combined)",
    "strong_scaling_combined.png",
    kind="strong"
)

# 2. Weak Scaling (uses kind="weak")
make_combined_plot(
    weak_single, weak_multi,
    "Weak single-node", "Weak multi-node",
    "Weak Scaling – Runtime + Efficiency (Combined)",
    "weak_scaling_combined.png",
    kind="weak"
)