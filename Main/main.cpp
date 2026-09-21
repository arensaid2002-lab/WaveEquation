#include <iostream>
#include <Eigen/Dense>
#include "NumMethods.h"
#include "MeshHandler.h"
#include "SolverWaveEquation.h"


using namespace Solver;
using namespace NumMethods;
int main(int, char**)
{
    
    Index nb_element = 100;
    float x1 = 0;
    float x2 = 100;

    float c = 300;
    float t = 10;
    float CFL = 0.5;

    float max_u = 100;
    float min_u = 0;
    
    Mesh::MeshHandler meshHandler(nb_element, x1, x2);
    meshHandler.GenerateMesh();

    RowVectorXf Mesh = meshHandler.get_Mesh();
    RowVectorXf u0 = SquareWave(max_u, min_u, Mesh);

    SolverWaveEquation WaveEquation(c, t,CFL, Mesh, u0);
    
    WaveEquation.Solve("E");
    
    std::cout << WaveEquation.get_Results() ;

    NumMethods::saveMatrix(WaveEquation.get_Results(), "results.csv", ",");  // Comma-separated

}

