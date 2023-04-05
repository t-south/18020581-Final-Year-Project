# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project(ge_cpp_dev_environment VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(GRAPHVIZ_EXTERNAL_LIBS FALSE)
# only dependencies between libraries
set(GRAPHVIZ_EXECUTABLES FALSE)

add_compile_definitions(OPENCV_VERSION=${OpenCV_VERSION})
add_compile_definitions(WITH_OPENCV2)
# Include sub-projects.
add_subdirectory(Game)
add_subdirectory(Engine)
add_custom_target(graphviz ALL
                  "${CMAKE_COMMAND}" "--graphviz=foo" .
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")