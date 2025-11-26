// ******************************************
// implicit time stepping implementation of 2D diffusion problem
// Ben Cumming, CSCS
// *****************************************

// A small benchmark app that solves the 2D fisher equation using second-order
// finite differences.

// Syntax: ./main nx nt t

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdio.h>


#ifdef _MPI
    #include <mpi.h>
#endif

#include "data.h"
#include "linalg.h"
#include "operators.h"
#include "walltime.h"
#include "stats.h"

using namespace data;
using namespace linalg;
using namespace operators;
using namespace stats;

// =============================================================================
/*
    Field &u:               
        e' il campo da scrivere su disco, la sol numerica del sottodominio locale    di questo processo (NON tutta la griglia.
    SubDomain &domain:      
        informazioni sulla decomposizione, tipo startx, starty, endx, endy, nx, ny...
    Discretization &options:
        informazioni sulla discretizzazione globale, tipo nx, nt, dx, dt...
*/
void write_binary(std::string fname, Field &u, SubDomain &domain,
                  Discretization &options)
{  
    // DONE: Implement output with MPI-IO

    // Create MPI file handle
    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD, fname.c_str(), // nome del file in char* perche MPI e' in C
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, // modalita' di apertura del file
                  MPI_INFO_NULL, &fh); 
    
    if (domain.rank == 0) std::cout << "File opened successfully." << std::endl;

    // Global grid size
    int NX = options.nx;
    int NY = options.nx;  // square domain

    // Local subdomain size
    int nx = domain.nx;
    int ny = domain.ny;

    // Create a subarray datatype to describe the local part of the global array
    if (domain.rank == 0) std::cout << "Creating subarray type..." << std::endl;
    MPI_Datatype filetype;
    int gsizes[2] = {NY, NX};
    int lsizes[2] = {ny, nx};
    int starts[2] = {domain.starty - 1, domain.startx - 1};

    MPI_Type_create_subarray(2, gsizes, lsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &filetype);
    MPI_Type_commit(&filetype);

    // Set the file view
    if (domain.rank == 0) std::cout << "Setting file view..." << std::endl;
    MPI_File_set_view(fh, 0, MPI_DOUBLE, filetype, "native", MPI_INFO_NULL);

    // Write all data in a single collective call
    if (domain.rank == 0) std::cout << "Writing all data..." << std::endl;
    MPI_File_write_all(fh, u.data(), nx * ny, MPI_DOUBLE, MPI_STATUS_IGNORE);
    if (domain.rank == 0) std::cout << "Write all data completed." << std::endl;

    MPI_Type_free(&filetype);
    if (domain.rank == 0) std::cout << "Closing file..." << std::endl;
    MPI_File_close(&fh);
    if (domain.rank == 0) std::cout << "File closed." << std::endl;
}

// read command line arguments
void readcmdline(Discretization& options, int argc, char* argv[]) {
    if (argc<4 || argc>5) {
        std::cerr << "Usage: main nx nt t verbose\n";
        std::cerr << "  nx      number of grid points in x-direction and "
                               "y-direction, respectively\n";
        std::cerr << "  nt      number of time steps\n";
        std::cerr << "  t       total time\n";
        std::cerr << "  verbose (optional) verbose output\n";
        exit(1);
    }

    // read nx
    options.nx = atoi(argv[1]);
    if (options.nx < 1) {
        std::cerr << "nx must be positive integer\n";
        exit(-1);
    }

    // read nt
    options.nt = atoi(argv[2]);
    if (options.nt < 1) {
        std::cerr << "nt must be positive integer\n";
        exit(-1);
    }

    // read total time
    double t = atof(argv[3]);
    if (t < 0) {
        std::cerr << "t must be positive real value\n";
        exit(-1);
    }

    // set verbosity if requested
    verbose_output = false;
    if (argc==5) {
        verbose_output = (domain.rank==0);
    }

    // set total number of grid points
    options.N = options.nx * options.nx;

    // set time step size
    options.dt = t / options.nt;

    // set distance between grid points
    // assume that x dimension has length 1.0
    options.dx = 1. / (options.nx - 1);

    // set alpha, assume diffusion coefficient D is 1
    double D = 1.;
    options.alpha = (options.dx * options.dx) / (D * options.dt);

    // set beta, assume diffusion coefficient D=1, reaction coefficient R=1000
    double R = 500.;
    options.beta = (R * options.dx * options.dx)/D;
}

// =============================================================================

