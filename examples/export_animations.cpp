// Exporte les deux schemas avec les memes parametres, via le solveur du projet.
#include "MeshHandler.h"
#include "NumMethods.h"
#include "SolverWaveEquation.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <stdexcept>
#include <string>

int main()
{
    try
    {
        const Eigen::Index points = 100;
        const float x_min = 0.0f, x_max = 100.0f;
        const float c = 300.0f, duration = 10.0f, cfl = 0.5f;
        const float u_max = 100.0f, u_min = 0.0f;

        Mesh::MeshHandler mesh_handler(points, x_min, x_max);
        mesh_handler.GenerateMesh();
        const Eigen::RowVectorXf mesh = mesh_handler.get_Mesh();
        const Eigen::RowVectorXf initial = NumMethods::SquareWave(u_max, u_min, mesh);
        const float dx = mesh(1) - mesh(0);
        const float dt = std::abs(cfl * dx / c);

        NumMethods::saveMatrix(mesh, "mesh.csv", ",");
        for (const std::string scheme : {std::string("E"), std::string("I")})
        {
            Solver::SolverWaveEquation solver(c, duration, cfl, mesh, initial);
            const Eigen::MatrixXf results = solver.Solve(scheme);
            if (!results.allFinite() || results.rows() != points + 1 || results.cols() < 2)
                throw std::runtime_error("Resultats invalides pour le schema " + scheme);

            const std::string filename = scheme == "E" ? "explicit.csv" : "implicit.csv";
            NumMethods::saveMatrix(results, filename, ",");
            std::cout << filename << " : " << results.rows() << " lignes, "
                      << results.cols() << " colonnes\n";
        }

        std::ofstream metadata;
        metadata.exceptions(std::ios::failbit | std::ios::badbit);
        metadata.open("parameters.json");
        metadata.imbue(std::locale::classic());
        metadata << std::setprecision(std::numeric_limits<float>::max_digits10)
                 << "{\n  \"points\": " << points
                 << ",\n  \"x_min\": " << x_min << ",\n  \"x_max\": " << x_max
                 << ",\n  \"c\": " << c << ",\n  \"cfl\": " << cfl
                 << ",\n  \"requested_duration\": " << duration
                 << ",\n  \"dx\": " << dx << ",\n  \"dt\": " << dt
                 << ",\n  \"u_min\": " << u_min << ",\n  \"u_max\": " << u_max
                 << "\n}\n";
        metadata.close();
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
