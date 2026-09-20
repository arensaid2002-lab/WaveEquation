#include <iostream> 
#include <Eigen/Dense>
#include <fstream>
#include <locale>
#include <stdexcept>


using namespace Eigen;
using namespace std;
namespace NumMethods
{
   RowVectorXf SquareWave(float max_u, float min_u, RowVectorXf Mesh)
   {
      Index N = Mesh.cols();
      RowVectorXf u = RowVectorXf::Zero(N);
      for (int i = 0; i < N ; i++)
      {
         if(Mesh(i) > 40 && Mesh(i) < 60)
         {
            u(i) = max_u;
         }
         else 
         {
            u(i) = min_u;
         }
      }
      return u;
   }
   
   float Ddx_i(float dx, Index i, RowVectorXf u, string scheme = "center", bool CFL_controled = false) 
   {
      float Ddx;
      Index N = u.cols();
      if(scheme == "forward")
      {
         Ddx = (i != N-1)? ( u(i+1) - u(i))/dx : (u(0) - u(i))/dx;
      }
      else if(scheme == "center")
      {
         Ddx = (u(i+1) - u(i-1))/(2*dx);
      }
      else if(scheme == "backward")
      {
         Ddx = (i != 0)? (u(i) - u(i-1) )/dx : (u(i) - u(N-1) )/dx;
      }
      
      if (CFL_controled)
      {
         return Ddx*dx;
      }
      else
      {
         return Ddx;
      }
   }
//////////////// généré par chatGPT pour la sauvegarde de la matrice dans un fichier csv
   void saveMatrix(const Eigen::MatrixXf& matrix, const std::string& filename, const std::string& separator = ",")
   {
      std::ofstream file;
      file.exceptions(std::ios::failbit | std::ios::badbit);
      file.open(filename);  // Throws if the file cannot be opened

      file.imbue(std::locale::classic()); // Decimal point: 1.5

      Eigen::IOFormat format(
         Eigen::FullPrecision,
         Eigen::DontAlignCols,
         separator,
         "\n"
      );

      file << matrix.format(format) << '\n';
      file.close();
   }

}