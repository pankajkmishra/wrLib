#include "heat2d.h"

int main ( int argc, char *argv[] ) {
    
    real *h_u;
    real *t_u;
    real *t_un;
    
    int rank;
    int size;
    int step;
    dom domain;
    int nbrs[4]; // stencil size, works for meshfree too
    int i, j;
    
    
    
    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    
    // Create a 2D cartessian communicator ======================
    MPI_Comm Communicator2D;
    int ndim     =2;                  // take this as an output from geometry class at later stage
    int dim[2]   ={Sy,Sx};           // take this as an input to "Grid" class at later stage
    int period[2]={false,false};     // periodic conditions
    int reorder  ={true};
    int coord[2];
    
    MPI_Cart_create(MPI_COMM_WORLD,ndim,dim,period,reorder,&Communicator2D);
    /* Create a communicator with attached topology information
     Inputs:
     MPI_COMM_WORLD: input communicator
     ndim          : number of dimensions of cartesian grid (integer)
     dim           : integer array of size ndims specifying the number of processes in each dimension
     period        : logical array of size ndims specifying whether the grid is periodic (true) or not (false) in each dimension
     reorder       : ranking may be reordered (true) or not (false)
     
     Outputs:
     Communicator2D:         communicator with new cartesian topology (handle)
    */
    
    //====================================================================
    MPI_Comm_rank  (Communicator2D,&rank); // rank wrt to Communicator2D
    
    
    //====================================================================
    /* Determines process coords in cartesian topology given rank in group */
    MPI_Cart_coords(Communicator2D,rank,2,coord);
    /* Input:
     Communicator2D: communicator with cartesian structure
     rank  : rank of a process within group of Communicator2D
     2     : length of vector coords in the calling program
     Output:
     coord : integer array (of size ndim) containing the Cartesian coordinates of specified process
     */

    // =============================================================
    MPI_Cart_shift(Communicator2D,0,1,&nbrs[DOWN],&nbrs[UP]); // Map the neighbors UP & DOWN
    /* Returns the shifted source and destination ranks, given a shift direction and amount
     Inputs:
     Communicator2D: communicator with cartesian structure
     0:             0 means UP direction   see heat2D.h
     1:             1 shift
     Outputs:
     nbrs[DOWN]:    Rank of source process
     nbrs[UP]:      Rank of desitation process
     */
    MPI_Cart_shift(Communicator2D,1,1,&nbrs[LEFT],&nbrs[RIGHT]);
    
    
    //=====================================================================
    domain = Decompose_domain(rank,size,coord,nbrs);
    Manage_Memory(0,domain,&h_u,&t_u,&t_un);
    
    
    //====================================================================
    if (domain.rank==0) set_initial_condition(2,h_u);
    
    
    //=================================================================
    MPI_Datatype global;
    MPI_Datatype myGlobal;
    MPI_Datatype myLocal;
    MPI_Datatype xSlice;
    MPI_Datatype ySlice;
    
    int nx = domain.nx;
    int ny = domain.ny;
    int bigsizes[2] = {Ny,Nx};
    int subsizes[2] = {ny,nx};
    int starts[2] = {0,0};
    
    MPI_Type_create_subarray(ndim, bigsizes, subsizes, starts, MPI_ORDER_C, MPI_CUSTOM_REAL, &global);
    //Create a datatype for a subarray of a regular, multidimensional array
    /* Inputs:
     ndim: number of dimensions (= 2)
     bigsizes: number of elements of type oldtype in each dimension of the full array
     subsizes: number of elements of type oldtype in each dimension of the subarray
     starts  : starting coordinates of the subarray in each dimension
     MPI_ORDER_C; array storage order flag
     MPI_CUSTOm_REAL: array element datatype
     
     OUTPUT:
     global: new_data_type
     */
    
    
    //===============================================================
     MPI_Type_create_resized(global, 0, nx*sizeof(real), &myGlobal);
    /* Inputs:
     global: input datatype
     0     : lower bound of the data type (address integer)
     nx*sizeof(real): new extent of datatype (address integer)
     
     */
     //================================================================
     MPI_Type_commit(&myGlobal);
    //==================================================================


   // Build a MPI data type for a subarray in workers
   // Scatter global array data
    
    
    if (WFR_type == 'classical') {
        // Write classical waveform relaxation algorithm here using the followin functions;
        Execute_classical_WFR :
        1. subdomain_solver - ( call_Laplace_solver)
        2. information)exchange - (Manage_communications)
        3. one_time_step --- ( we need to define this in heat.h & heat.cpp)
    }
    
    if (WFR_type == 'classical'){
        Execute_Pipeline_WFR
        1. subdomain_solver - ( call_Laplace_solver)
        2. information)exchange - (Manage_communications)
        3. one_time_step --- (we need to define this in heat.h & heat.cpp)
    }

    
    // MPI_Gather ('Optional ?)
    
    MPI_Finalize;
    
