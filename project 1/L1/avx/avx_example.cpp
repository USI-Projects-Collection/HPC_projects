/*
avx_example.cpp
HPC Class: Example of using AVX intrinsics.
Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

Compile Instructions:
- Compile: g++ -mavx -o main.exe avx_example.cpp
- Allocate (SLURM): salloc -N 1 --time=00:05:00 --reservation=hpc-monday
- Execute: ./main.exe

TODO:
1. How many floats can fit in the AVX register?
2. What if we used doubles instead of floats? (you need to use type "__m256d" swap the functions with prefex "pd" ie _mm256_loadu_pd,_mm256_storeu_pd,_mm256_add_pd )
*/

#include <iostream>
#include <immintrin.h>  // For AVX intrinsics

int main() {
    float a[8] = {1,2,3,4,5,0,0,0};
    float b[8] = {5,6,7,8,9,0,0,0};
    float c[8];

    __m256 v_a = _mm256_loadu_ps(a);
    __m256 v_b = _mm256_loadu_ps(b);
    __m256 v_c = _mm256_add_ps(v_a, v_b);
    _mm256_storeu_ps(c, v_c);

    for (int i = 0; i < 8; ++i) std::cout << c[i] << ' ';
    std::cout << '\n';
    return 0;
}