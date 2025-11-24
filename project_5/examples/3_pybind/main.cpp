/*
 * HPC Assignment: Pybind11 Factor Finder and OpenMP Hello Module
 * Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>
 *
 * Description:
 * This module uses Pybind11 to create a C++ extension for Python, exposing two primary functions:
 * - `find_factor`: Finds and returns the largest nontrivial factor of a given unsigned integer `X`.
 *   If no nontrivial factors are found, the function returns 1 (the trivial factor). The function 
 *   is also vectorized with Pybind11 to operate element-wise on numpy arrays.
 * - `openmp_hello`: Demonstrates OpenMP parallelism by printing a message from each thread, indicating 
 *   the thread number and total thread count. This function serves as a basic example to verify 
 *   OpenMP integration within a Python-accessible module.
 *
 * Compilation Instructions:
 * To compile this code as a Python module with OpenMP support, use the provided makefile.
 *
 * Run Instructions:
 * Import and use this module within Python as follows:
 * ```python
 * import MOD_NAME
 * result = MOD_NAME.find_factor(28)  # Finds the largest nontrivial factor of 28
 * array_result = MOD_NAME.find_factor(numpy.array([28, 30, 31]))  # Factors array elements
 * MOD_NAME.openmp_hello()  # Prints a message from each OpenMP thread
 * ```
 *
 * TODO:
 * 1) Compile the code and load the module. What is the module name? (you can change it by recompiling)
 * 2) Try running `openmp_hello`. How would you change the number of threads?
 * 3) Try running `find_factor` for `4294967295` and then `4294967296`. Comment on any observed errors.
 * 4) Test the vectorized form of `find_factor` with a list of numbers.
 * 5) See the help/readme of a function... in python `help(MOD_NAME.find_factor)`.
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <iostream>
#include <omp.h>

namespace py = pybind11;

int find_factor(unsigned int X) {
    for(unsigned int i = X-1; i > 0; --i) {
        if(X % i == 0)
            return i;
    }
    return 1;
}

void openmp_hello() {
    #pragma omp parallel
    {
        // Ensure orderly output from each thread (this make the output not parallel!!!!)
       #pragma omp critical
       {
       std::cout  << "OpenMP is working " << omp_get_thread_num()<< "/"<<omp_get_num_threads() << " threads." << std::endl;
       }
    }
}

PYBIND11_MODULE(MOD_NAME, m) {
    m.def("find_factor", &find_factor, "Returns the largest nontrivial factor of a number. If none found, returns 1 (i.e., the trivial factor)");
    m.def("find_factor", py::vectorize(find_factor), "Vectorized form of find_factor to work on numpy arrays");
    m.def("openmp_hello", &openmp_hello, "Prints a message from each OpenMP thread to verify multi-threading functionality");
}