#include "SolverHandler.h"
#include <Eigen/Dense>
#include <iostream>

namespace Solver
{
    SolverHandler::SolverHandler(float CFL, RowVectorXf Mesh)
    {
        dx_ = Mesh(1) - Mesh(0);
        CFL_ = CFL;
        Mesh_ = Mesh;
    }

}