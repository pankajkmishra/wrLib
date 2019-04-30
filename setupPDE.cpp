//---------------------------------
#include<setupPDE.hpp>
#include<parcom.hpp>
#include<mpi.hpp>
#include<waveform.hpp>
#include<WF_classical.hpp>
#include<WF_wrap.hpp>

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void process_inputs(int argc char** argv){
    
    // Process inputs & if they are not okay, abort with an error!
    if ((argc != 5) && (proc_rank==0))
    {
        cout<<"Invalid number of arguments, Aborting!"<<endl;
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    NumGridPoints = atoi(argv[3]);
    numTimeSteps  = atoi(argv[4]);
           u0[0]  = atof(argv[1]);
           u0[1]  = atof(argv[2]);
    
    if ((NumGridPoints < 1)&&(proc_rank==0))
    {
        cout<<"I need atleast 1 point.  Aborting!"<<endl;
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    
    if ((numProcs > NumGridPoints) && (proc_rank==0))
    {
        cout<<"More processor than tasks --- will take care of this later, abort for now!"<<endl;
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void perform_one_timestep(int current_num, double &current_grid, double &u_current_grid)
{
    r = dt/pow(dx,2);
    for (int j = 1; j < current_num - 1; j++){
        u_current_grid[j]=(1-2*r)*current_grid[j]+r*current_grid[j-1]+r*current_grid[j+1];
    }
    
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void update_initial_conditions(){
    if (proc_rank==0){
        u_current_grid[0]=u0[0];
    //----------------------------------------
    }
    if (proc_rank==numProcs-1){
        u_current_grid[current_num-1]=u0[1];
    }
    //----------------------------------------
    for (int i = 0; i < current_num; i++){
        current_grid[i]=u_current_grid[i];
    }
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

