#include "SolverHandler.h"
#include <Eigen/Dense>
#include <iostream>

namespace Solver
{
    class SolverWaveEquation : public SolverHandler 
    {
        public:
            SolverWaveEquation(float CF, float tol, float step_size, RowVectorXf Mesh);
            void GenerateMatrices_Axb();
            void Solve();
        private:
            float CF_;
            MatrixXf A;
            MatrixXf b;


            
    };

}
