#include "data.h"

#include <iostream>
#include <cmath>
#include <mpi.h>

namespace data {

// fields that hold the solution
Field y_new;
Field y_old;

// fields that hold the boundary points
Field bndN;
Field bndE;
Field bndS;
Field bndW;

// buffers used during boundary halo communication
Field buffN;
Field buffE;
Field buffS;
Field buffW;

// global domain and local sub-domain
Discretization options;
SubDomain      domain;

void SubDomain::init(int mpi_rank, int mpi_size,
                     Discretization& discretization) {
    // DONE: determine the number of sub-domains in the x and y dimensions
    //       using MPI_Dims_create
    int dims[2] = {0, 0}; // 0 let mpi decide I can for some reason enforce a specific decomposition like {0,1} 
    MPI_Dims_create(mpi_size, 2, dims);
    ndomy = dims[0]; 
    ndomx = dims[1];

    // DONE: create a 2D non-periodic Cartesian topology using MPI_Cart_create
    int periods[2] = {0, 0}; // dice per ogni dimensione se e' periodica o no (se deve fare il wrap around)
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart); // 0: reorder, dice se MPI puo' riordinare i rank per ottimizzare, 0 significa no reorder

    // DONE: retrieve coordinates of the rank in the topology using
    // MPI_Cart_coords
    int coords[2];
    MPI_Cart_coords(comm_cart, mpi_rank, 2, coords);
    domy = coords[0] + 1; // perche' +1? 
    domx = coords[1] + 1;

    // DONE: set neighbours for all directions using MPI_Cart_shift
    MPI_Cart_shift(comm_cart, 0, 1, &neighbour_south, &neighbour_north); // shift along dimension 0 (y direction)
    MPI_Cart_shift(comm_cart, 1, 1, &neighbour_west,  &neighbour_east); // shift along dimension 1 (x direction)

    int nx_global = discretization.nx;

    /*
        gestische quando il subdomain non e' divisibile esattamente per il numero di processi
        esempio: 10 punti e 3 processi -> 4, 3, 3 punti
    */
    
    // base size e remainder lungo x
    int base_nx = nx_global / ndomx;
    int rem_x   = nx_global % ndomx;

    // base size e remainder lungo y
    int base_ny = nx_global / ndomy;
    int rem_y   = nx_global % ndomy;

    // domx, domy sono 1-based
    int ix = domx - 1;  // 0-based process coord in x
    int iy = domy - 1;  // 0-based process coord in y

    // dimensioni locali
    nx = base_nx + (ix < rem_x ? 1 : 0);
    ny = base_ny + (iy < rem_y ? 1 : 0);

    // start indices (1-based global indexing)
    startx = ix * base_nx + std::min(ix, rem_x) + 1;
    starty = iy * base_ny + std::min(iy, rem_y) + 1;

    endx = startx + nx - 1;
    endy = starty + ny - 1;

    // numero di punti locali
    N = nx * ny;

    rank = mpi_rank;
    size = mpi_size;
}

// print domain decomposition information to stdout
void SubDomain::print() {
    for (int irank = 0; irank < size; irank++) {
        if (irank == rank) {
            std::cout << "rank " << rank << " / " << size
                      << " : (" << domx << ", " << domy << ")"
                      << " neigh N:S " << neighbour_north
                      << ":"           << neighbour_south
                      << " neigh E:W " << neighbour_east
                      << ":"           << neighbour_west
                      << " local dims " << nx << " x " << ny
                      << std::endl;
        }
//        MPI_Barrier(MPI_COMM_WORLD);
    }
}

}
