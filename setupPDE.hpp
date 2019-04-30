/*setupPDE
*/
#include<input_variables.h>

class setupPDE{
protected:
    int *nDim; //
    int *NumGridPoints;//
    int *numTimeSteps;//
    int *numProcs;//
    int *proc_rank;//
    double * u0; //
    double * u; //
    
    
public:
    void process_inputs(int, char**);
    void perform_one_timestep();
    void update_initial_conditions();
}

