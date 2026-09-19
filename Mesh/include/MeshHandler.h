#include <Eigen/Dense>
using namespace Eigen;
namespace Mesh
{
    class MeshHandler
    {
        public:
            MeshHandler(float nb_element, float x_1, float x_2){};
            ~MeshHandler();
            RowVectorXf GenerateMesh();
        private:
            float nb_element_;
            int x_1_;
            int x_2_;
    };
}
