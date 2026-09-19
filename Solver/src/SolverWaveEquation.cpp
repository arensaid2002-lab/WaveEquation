#include "SolverHandler.h"
#include "SolverWaveEquation.h"
#include <Eigen/Dense>
#include <iostream>

namespace Solver
{
    SolverWaveEquation::SolverWaveEquation(float CF, float tol, float step_size, RowVectorXf Mesh): SolverHandler(tol, step_size, Mesh) 
    {
        CF_ = CF;
    }
    
    void SolverWaveEquation::GenerateMatrices_Axb()
    {
        int N = Mesh_.cols();
        MatrixXf A(N,N);
        for (int i = 0; i < N; i++)
        {
            
        }



    }

}
