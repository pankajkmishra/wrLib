//-- will decide later what includes are actually necesarry, pasting everything now
//------------------------------------
#include<setupPDE.hpp>
#include<parcom.hpp>
#include<mpi.hpp>
#include<waveform.hpp>
#include<WF_classical.hpp>
#include<WF_wrap.hpp>
//----------------------------------
void decompose_domain1D(int &numProcs, double &current_grid, double &u_current_grid, int& current_start, int &current_end, int &current_num );

{
    int start_points[numProcs];
    int end_points[numProcs];
    int num_points[numProcs];
    
    if (numProcs<=NumGridPoints)
    {
        int aprxPointsPerProc = NumGridPoints/numProcs;
        int remPoints= NumGridPoints % numProcs;
        
        for (int i = 0; i < numProcs; i++)
        {
            int start,end,num;
            if(i<remPoints)
            {
                start  = // compute start
                end =  // compute end
                num = // to be computed
                }
                else
                {
                    start  = // compute start
                    end =  // compute end
                    num = // to be computed
                }
                
                num =num+1;
                end =end+1;
                num =num+1;
                
                // compute start, end and number of points for each subdomain
                start_points[i]=start;
                end_points[i]=end;
                num_points[i]=num;
            }
        }
        else
        {
            
            cout<< "Need to think what to do, if user requests more processes than grids."
            }
            
            
            
            int current_num = num_points[proc_rank];
            int current_start = start_points[proc_rank];
            int current_end = end_points[proc_rank];
            double current_grid[current_num];
            double u_current_grid[current_num];


    }



//Add more cases here
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void send_and_receive_between_processors(){
                if (proc_rank>0){
                    MPI_Send(&u_current_grid[1],1,MPI_DOUBLE,proc_rank-1,2,MPI_COMM_WORLD);
                }
                //-------------------------------------------------------------------------
                if (proc_rank<numProcs-1){
                    MPI_Send(&u_current_grid[current_num-2],1,MPI_DOUBLE,proc_rank+1,2,MPI_COMM_WORLD);
                }
                //===================================================================================
                if (proc_rank>0){
                    MPI_Status status;
                    MPI_Recv(&u_current_grid[0],1,MPI_DOUBLE,proc_rank-1,2,MPI_COMM_WORLD,&status);
                }
                //-------------------------------------------------------------------------------------
                if (proc_rank<numProcs-1){
                    MPI_Status status;
                    MPI_Recv(&u_current_grid[current_num-1],1,MPI_DOUBLE,proc_rank+1,2,MPI_COMM_WORLD,&status);
                }
                
            }
