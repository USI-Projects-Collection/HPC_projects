#!/bin/bash

cd ./test_val
sbatch test.sh

cd ../test_perf
sbatch test.sh

cd ../test_scale
sbatch test.sh

cd ..
