add_library(Mesh src/MeshHandler.cpp)

target_include_directories(Mesh
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Mesh PUBLIC Eigen3::Eigen)