#!/bin/bash

cd ./test_val
python3 ../draw.py solution_petsc
python3 ../draw.py solution_sp_dir
python3 ../draw.py solution_dn_dir
python3 ../draw.py solution_sp_cg

cd ../test_perf
python3 ../draw.py solution_petsc
python3 ../draw.py solution_sp_dir
python3 ../draw.py solution_sp_cg

cd ../test_scale
python3 ../draw.py solution_petsc

cd ..
echo "All plots generated!"