#!/bin/bash

cd ./test_perf
./test.sh

cd ../test_scale
./test.sh

cd ../test_val
./test.sh

cd ..
