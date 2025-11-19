/*
 * HPC Assignment: MPI Basic
 * Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>
 *
 * Description:
 * This program provides a basic flow of an MPI program, including initializing the MPI environment,
 * distributing tasks across processes, and gathering results. Here we partition the work.
 *
 * Compilation Instructions:
 * To compile the program (see make file) with both OpenMP and MPI support, use the following command:
 * mpic++ -fopenmp main.cpp -o main.exe
 *
 * Run Instructions:
 * Use mpirun to execute the program with different numbers of processes:
 * $ mpirun -np 1 ./main.exe   # Run with 1 process
 * $ mpirun -np 2 ./main.exe   # Run with 2 processes
 * $ mpirun -np 4 ./main.exe   # Run with 4 processes
 *
 * TODO:
 * 1) Run the program with 1, 2, and 4 processes, noting any changes in performance or behavior.
 * 2) Hypothesize the program’s runtime behavior if scaled to 100 processes and consider factors
 *    that may affect efficiency or overhead at higher process counts.
 */

#include <cmath>
#include <vector>
#include <iostream>
#include <mpi.h>

int main(){

    int n = 1e4;
    std::vector<double> val(n,0);
    
    int size, rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t=MPI_Wtime();

    int block_size = n/size;
    int start = rank*block_size;
    int end   = (rank+1)*block_size;
    if(rank==size-1){
        end=n;
    }
    for ( int i = start; i < end; i++){
        for ( int j=0; j < 1000; j++){
            val[i]+= std::sin(2 * 3.145 * double(j+i) / double(n));
        }
    }
    
    MPI_Allreduce( MPI_IN_PLACE, &val[0]  , n , MPI_DOUBLE  , MPI_SUM , MPI_COMM_WORLD);

    double sum=0;
    for ( int i = 0; i < val.size(); i++){
       sum+=val[i];
    }

    printf("(Rank %d) Time=%f sum=%f \n",rank,MPI_Wtime() - t,sum);

    MPI_Finalize();
    return 0;
}

