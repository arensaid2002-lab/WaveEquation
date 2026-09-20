#include "SolverHandler.h"
#include "SolverWaveEquation.h"
#include "NumMethods.h"
#include <Eigen/Dense>
#include <iostream>
#include <omp.h>

using namespace NumMethods;
using namespace Eigen;
using namespace std;
namespace Solver
{
    SolverWaveEquation::SolverWaveEquation(float c, float t, float CFL, RowVectorXf Mesh, RowVectorXf u_0): SolverHandler(CFL, Mesh) 
    {
        u0 = u_0;
        c_ = c;
        t_ = t;
    }
    VectorXf SolverWaveEquation::GenerateLinearSystem_EE(RowVectorXf u)
    {
        Index N = u.cols();
        VectorXf X(N);
        string scheme_direction = (c_ > 0)? "backward" : "forward";

        #pragma omp parallel for schedule(static) num_threads(16)
        for (Index i = 0; i < N; i++)
        {
            X(i) = u(i) - CFL_*(Ddx_i(dx_, i, u, scheme_direction, true));
        } 
        return X; 
    }
    MatrixXf SolverWaveEquation::GenerateLinearSystem_EI(RowVectorXf u)
    {
        Index N = u.cols();
        MatrixXf A = MatrixXf::Zero(N,N);
        string scheme_direction = (c_ > 0)? "backward" : "forward";

        #pragma omp parallel for schedule(static) num_threads(16)
        for (Index i = 0; i < N; i++)
        {
            if (i == 0)
            {
                A(i, N-1) = (scheme_direction == "backward")? (-CFL_): 0;
                A(i, i) = (scheme_direction == "forward")? (CFL_) : 0;
            }
            else if(i == N-1)
            {
                A(i, 0) = (scheme_direction == "forward")? (CFL_) : 0;
                A(i, i-1) = (scheme_direction == "backward")? (-CFL_): 0;
            }
            else 
            {
                A(i, i-1) = (scheme_direction == "backward")? (-CFL_): 0;
                A(i, i+1) = (scheme_direction == "forward")? (CFL_) : 0;
            }
            A(i,i) = (scheme_direction == "backward")? (1+CFL_) : (1-CFL_);

        } 
        
        return A;
    }
    MatrixXf SolverWaveEquation::Solve(string scheme)
    {
        float dt = CFL_*dx_*c_;
        
        Index N_t = static_cast<Index>(std::floor(t_/dt));         
        Index N = Mesh_.cols();

        RowVectorXf time = VectorXf::LinSpaced(N_t, 0, t_);

        MatrixXf Results_u(N, N_t);
        Results_u.col(0) = u0.transpose();

        
        for (Index i = 0; i < N_t - 1; i++)
        {
            if(scheme == "E")
            {
                Results_u.col(i+1) = GenerateLinearSystem_EE(Results_u.col(i).transpose()); 
            }
            else if(scheme == "I")
            {
                MatrixXf A;
                MatrixXf b;
                A = GenerateLinearSystem_EI(Results_u.col(i).transpose());
                Results_u.col(i+1) = A.colPivHouseholderQr().solve(Results_u.col(i)); 
            }
        }
        Results.resize(N+1, N_t);
        Results << time,Results_u;
        return Results;
    }
    MatrixXf SolverWaveEquation::get_Results() {return Results;}
}
