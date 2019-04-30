//---------------------------------
#include<setupPDE.hpp>
#include<parcom.hpp>
#include<mpi.hpp>
#include<waveform.hpp>
//----------------------------------


int main(int argc, char *argv[]){
    
    MPI_Init(&argc,&argv);
    
    int numProcs;
    int proc_rank;
    
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

//-----------------------------------------------------------in setupPDE class
    process_inputs(argc, argv, NumGridPoints, numTimesteps, u0);

//----------------------------------------------------------in Parcom class
    decompose_domain1D(int &numProcs, double &current_grid, double &u_current_grid, int& current_start, int &current_end, int &current_num );

    send_and_receive_between_processors()
//--------------------------------------------------------in setupPDE class
    update_boundary_conditions( int proc_rank, int numProcs, double &current_grid, double &u_current_grid, int& current_start, int &current_end, int &current_num double &u0)
    
//--------------------------------------------------------in waveform class
    do_WF_iteration(int proc_rank, numProcess, char WF_type, int maximum_iteration, double -----  // use 'do_one_time_step' from the class setupPDE;
        // use 'send_and_receieve' from the class parcom;
//----------------------------------------------------------------------
    if (collect == 1){
        collection_solutions();
    }
                                  
}
