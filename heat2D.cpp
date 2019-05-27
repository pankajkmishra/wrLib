#include "heat2d.h"

domain Decompose_domain(int rank, int npcs, int *coord, int *ngbr){
    
    domain.rank = rank;
    domain.npcs = npcs;
    domain.ny = Ny/Sy;
    domain.nx = Nx/Sx;
    domain.size = domain.nx*domain.ny;
    domain.rx = coord[1];
    domain.ry = coord[0];
    domain.u = ngbr[UP];
    domain.d = ngbr[DOWN];
    domain.l = ngbr[LEFT];
    domain.r = ngbr[RIGHT];
    
    // Have process 0 print out some information.
    if (rank==0) {
        printf("Domain_Decomposition Function" );
    }
    return domain;
}

//=====================================================================================
void Manage_Memory(int phase, dom domain, real **h_u, real **t_u, real **t_un){
    if (phase==0) {
        // Allocate global domain on ROOT
        if (domain.rank==0) *h_u=(real*)malloc(NX*NY*sizeof(real)); // only for master!
        // Allocate local domains on MPI threats with 2 extra slots
        *t_u =(real*)malloc((domain.nx+2*R)*(domain.ny+2*R)*sizeof(real));
        *t_un=(real*)malloc((domain.nx+2*R)*(domain.ny+2*R)*sizeof(real));
        memset(*t_u ,0,(domain.nx+2*R)*(domain.ny+2*R));
        memset(*t_un,0,(domain.nx+2*R)*(domain.ny+2*R));
    }
    if (phase==1) {
        // Free the domain on master
        if (domain.rank==0) free(*h_u);
        free(*t_u);
        free(*t_un);
    }
}


void set_initial_condition(const int IC, real * __restrict u0){
    
    TBD;
}

//=======================================================================================
void Call_Laplace(dmn domain, real **u, real **un){
    
    Laplace2D(domain.nx+2*R,domain.ny+2*R,domain.rx,domain.ry,*u,*un);
    
}

void Laplace2D(const int nx, const int ny, const int rx, const int ry,
               const real * u, real * un){
    // Using (i,j) = [i+N*j] indexes
    int o, n, s, e, w;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            
            o =  i + nx*j ; // node( j,i )     n
            n = i+nx*(j+1); // node(j+1,i)     |
            s = i+nx*(j-1); // node(j-1,i)  w--o--e
            e = (i+1)+nx*j; // node(j,i+1)     |
            w = (i-1)+nx*j; // node(j,i-1)     s
            
            // only update "interior" nodes
            if(i>0 && i<nx-1 && j>0 && j<ny-1) {
                un[o] = u[o] + KX*(u[e]-2*u[o]+u[w]) + KY*(u[n]-2*u[o]+u[s]);
            } else {
                un[o] = u[o];
            }
        }
    }
}

//==========================================================================================

void Manage_Communications(dom domain, MPI_Comm Communicator2D, MPI_Datatype xSlice, MPI_Datatype ySlice, real *u) {
    const int nx = domain.nx;
    const int ny = domain.ny;
    const int n = R+domain.nx+R;
    
    // Impose BCs!
    if (domain.rx==  0 ) Set_DirichletBC(domain, u,'L');
    if (domain.rx==SX-1) Set_DirichletBC(domain, u,'R');
    if (domain.ry==  0 ) Set_DirichletBC(domain, u,'B');
    if (domain.ry==SY-1) Set_DirichletBC(domain, u,'T');
    
    // Exchage Halo regions:  top and bottom neighbors
    MPI_Sendrecv(&(u[ R + n*ny ]), 1, xSlice, domain.u, 1,
                 &(u[ R + n* 0 ]), 1, xSlice, domain.d, 1,
                 Communicator2D, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&(u[ R + n*  1   ]), 1, xSlice, domain.d, 2,
                 &(u[ R + n*(ny+1)]), 1, xSlice, domain.u, 2,
                 Communicator2D, MPI_STATUS_IGNORE);
    // Exchage Halo regions:  left and right neighbors
    MPI_Sendrecv(&(u[ nx + n*R ]), 1, ySlice, domain.r, 3,
                 &(u[  0 + n*R ]), 1, ySlice, domain.l, 3,
                 Communicator2D, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&(u[  R    + n*R ]), 1, ySlice, domain.l, 4,
                 &(u[(nx+1) + n*R ]), 1, ySlice, domain.r, 4,
                 Communicator2D, MPI_STATUS_IGNORE);
}

void Set_DirichletBC(dmn domain, real *u, const char letter){
    
    To be written;
}
