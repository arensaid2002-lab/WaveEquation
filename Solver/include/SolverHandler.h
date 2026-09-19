#include <Eigen/Dense>
#include <iostream> 
using namespace Eigen;

namespace Solver
{
    class SolverHandler
    {
        public:
            SolverHandler(float tol, float step_size, RowVectorXf Mesh ){};
            ~SolverHandler(){};

            MatrixXf Solve(RowVectorXf Mesh);


            
        protected:
            std::string status_operation = "Generating matrices and calculating Values"; 
            std::string status_process_done = "The program finished succesfully";
            
            float tol_;
            float step_size_;
            RowVectorXf Mesh_;
            RowVectorXf initial_conditions_;
            
            
    };
}