#include "MeshHandler.h"
#include <Eigen/Dense>
using namespace Eigen;


namespace Mesh
{
    MeshHandler::MeshHandler(Index nb_elements, float x_1, float x_2)
    {
        nb_element_ = nb_elements;
        x_1_ = x_1;
        x_2_ = x_2;
    }
    
    MeshHandler::~MeshHandler(){}
    
    void MeshHandler::GenerateMesh()
    {
        Mesh_ = RowVectorXf::LinSpaced(nb_element_, x_1_, x_2_);
    }
    RowVectorXf MeshHandler::get_Mesh(){return Mesh_;}
}