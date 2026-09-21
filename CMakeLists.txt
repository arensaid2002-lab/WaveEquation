cmake_minimum_required(VERSION 3.10.0)
project(WaveEquation VERSION 0.1.0 LANGUAGES C CXX)

#=== Dependencies ===#

find_package(Eigen3 CONFIG QUIET)
find_package(OpenMP REQUIRED COMPONENTS CXX)

IF(NOT Eigen3_FOUND)
    message(STATUS "Eigen not found, downloading and building Kokkos")
    include(FetchContent)
    FetchContent_Declare(
        Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 5.0.1
    )
    FetchContent_MakeAvailable(Eigen)
ENDIF()

add_subdirectory(Mesh)
add_subdirectory(Solver)
add_subdirectory(NumMethods)

#target_include_directories(WaveEquation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Mesh/include)
add_executable(WaveEquation Main/main.cpp)
target_link_libraries(WaveEquation PRIVATE Mesh Solver Eigen3::Eigen OpenMP::OpenMP_CXX)

option(WAVE_EQUATION_BUILD_ANIMATIONS "Build the CSV exporter for the README animations" OFF)
if(WAVE_EQUATION_BUILD_ANIMATIONS)
    add_executable(WaveEquationAnimations examples/export_animations.cpp)
    target_link_libraries(WaveEquationAnimations PRIVATE Solver)
endif()
