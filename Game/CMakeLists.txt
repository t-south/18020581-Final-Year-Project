# CMakeList.txt : CMake project for GameEngineProject, include source and define
# project specific logic here.
#

# Add source to this project's executable.

cmake_minimum_required(VERSION 3.8)
project(ge_cpp_application VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} src/Game.cpp)
target_link_libraries(${PROJECT_NAME} ge_cpp_engine)
target_include_directories(${PROJECT_NAME} PRIVATE ge_cpp_engine)

#add_executable (GameEngineProject "GameEngineProject.cpp" "GameEngineProject.h")

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET GameEngineProject PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
