/*
loop_reorder_example.cpp
HPC Class: Loop reordering and contiguous data access.
Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

Compile Instructions:
- Compile: g++ -o main.exe loop_reoder_example.cpp
- Allocate (SLURM): salloc -N 1 --time=00:05:00 --reservation=hpc-monday
- Execute: ./main.exe

TODO:
1. What is "vector<double>"? It is very useful. See: https://en.cppreference.com/w/cpp/container/vector 
2. Run the code. Why is there a difference in runtime?
3. Try compiling with the "-O3" flag, i.e., g++ -o main.exe loop_reoder_example.cpp -O3. How does that change the runtime?
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Function for matrix-vector multiplication (row-major access)
void matrix_vector_multiply_row(vector<double>& y, double** A, const vector<double>& x, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            y[i] = A[i][j] * x[j];
        }
    }
}

// Function for matrix-vector multiplication (column-major access)
void matrix_vector_multiply_col(vector<double>& y, double** A, const vector<double>& x, size_t n) {
    fill(y.begin(), y.end(), 0);  // Initialize result vector
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < n; ++i) {
            y[i] = A[i][j] * x[j];
        }
    }
}

int main() {
    // Matrix size and number of iterations
    size_t n = 2000;
    size_t K = 100;

    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    // Allocate and initialize a symmetric matrix with random numbers
    double** A = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        A[i] = new double[n];
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            double val = dis(gen);
            A[i][j] = val;
            A[j][i] = val;  // Symmetric assignment
        }
    }

    // Initialize vector x with random numbers
    vector<double> x(n);
    for (size_t i = 0; i < n; ++i) {
        x[i] = dis(gen);
    }

    // Initialize vector y
    vector<double> y(n);

    // Test column-major matrix-vector multiplication
    auto start_time = high_resolution_clock::now();
    for (size_t i = 0; i < K; ++i) {
        matrix_vector_multiply_col(y, A, x, n);
    }
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    double sum_col = accumulate(y.begin(), y.end(), 0.0);
    cout << "Average runtime for matrix_vector_multiply_col over " << K << " runs: "
         << static_cast<double>(duration.count()) / K << " ms, sum: " << sum_col << endl;

    // Test row-major matrix-vector multiplication
    start_time = high_resolution_clock::now();
    for (size_t i = 0; i < K; ++i) {
        matrix_vector_multiply_row(y, A, x, n);
    }
    end_time = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end_time - start_time);
    double sum_row = accumulate(y.begin(), y.end(), 0.0);
    cout << "Average runtime for matrix_vector_multiply_row over " << K << " runs: "
         << static_cast<double>(duration.count()) / K << " ms, sum: " << sum_row << endl;

    // Cleanup
    for (size_t i = 0; i < n; ++i) {
        delete[] A[i];
    }
    delete[] A;

    return 0;
}