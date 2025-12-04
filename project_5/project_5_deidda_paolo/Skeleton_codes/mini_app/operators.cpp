//******************************************
// operators.cpp
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 2/3d-meshes

#include "data.h"
#include "operators.h"
#include "stats.h"

#include <iostream>

namespace operators {

// compute the diffusion-reaction stencils
// s_old is the population concentration at time step k-1, s_new at k,
// and f is the residual (see Eq. (7) in Project 3).
void diffusion(data::Field const& s_old, data::Field const& s_new,
               data::Field& f) {
    using data::options;
    using data::domain;

    using data::bndE;
    using data::bndW;
    using data::bndN;
    using data::bndS;

    using data::buffE;
    using data::buffW;
    using data::buffN;
    using data::buffS;

    double alpha = options.alpha;
    double beta = options.beta;

    int nx = domain.nx;
    int ny = domain.ny;
    int iend  = nx - 1;
    int jend  = ny - 1;

    // TODO: exchange the ghost cells using non-blocking point-to-point
    //       communication
    MPI_Request requests[8];
    int req_count = 0;
    
    // std::cout << "Rank " << domain.rank << " entering diffusion" << std::endl;


    // North/South: buffers have length nx
    // Pack and post if neighbor exists
    if (domain.neighbour_north != MPI_PROC_NULL) {
        // send our top row to the north neighbor
        for (int i = 0; i < nx; ++i) {
            buffN[i] = s_new(i, ny - 1);
        }
        MPI_Irecv(bndN.data(), nx, MPI_DOUBLE,
                  domain.neighbour_north, 0, MPI_COMM_WORLD,
                  &requests[req_count++]);
        MPI_Isend(buffN.data(), nx, MPI_DOUBLE,
                  domain.neighbour_north, 1, MPI_COMM_WORLD,
                  &requests[req_count++]);
    }

    if (domain.neighbour_south != MPI_PROC_NULL) {
        // send our bottom row to the south neighbor
        for (int i = 0; i < nx; ++i) {
            buffS[i] = s_new(i, 0);
        }
        MPI_Irecv(bndS.data(), nx, MPI_DOUBLE,
                  domain.neighbour_south, 1, MPI_COMM_WORLD,
                  &requests[req_count++]);
        MPI_Isend(buffS.data(), nx, MPI_DOUBLE,
                  domain.neighbour_south, 0, MPI_COMM_WORLD,
                  &requests[req_count++]);
    }

    // East/West: buffers have length ny
    if (domain.neighbour_east != MPI_PROC_NULL) {
        // send our rightmost column to the east neighbor
        for (int j = 0; j < ny; ++j) {
            buffE[j] = s_new(nx - 1, j);
        }
        MPI_Irecv(bndE.data(), ny, MPI_DOUBLE,
                  domain.neighbour_east, 2, MPI_COMM_WORLD,
                  &requests[req_count++]);
        MPI_Isend(buffE.data(), ny, MPI_DOUBLE,
                  domain.neighbour_east, 3, MPI_COMM_WORLD,
                  &requests[req_count++]);
    }

    if (domain.neighbour_west != MPI_PROC_NULL) {
        // send our leftmost column to the west neighbor
        for (int j = 0; j < ny; ++j) {
            buffW[j] = s_new(0, j);
        }
        MPI_Irecv(bndW.data(), ny, MPI_DOUBLE,
                  domain.neighbour_west, 3, MPI_COMM_WORLD,
                  &requests[req_count++]);
        MPI_Isend(buffW.data(), ny, MPI_DOUBLE,
                  domain.neighbour_west, 2, MPI_COMM_WORLD,
                  &requests[req_count++]);
    }

    // the interior grid points
    for (int j=1; j < jend; j++) {
        for (int i=1; i < iend; i++) {
            f(i,j) = -(4. + alpha) * s_new(i,j)     // central point
                   + s_new(i-1,j) + s_new(i+1,j)    // east and west
                   + s_new(i,j-1) + s_new(i,j+1)    // north and south
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }
    }

    if (req_count > 0) {
        // std::cout << "Rank " << domain.rank << " waiting for " << req_count << " requests" << std::endl;
        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);
        // std::cout << "Rank " << domain.rank << " finished waitall" << std::endl;
    }


    // east boundary
    {
        int i = nx - 1;
        for (int j = 1; j < jend; j++) {
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + s_new(i-1,j) + bndE[j]
                   + s_new(i,j-1) + s_new(i,j+1)
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }
    }

    // west boundary
    {
        int i = 0;
        for (int j = 1; j < jend; j++) {
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + bndW[j]      + s_new(i+1,j)
                   + s_new(i,j-1) + s_new(i,j+1)
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }
    }

    // north boundary (plus NE and NW corners)
    {
        int j = ny - 1;

        {
            int i = 0; // NW corner
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + bndW[j]      + s_new(i+1,j)
                   + s_new(i,j-1) + bndN[i]
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }

        // north boundary
        for (int i = 1; i < iend; i++) {
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + s_new(i-1,j) + s_new(i+1,j)
                   + s_new(i,j-1) + bndN[i]
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }

        {
            int i = nx - 1; // NE corner
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + s_new(i-1,j) + bndE[j]
                   + s_new(i,j-1) + bndN[i]
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }
    }

    // south boundary (plus SW and SE corners)
    {
        int j = 0;
        {
            int i = 0; // SW corner
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + bndW[j] + s_new(i+1,j)
                   + bndS[i] + s_new(i,j+1)
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }

        // south boundary
        for (int i = 1; i < iend; i++) {
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + s_new(i-1,j) + s_new(i+1,j)
                   + bndS[i]      + s_new(i,j+1)
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }

        {
            int i = nx - 1; // SE corner
            f(i,j) = -(4. + alpha) * s_new(i,j)
                   + s_new(i-1,j) + bndE[j]
                   + bndS[i]      + s_new(i,j+1)
                   + alpha * s_old(i,j)
                   + beta * s_new(i,j) * (1.0 - s_new(i,j));
        }
    }

    // Accumulate the flop counts
    // 8 ops total per point
    stats::flops_diff += 12 * (nx - 2) * (ny - 2) // interior points
                      +  11 * (nx - 2  +  ny - 2) // NESW boundary points
                      +  11 * 4;                  // corner points
}

}
