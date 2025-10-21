#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  long int N = 10000000000; // <<< carefull! This larger than regular int (its a long int)
  double dx = 1./double(N);
  double sum=0;
  double pi =0;
  double time=0;


  /* TODO Serial Version of Computing Pi*/
  time = omp_get_wtime();
  // TODO Fill this gap
  pi = sum * dx;
  time = omp_get_wtime() - time;
  printf("pi=%e, N=%ld, time_srl=%e secs\n", pi, N, time);


  /* TODO Parallel Version of Computing Pi*/
  time = omp_get_wtime();
  sum = 0.;
  // TODO Fill this gap
  pi = sum*dx;
  time = omp_get_wtime() - time;
  printf("pi=%e, N=%ld, time_plr=%e secs\n", pi, N, time);

  return 0;
}
