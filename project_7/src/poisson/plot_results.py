#!/usr/bin/env python3
"""
Generate log-log plots for Performance Benchmark and Strong Scaling
"""

import matplotlib.pyplot as plt
import numpy as np

# ============================================================================
# 1.4 Performance Benchmark - Solve time vs Grid size
# ============================================================================

grid_sizes = np.array([8, 16, 32, 64, 128, 256, 512])
n_squared = grid_sizes ** 2  # Total unknowns

# Solve times from test_perf output
scipy_direct = np.array([0.000320, 0.000835, 0.003371, 0.014570, 0.074122, 0.442920, 3.040594])
scipy_dense = np.array([0.000537, 0.001324, 0.041605, 1.808445, 99.097132, np.nan, np.nan])
scipy_cg = np.array([0.000501, 0.001025, 0.002536, 0.009114, 0.048808, 0.357634, 3.225408])
petsc = np.array([0.000126, 0.000092, 0.000476, 0.003308, 0.025979, 0.207763, 1.948708])

fig1, ax1 = plt.subplots(figsize=(10, 7))

ax1.loglog(grid_sizes, scipy_direct, 'o-', label='SciPy Direct (sparse)', linewidth=2, markersize=8)
ax1.loglog(grid_sizes[:5], scipy_dense[:5], 's-', label='SciPy Dense Direct', linewidth=2, markersize=8)
ax1.loglog(grid_sizes, scipy_cg, '^-', label='SciPy CG (sparse)', linewidth=2, markersize=8)
ax1.loglog(grid_sizes, petsc, 'd-', label='PETSc (1 proc)', linewidth=2, markersize=8)

# Reference slopes
x_ref = np.array([32, 512])
ax1.loglog(x_ref, 1e-4 * (x_ref/32)**2, 'k--', alpha=0.5, label=r'$\mathcal{O}(n^2)$')
ax1.loglog(x_ref, 1e-4 * (x_ref/32)**3, 'k:', alpha=0.5, label=r'$\mathcal{O}(n^3)$')

ax1.set_xlabel('Grid size $n$', fontsize=14)
ax1.set_ylabel('Solve time (seconds)', fontsize=14)
ax1.set_title('Performance Benchmark: Solve Time vs Grid Size', fontsize=16)
ax1.legend(fontsize=11, loc='upper left')
ax1.grid(True, which='both', alpha=0.3)
ax1.set_xticks(grid_sizes)
ax1.set_xticklabels(grid_sizes)

plt.tight_layout()
plt.savefig('test_perf/benchmark_loglog.png', dpi=150, bbox_inches='tight')
plt.savefig('test_perf/benchmark_loglog.pdf', bbox_inches='tight')
print("Saved: test_perf/benchmark_loglog.png")

# ============================================================================
# 1.5 Strong Scaling - Speedup and Efficiency vs Processes
# ============================================================================

processes = np.array([1, 2, 4, 8, 16])
solve_times = np.array([18.679978, 10.778899, 5.306495, 3.462232, 1.808753])

speedup = solve_times[0] / solve_times
efficiency = speedup / processes * 100
ideal_speedup = processes

fig2, (ax2a, ax2b) = plt.subplots(1, 2, figsize=(14, 6))

# Speedup plot (log-log)
ax2a.loglog(processes, speedup, 'bo-', linewidth=2, markersize=10, label='Measured')
ax2a.loglog(processes, ideal_speedup, 'k--', linewidth=2, label='Ideal (linear)')
ax2a.set_xlabel('Number of MPI processes', fontsize=14)
ax2a.set_ylabel('Speedup', fontsize=14)
ax2a.set_title('Strong Scaling: Speedup', fontsize=16)
ax2a.legend(fontsize=12)
ax2a.grid(True, which='both', alpha=0.3)
ax2a.set_xticks(processes)
ax2a.set_xticklabels(processes)
ax2a.set_yticks(processes)
ax2a.set_yticklabels(processes)

# Efficiency plot
ax2b.semilogx(processes, efficiency, 'ro-', linewidth=2, markersize=10)
ax2b.axhline(y=100, color='k', linestyle='--', linewidth=2, label='Ideal (100%)')
ax2b.set_xlabel('Number of MPI processes', fontsize=14)
ax2b.set_ylabel('Parallel Efficiency (%)', fontsize=14)
ax2b.set_title('Strong Scaling: Efficiency', fontsize=16)
ax2b.set_ylim([0, 110])
ax2b.legend(fontsize=12)
ax2b.grid(True, which='both', alpha=0.3)
ax2b.set_xticks(processes)
ax2b.set_xticklabels(processes)

plt.tight_layout()
plt.savefig('test_scale/scaling_loglog.png', dpi=150, bbox_inches='tight')
plt.savefig('test_scale/scaling_loglog.pdf', bbox_inches='tight')
print("Saved: test_scale/scaling_loglog.png")

# ============================================================================
# Print summary tables
# ============================================================================

print("\n" + "="*60)
print("Performance Benchmark Summary")
print("="*60)
print(f"{'Grid':<8} {'SciPy Dir':<12} {'SciPy Dense':<12} {'SciPy CG':<12} {'PETSc':<12}")
for i, n in enumerate(grid_sizes):
    dense_str = f"{scipy_dense[i]:.6f}" if not np.isnan(scipy_dense[i]) else "---"
    print(f"{n:<8} {scipy_direct[i]:<12.6f} {dense_str:<12} {scipy_cg[i]:<12.6f} {petsc[i]:<12.6f}")

print("\n" + "="*60)
print("Strong Scaling Summary (1024x1024 grid)")
print("="*60)
print(f"{'Procs':<8} {'Time (s)':<12} {'Speedup':<10} {'Efficiency':<12}")
for i, p in enumerate(processes):
    print(f"{p:<8} {solve_times[i]:<12.4f} {speedup[i]:<10.2f} {efficiency[i]:<10.1f}%")

# plt.show()
