cmake_minimum_required(VERSION 3.10.0)
project(WaveEquation VERSION 0.1.0 LANGUAGES C CXX)

#=== Dependencies ===#
find_package(Kokkos CONFIG QUIET)
find_package(Eigen3 CONFIG QUIET)
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

IF(NOT Kokkos_FOUND)
    message(STATUS "Kokkos not found, downloading and building Kokkos"
    )
    include(FetchContent)
    FetchContent_Declare(
        kokkos
        GIT_REPOSITORY https://github.com/kokkos/kokkos.git
        GIT_TAG 5.2.2
    )
    FetchContent_MakeAvailable(kokkos)
ENDIF()


add_subdirectory(Mesh)
add_subdirectory(Solver)
add_subdirectory(NumMethods)


add_executable(WaveEquation Main/main.cpp)

# target_link_libraries(WaveEquation PRIVATE Mesh)
# target_link_libraries(WaveEquation PRIVATE Solver)
#target_include_directories(WaveEquation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Mesh/include)