int main(int argc, char* argv[]) {
    // read command line arguments
    readcmdline(options, argc, argv);

    // set iteration parameters
    int max_cg_iters     = 300;
    int max_newton_iters = 50;
    double tolerance     = 1.e-6;

    // initialize MPI
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // TODO: initialize sub-domain (data.{h,cpp})
    domain.init(rank, size, options);
    // domain.print(); // for debugging
    if (rank == 0) std::cout << "Domain initialized. Printing neighbors..." << std::endl;
    domain.print();

    int nx = domain.nx; // nx is local sub-domain size in x direction specifically is the number of grid points in x direction for each sub-domain
    int ny = domain.ny;
    int N  = domain.N; // N is total number of grid points, nx * ny would have been too clear
    int nt  = options.nt;

    if (rank == 0) {
        std::cout << std::string(80, '=') << std::endl;
        std::cout << "                      Welcome to mini-stencil!" << std::endl;

    #ifdef _MPI
        std::cout << "version   :: C++ MPI" << std::endl;
        int threads = size;
    #else
        std::cout << "version   :: C++ Serial" << std::endl;
        int threads = 1;
    #endif
        std::cout << "threads   :: " << threads << std::endl;
        std::cout << "mesh      :: " << options.nx << " * " << options.nx
                                    << " dx = " << options.dx << std::endl;
        std::cout << "time      :: " << nt << " time steps from 0 .. "
                                        << options.nt*options.dt << std::endl;
        std::cout << "iteration :: " << "CG "          << max_cg_iters
                                    << ", Newton "    << max_newton_iters
                                    << ", tolerance " << tolerance << std::endl;
        std::cout << std::string(80, '=') << std::endl;
    }

    // allocate global fields
    y_new.init(nx, ny);
    y_old.init(nx, ny);
    bndN.init(nx, 1);
    bndS.init(nx, 1);
    bndE.init(ny, 1);
    bndW.init(ny, 1);
    buffN.init(nx, 1);
    buffS.init(nx, 1);
    buffE.init(ny, 1);
    buffW.init(ny, 1);

    Field f(nx,ny);
    Field deltay(nx,ny);

    // set Dirichlet boundary conditions to 0.1 all around
    double bdy_value = 0.1;
    hpc_fill(bndN, bdy_value);
    hpc_fill(bndS, bdy_value);
    hpc_fill(bndE, bdy_value);
    hpc_fill(bndW, bdy_value);

    // set the initial condition
    // a circle of concentration 0.2 centred at (xdim/4, ydim/4) with radius
    // no larger than 1/8 of both xdim and ydim
    double const_fill = 0.1;
    double inner_circle = 0.2;
    hpc_fill(y_new, const_fill);
    double xc = 1.0 / 4.0;
    double yc = 1.0 / 4.0;
    double radius = std::min(xc, yc) / 2.0;
    for (int j = domain.starty-1; j < domain.endy; j++) {
        double y = (j - 1) * options.dx;
        for (int i = domain.startx-1; i < domain.endx; i++) {
            double x = (i - 1) * options.dx;
            if ((x - xc) * (x - xc) + (y - yc) * (y - yc) < radius * radius) {
                y_new(i-domain.startx+1,j-domain.starty+1) = inner_circle;
            }
        }
    }

    iters_cg = 0;
    iters_newton = 0;

    // start timer
    double time_start = walltime();

    // main time loop
    for (int timestep = 1; timestep <= nt; timestep++) {
        if (rank == 0 && timestep % 10 == 0) std::cout << "Starting step " << timestep << std::endl;

        // set y_new and y_old to be the solution
        hpc_copy(y_old, y_new);

        double residual;
        bool converged = false;
        int it;
        for (it = 0; it < max_newton_iters; it++) {
            // compute residual
            diffusion(y_old, y_new, f);
            residual = hpc_norm2(f);

            // check for convergence
            if (residual < tolerance) {
                converged = true;
                break;
            }

            // solve linear system to get deltay
            bool cg_converged = false;
            hpc_cg(deltay, y_old, y_new, f, max_cg_iters, tolerance,
                   cg_converged);

            // check that the CG solver converged
            if (!cg_converged) break;

            // update solution
            hpc_axpy(y_new, -1.0, deltay);
        }
        iters_newton += it+1;

        // output some statistics
        if (converged && verbose_output) {
            std::cout << "step " << timestep
                      << " required " << it
                      << " iterations for residual " << residual
                      << std::endl;
        }
        if (!converged) {
            std::cerr << "step " << timestep
                      << " ERROR : nonlinear iterations failed to converge" << std::endl;;
            break;
        }
    }

    // get times
    double time_end = walltime();

    ////////////////////////////////////////////////////////////////////
    // write final solution to BOV file for visualization
    ////////////////////////////////////////////////////////////////////

    // binary data
    // DONE: Implement write_binary using MPI-IO
    // binary data
    // DONE: Implement write_binary using MPI-IO
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) std::cout << "All ranks reached write_binary." << std::endl;
    write_binary("output.bin", y_old, domain, options);
    

    // metadata
    // DONE: Only once process should do the following
    if (rank == 0) {
        std::ofstream fid("output.bov");
        fid << "TIME: " << options.nt*options.dt << std::endl;
        fid << "DATA_FILE: output.bin" << std::endl;
        fid << "DATA_SIZE: " << options.nx << " " << options.nx << " 1"
            << std::endl;
        fid << "DATA_FORMAT: DOUBLE" << std::endl;
        fid << "VARIABLE: phi" << std::endl;
        fid << "DATA_ENDIAN: LITTLE" << std::endl;
        fid << "CENTERING: nodal" << std::endl;
        fid << "BRICK_ORIGIN: " << "0. 0. 0." << std::endl;
        fid << "BRICK_SIZE: " << (options.nx-1)*options.dx << ' '
                              << (options.nx-1)*options.dx << ' '
                              << " 1.0"
            << std::endl;
    }

    // print table summarizing results
    double timespent = time_end - time_start;
    // DONE: Only once process should do the following
    if (rank == 0){
        std::cout << std::string(80, '-') << std::endl;
        std::cout << "simulation took " << timespent << " seconds" << std::endl;
        std::cout << int(iters_cg)
                  << " conjugate gradient iterations, at rate of "
                  << float(iters_cg)/timespent << " iters/second" << std::endl;
        std::cout << iters_newton << " newton iterations" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        std::cout << "### " << size << ", "
                            << options.nx << ", "
                            << options.nt << ", "
                            << iters_cg   << ", "
                            << iters_newton <<  ", "
                            << timespent
                  << " ###" << std::endl;
        std::cout << "Goodbye!" << std::endl;
    }

    // DONE: finalize MPI
    MPI_Comm_free(&domain.comm_cart);
    MPI_Finalize();

    return 0;
}
