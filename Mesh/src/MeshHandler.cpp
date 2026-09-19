#include "MeshHandler.h"
#include <Eigen/Dense>
using namespace Eigen;


namespace Mesh
{
    MeshHandler::MeshHandler(float nb_elements,  float x_1, float x_2)
    {
        nb_element_ = nb_elements;
        x_1_ = x_1;
        x_2_ = x_2;
    }
    
    RowVectorXf MeshHandler::GenerateMesh()
    {
        RowVectorXf position = RowVectorXf::LinSpaced(nb_element_, x_1_, x_2_);
        return position;
    }
}