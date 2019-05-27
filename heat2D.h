// Header for solving 2D heat equations --- Prototype
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <mpi.h>
/*
 These parameters(global) are specifically defined here for now. Once we complete
 the "geometry" class, these parameters shall be input from the domain geometry.
 For now, let us focus on the prototype.
*/
#define Nx 100      // number of cells in the x-direction
#define Ny 100      // number of cells in the y-direction
#define L  10.0     // domain length (to be replaced using "box" in geom)
#define W  10.0     // domain width  (to be replaced using "box in geom )
#define C  1.0      // material conductivity, assume spatially constant for now.
#define TEND 1.0    // End-time of the simulation ( start time is 0)
#define DX (L/NX)   // dx, cell size
#define DY (W/NY)   // dy, cell size

#define Sx 1        // cells per subdomain in x
#define Sy 1        // cells per subdomain in y

#define DT (1/(2*C*(1/DX/DX+1/DY/DY))) // compute DT using stability criterion
#define NO_STEPS (TEND/DT)             // No. of time steps

#define KX (C*DT/(DX*DX)) // to be used in laplace solver
#define KY (C*DT/(DY*DY)) // to be used in laplace solver

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

/* Declare local structures */
typedef struct {
    int rank;       // global rank
    int npcs;       // total number of procs
    int size;       // domain size (local)
    int nx;         // number of cells in the x-direction (local)
    int ny;         // number of cells in the y-direction (local)
    int rx;         // x-rank coordinate
    int ry;         // y-rank coordinate
    int u;          // upper neigbour rank
    int d;          // lower neigbour rank
    int l;          // left neigbour rank
    int r;          // right neigbour rank
} dom;

/* Declare functions */
void Decompose_domain(int rank, int npcs, int *coord, int *ngbr);
void Manage_Memory(int phase, dom domain, real **h_u, real **t_u, real **t_un);
void set_initial_conditions (int IC, real *h_u);
void call_Laplace_solver(dom domain, real **t_u, real **t_un);
void Manage_Communications(dmn domain, MPI_Comm Comm, MPI_Datatype xSlice, MPI_Datatype ySlice, real *t_u);
void one_time_step();

