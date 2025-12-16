#!/bin/bash

echo "Submitting validation test..."
cd ./test_val
sbatch test.sh

echo "Submitting performance test..."
cd ../test_perf
sbatch test.sh

echo "Submitting scaling test..."
cd ../test_scale
sbatch test.sh

cd ..
echo "All jobs submitted. Use 'squeue' to check status."
