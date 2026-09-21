add_library(Solver src/SolverHandler.cpp src/SolverWaveEquation.cpp)
#add_library(Solver STATIC )
target_include_directories(Solver
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Solver PUBLIC Eigen3::Eigen)
target_link_libraries(Solver PUBLIC NumMethods)
target_link_libraries(Solver PUBLIC Mesh)
target_link_libraries(Solver PUBLIC OpenMP::OpenMP_CXX)
