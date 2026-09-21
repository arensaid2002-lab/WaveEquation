#include "SolverHandler.h"
#include <Eigen/Dense>
#include <iostream>

using namespace std;

namespace Solver
{
    class SolverWaveEquation : public SolverHandler 
    {
        public:
            SolverWaveEquation(float c, float t, float CFL, RowVectorXf Mesh, RowVectorXf u_0);
            VectorXf GenerateLinearSystem_EE(RowVectorXf u);
            MatrixXf GenerateLinearSystem_EI(RowVectorXf u);
            MatrixXf Solve(string scheme);
            MatrixXf get_Results();
        private:

            float c_; 
            float t_;

            RowVectorXf u0;
            MatrixXf Results;
   
    };

}
