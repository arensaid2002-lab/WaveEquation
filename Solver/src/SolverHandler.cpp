#include "SolverHandler.h"
#include <Eigen/Dense>
#include <iostream>

namespace Solver
{
    SolverHandler::SolverHandler(float tol, float step_size, RowVectorXf Mesh)
    {
        tol_ = tol;
        step_size_ = step_size;
        Mesh_ = Mesh;
    }
    MatrixXf SolverHandler::Solve(RowVectorXf Mesh)
    {
        // il fait quelque chose
    }
}