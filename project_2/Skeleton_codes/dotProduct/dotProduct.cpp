#include <omp.h>
#include "walltime.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.008s ~0.8MB
// #define N 100000
// 0.1s ~8MB
// #define N 1000000
// 1.1s ~80MB
// #define N 10000000
// 13s ~800MB
// #define N 100000000
// 127s 16GB
//#define N 1000000000
#define EPSILON 0.1

using namespace std;

int main() {
  double time_serial, time_start = 0.0;
  double *a, *b;

  // Allocate memory for the vectors as 1-D arrays
  a = new double[N];
  b = new double[N];

  // Initialize the vectors with some values
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = i / 10.0;
  }

  long double alpha = 0;
  // serial execution
  // Note that we do extra iterations to reduce relative timing overhead
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha = 0.0;
    for (int i = 0; i < N; i++) {
      alpha += a[i] * b[i];
    }
  }
  time_serial = wall_time() - time_start;
  cout << "Serial execution time = " << time_serial << " sec" << endl;

  long double alpha_reduction = 0.0;
  long double alpha_critical = 0.0;
  double time_red = 0.0;
  double time_critical = 0.0;

  // Parallel dot product using reduction pragma
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    long double alpha_tmp = 0.0;
#pragma omp parallel for reduction(+ : alpha_tmp)
    for (int i = 0; i < N; i++) {
      alpha_tmp += a[i] * b[i];
    }
    alpha_reduction = alpha_tmp;
  }
  time_red = wall_time() - time_start;

  // Parallel dot product using critical pragma
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    long double alpha_tmp = 0.0;
#pragma omp parallel
    {
      long double thread_partial = 0.0;
#pragma omp for
      for (int i = 0; i < N; i++) {
        thread_partial += a[i] * b[i];
      }
#pragma omp critical
      { alpha_tmp += thread_partial; }
    }
    alpha_critical = alpha_tmp;
  }
  time_critical = wall_time() - time_start;

  long double denom = fabsl(alpha) > 0.0L ? fabsl(alpha) : 1.0L;
  if ((fabsl(alpha_reduction - alpha) / denom) > EPSILON) {
    cout << "parallel reduction: " << alpha_reduction
         << ", serial: " << alpha << "\n";
    cerr << "Reduction-based alpha not implemented correctly!\n";
    exit(1);
  }
  if ((fabsl(alpha_critical - alpha) / denom) > EPSILON) {
    cout << "parallel critical: " << alpha_critical
         << ", serial: " << alpha << "\n";
    cerr << "Critical-based alpha not implemented correctly!\n";
    exit(1);
  }

  cout << "Parallel reduction dot product = " << alpha_reduction
       << ", time = " << time_red << " sec" << endl;
  cout << "Parallel critical dot product = " << alpha_critical
       << ", time = " << time_critical << " sec" << endl;

  // De-allocate memory
  delete[] a;
  delete[] b;

  return 0;
}
