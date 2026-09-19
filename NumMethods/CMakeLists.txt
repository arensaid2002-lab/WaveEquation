add_library(NumMethods STATIC src/NumMethods.cpp)
target_include_directories(NumMethods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_link_libraries(NumMethods PUBLIC Eigen3::Eigen)