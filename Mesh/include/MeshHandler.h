#pragma once
#include <Eigen/Dense>
using namespace Eigen;

namespace Mesh
{
   class  MeshHandler
    {
        public:
            MeshHandler(Index nb_element, float x_1, float x_2);
            ~MeshHandler();
            void GenerateMesh();
            
            RowVectorXf get_Mesh();

        private:

            RowVectorXf Mesh_;
            Index nb_element_;
            float x_1_;
            float x_2_;
    };
}
