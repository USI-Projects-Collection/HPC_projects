#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

int main() {
#ifdef _OPENMP
    #pragma omp parallel
    {
        if (omp_get_thread_num() == 0) {
            std::cout << "OpenMP threads=" << omp_get_num_threads() << std::endl;
        }
}

#else
    std::cout << "OpenMP is not available." << std::endl;
#endif

    return 0;
}
