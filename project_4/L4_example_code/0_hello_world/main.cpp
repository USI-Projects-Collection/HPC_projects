/*
 * HPC Assignment: OpenMP and MPI Test
 * Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>
 *
 * Description:
 * This program is an example of using OpenMP with MPI.
 *
 * Compilation Instructions:
 * To compile the program (see make file) with both OpenMP and MPI support, use the following command:
 * mpic++ -fopenmp main.cpp -o main.exe
 *
 * Run Instructions:
 * 1) Using the sbatch file and makefile.
 *
 * TODO:
 * 1) See the sbatch script and make file.
 * 2) Edit the sbatch file chance "ntasks-per-node" and "cpus-per-task"
 */

#include <iostream>
#include <omp.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    #pragma omp parallel
    {
       int thread_id = omp_get_thread_num();
       if (thread_id == 0) {
           std::cout  << "OpenMP is working " << omp_get_num_threads() << " threads." << " MPI is working. Rank "  << world_rank << " out of " << world_size << " | "<<processor_name << std::endl;
       }
    }

    MPI_Finalize();
    return 0;
}
