/*
test_mkl.cpp
HPC Class: Tests if the MKL library is correctly installed and functional.
Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

Run Instructions:
- Load module: module load intel-oneapi-mkl
- Compile: g++ -fopenmp -I${MKLROOT}/include test_mkl.cpp -L${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -o test_mkl
- Allocate (SLURM): salloc -N 1 --time=00:05:00 --reservation=hpc-tuesday 
- Execute: ./main.exe

TODO:
1. Create a Makefile.
2. Create an sbatch script.
*/

#include <iostream>
#include <mkl.h>

int main() {
  const int n = 3;
  double A[n*n] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  double B[n*n] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
  double C[n*n] = {0.0};
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);

  std::cout << "Intel MKL is working. Matrix C (result of A*B):" << std::endl;
  for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {std::cout << C[i*n + j] << " ";}
      std::cout << std::endl;
  }
  return 0;
}
