#!/bin/bash

#SBATCH --job-name=py_mandel
#SBATCH --output=mandel_scaling_%j.out
#SBATCH --error=mandel_scaling_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=17 
#SBATCH --time=00:20:00

# Attiva l'environment conda se necessario
source /apps/miniconda3/bin/activate
conda activate project5_env

module unload openmpi

NX=4001
NY=4001

echo "### Scaling Study for Mandelbrot Manager-Worker ###"
echo "Grid: $NX x $NY"

for TASKS in 50 100; do
    echo "========================================"
    echo "Running with $TASKS tasks split"
    echo "========================================"
    echo "Workers, Time" > scaling_tasks_${TASKS}.csv
    
    for P in 3 5 9 17; do
        WORKERS=$((P-1))
        echo "Running with $WORKERS workers (Total Procs: $P)..."
        
        OUTPUT=$(mpiexec -n $P python manager_worker.py $NX $NY $TASKS)
        
        TIME=$(echo "$OUTPUT" | grep "Run took" | awk '{print $3}')
        
        echo "Time: $TIME seconds"
        echo "$WORKERS, $TIME" >> scaling_tasks_${TASKS}.csv
    done
done