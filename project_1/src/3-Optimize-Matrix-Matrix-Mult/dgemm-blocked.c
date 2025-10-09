const char* dgemm_desc = "Blocked dgemm.";

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 2
#endif

/* This routine performs a dgemm operation on a block
 *  C := C + A * B
 * where A, B, and C are small blocks that fit in cache
 */
static inline void do_block(int n, int si, int sj, int sk, int block_size,
                           double* A, double* B, double* C) {
  int i_max = si + block_size;
  int j_max = sj + block_size;
  int k_max = sk + block_size;

  i_max = (i_max > n) ? n : i_max;
  j_max = (j_max > n) ? n : j_max;
  k_max = (k_max > n) ? n : k_max;

  for (int i = si; i < i_max; ++i) {
    for (int j = sj; j < j_max; ++j) {
      double cij = C[i + j*n];
      for (int k = sk; k < k_max; ++k) {
        cij += A[i + k*n] * B[k + j*n];
      }
      C[i + j*n] = cij;
    }
  }
}

void square_dgemm(int n, double* A, double* B, double* C) {
  int block_size = BLOCK_SIZE;

  for (int i = 0; i < n; i += block_size) {
    for (int j = 0; j < n; j += block_size) {
      for (int k = 0; k < n; k += block_size) {
        do_block(n, i, j, k, block_size, A, B, C);
      }
    }
  }
}
