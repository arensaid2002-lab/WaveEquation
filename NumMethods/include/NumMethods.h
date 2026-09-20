#include <Eigen/Dense>
#include <iostream>

using namespace std;
using namespace Eigen;
namespace NumMethods
{
    RowVectorXf SquareWave(float max_u, float min_u, RowVectorXf Mesh);
    float Ddx_i(float dx, Index i, RowVectorXf u, string scheme = "center", bool CFL_controled = false);
    void saveMatrix(const Eigen::MatrixXf& matrix, const std::string& filename, const std::string& separator = ",");
}