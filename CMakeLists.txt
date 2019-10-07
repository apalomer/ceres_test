# Minimum version required
cmake_minimum_required(VERSION 2.8)

# Project Name
project(ceres_test)

# Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIR})
include_directories(C:/ProgramData/Miniconda3/Library/include)

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN_INCLUDE_DIRS})
add_definitions(${EIGEN_DEFINITIONS})

if (WIN32)
  add_compile_definitions(_DISABLE_EXTENDED_ALIGNED_STORAGE)
  #add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

# Hello world
add_executable(hello_world src/hello_world.cpp)
target_link_libraries(hello_world
    ceres
)

# Test
add_executable(test_ceres src/test.cpp)
target_link_libraries(test_ceres
    ceres
)
