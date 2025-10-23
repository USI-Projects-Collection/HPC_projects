#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "walltime.h"
#include <omp.h>

int main(int argc, char *argv[]) {
  int N = 2000000000;
  double up = 1.00000001;
  double Sn = 1.00000001;
  int n;

  /* allocate memory for both parallel models */
  double *opt_chunk = (double *)malloc((N + 1) * sizeof(double));
  double *opt_dyn   = (double *)malloc((N + 1) * sizeof(double));
  if (opt_chunk == NULL || opt_dyn == NULL) {
    perror("failed to allocate problem size");
    exit(EXIT_FAILURE);
  }

  // ----------- Chunk Division (Manual) -----------
  double time_start_chunk = walltime();
  #pragma omp parallel firstprivate(Sn)
  {
    int num_threads = omp_get_max_threads();
    int tid = omp_get_thread_num();
    int chunk_size = (N + 1) / num_threads;
    int start = tid * chunk_size;
    int end = (tid == num_threads - 1) ? (N + 1) : ((tid + 1) * chunk_size);
    double Sn_local = Sn * pow(up, start);
    
    for (int n = start; n < end; ++n) {
      opt_chunk[n] = Sn_local;
      Sn_local *= up;
    }
  }
  double time_chunk = walltime() - time_start_chunk;

  // ----------- Dynamic Schedule with Check -----------
  double time_start_check = walltime();
  #pragma omp parallel firstprivate(Sn)
  #pragma omp for schedule(dynamic) lastprivate(Sn)
  for (int i = 0; i <= N; ++i) {
    double Sn_local = Sn;
    int last_i = -2;
    if (i != last_i + 1) {
      Sn_local = Sn * pow(up, i);
    }
    opt_dyn[i] = Sn_local;
    Sn_local *= up;
    last_i = i;
  }
  double time_check = walltime() - time_start_check;


  // non toccare questo!!!
  double temp = 0.0;
  for (n = 0; n <= N; ++n) {
    temp += opt_chunk[n] * opt_chunk[n];
  }

  printf("Number of threads: %d\n", omp_get_max_threads());
  printf("Parallel RunTime (Chunk Division): %.6f seconds\n", time_chunk);
  printf("Parallel RunTime (Dynamic Check): %.6f seconds\n", time_check);
  printf("Chunk model:   N=%d, opt[N]=%.17g, ||opt||^2_2=%f\n", N, opt_chunk[N], temp / (double)N);
  printf("Dynamic model: N=%d, opt[N]=%.17g, ||opt||^2_2=%f\n", N, opt_dyn[N], temp / (double)N);
  printf("\n");

  return 0;
}
