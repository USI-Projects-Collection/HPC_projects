#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  long long N = 10000000000LL; // default integration intervals
  if (argc > 1) {
    long long tmp = atoll(argv[1]);
    if (tmp > 0) {
      N = tmp;
    }
  }
  double dx = 1.0 / static_cast<double>(N);
  double sum = 0.0;
  double pi = 0.0;
  double time = 0.0;

  /* Serial version of computing pi */
  sum = 0.0;
  time = omp_get_wtime();
  for (long long i = 0; i < N; ++i) {
    double xi = (static_cast<double>(i) + 0.5) * dx;
    sum += 4.0 / (1.0 + xi * xi);
  }
  pi = sum * dx;
  time = omp_get_wtime() - time;
  printf("pi=%0.12f, N=%lld, time_srl=%e secs\n", pi, N, time);

  /* Parallel version of computing pi */
  time = omp_get_wtime();
  sum = 0.;
  #pragma omp parallel for reduction(+ : sum)
  for (long long i = 0; i < N; ++i) {
    double xi = (static_cast<double>(i) + 0.5) * dx;
    sum += 4.0 / (1.0 + xi * xi);
  }
  pi = sum * dx;
  time = omp_get_wtime() - time;
  printf("pi=%0.12f, N=%lld, time_plr=%e secs\n", pi, N, time);

  return 0;
}
