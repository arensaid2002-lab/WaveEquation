#pragma once

#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

namespace Solver
{
    class SolverHandler
    {
        public:
            SolverHandler(float CFL, RowVectorXf Mesh);
            ~SolverHandler(){};



            
        protected:
            std::string status_operation = "Generating matrices and calculating Values"; 
            std::string status_process_done = "The program finished succesfully";
            
            float dx_;
            float CFL_;
            RowVectorXf Mesh_;
            
            
            
    };
}