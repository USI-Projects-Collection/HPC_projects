#include <iostream>
#include <omp.h>
#include <random>
#include <vector>
#include "walltime.h"

#define VEC_SIZE 1000000000
#define BINS 16

int main() {
  double time_start_atomic, time_end_atomic, time_start_critical, time_end_critical, time_start_mergeOut, time_end_mergeOut;

  // Initialize random number generator
  unsigned int seed = 123;
  float mean = BINS / 2.0;
  float sigma = BINS / 12.0;
  std::default_random_engine generator(seed);
  std::normal_distribution<float> distribution(mean, sigma);

  // Generate random sequence
  // Note: normal distribution is on interval [-inf; inf]
  //       we want [0; BINS-1]
  std::vector<int> vec(VEC_SIZE);
  for (long i = 0; i < VEC_SIZE; ++i) {
    vec[i] = int(distribution(generator));
    if (vec[i] < 0       ) vec[i] = 0;
    if (vec[i] > BINS - 1) vec[i] = BINS - 1;
  }

  // Initialize histogram: Set all bins to zero
  long dist_atomic[BINS], dist_crit[BINS], dist_mergeOut[BINS];
  for (int i = 0; i < BINS; ++i) {
    dist_atomic[i] = 0;
    dist_crit[i] = 0;
    dist_mergeOut[i] = 0;
  }

  // with private histograms for each thread ===================
  time_start_critical = walltime();
  #pragma omp parallel 
  {
    long dist_private[BINS];
    for (int i = 0; i < BINS; ++i) {
      dist_private[i] = 0;
    }

    #pragma omp for
    for (long i = 0; i < VEC_SIZE; ++i) {
      dist_private[vec[i]]++;
    }

    #pragma omp critical 
    {
      for (int i = 0; i < BINS; ++i) {
        dist_crit[i] += dist_private[i];
      }
    }
  }
  time_end_critical = walltime();

  // with private histograms and merge outside ================
  time_start_mergeOut = walltime();

  int num_threads = omp_get_max_threads();
  std::vector<std::vector<long>> all_private_dists(num_threads, std::vector<long>(BINS, 0));

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    std::vector<long>& dist_private = all_private_dists[tid];
    for (int i = 0; i < BINS; ++i) {
      dist_private[i] = 0;
    }

    #pragma omp for
    for (long i = 0; i < VEC_SIZE; ++i) {
      dist_private[vec[i]]++;
    }
  }

  // merge outside serially
  for (int i = 0; i < BINS; ++i) {
    dist_mergeOut[i] = 0;
    for (int t = 0; t < num_threads; ++t) {
      dist_mergeOut[i] += all_private_dists[t][i];
    }
  }

  time_end_mergeOut = walltime();

  // with atomic updates ========================================
  time_start_atomic = walltime();

  #pragma omp parallel for
  for (long i = 0; i < VEC_SIZE; ++i) {
    #pragma omp atomic
    dist_atomic[vec[i]]++;
  }
  time_end_atomic = walltime();


  // Write results
  for (int i = 0; i < BINS; ++i) {
    std::cout << 'i' << ' | ' << 'crit' << ' | ' << 'mergeOut' << " | " << 'atomic' << std::endl;
    std::cout << i << " | " << dist_crit[i] << " | " << dist_mergeOut[i] << " | " << dist_atomic[i] << std::endl;
  }
  std::cout << "Time Merge Critical: " << time_end_critical - time_start_critical << " sec" << std::endl;
  std::cout << "Time Merge Out: " << time_end_mergeOut - time_start_mergeOut << " sec" << std::endl;
  std::cout << "Time Atomic: " << time_end_atomic - time_start_atomic << " sec" << std::endl;


  return 0;
}
