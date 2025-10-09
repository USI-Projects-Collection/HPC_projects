// dgemm-blocked-omp.c

#include <stddef.h>

const char* dgemm_desc = "Blocked dgemm + OpenMP";

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4
#endif

static inline void do_block(int n, int si, int sj, int sk, int bs,
                            const double* A,
                            const double* B,
                            double* C) {

  int i_max = si + bs; if (i_max > n) i_max = n; // evita overflow
  int j_max = sj + bs; if (j_max > n) j_max = n;
  int k_max = sk + bs; if (k_max > n) k_max = n;

  for (int j = sj; j < j_max; ++j) {
    for (int i = si; i < i_max; ++i) {
      double cij = C[i + (size_t)j*n];

      // vettorizza questo loop
      #pragma omp simd
      for (int k = sk; k < k_max; ++k) {
        cij += A[i + (size_t)k*n] * B[k + (size_t)j*n];
      }

      C[i + (size_t)j*n] = cij;

    }
  }
}

void square_dgemm(int n, double* A, double* B, double* C) {
  const int bs = BLOCK_SIZE;

  // Parallelizza i loop di blocco (coprono quasi tutto il lavoro)
  // collapse(2) = spartisce (i,j) tra i thread
  #pragma omp parallel for collapse(2) schedule(static)
  for (int i = 0; i < n; i += bs) {
    for (int j = 0; j < n; j += bs) {
      for (int k = 0; k < n; k += bs) {
        do_block(n, i, j, k, bs, A, B, C);
      }
    }
  }
}