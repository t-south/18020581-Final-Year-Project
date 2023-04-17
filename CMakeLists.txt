# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)
project(ge_cpp_application VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} Engine/src/Entry/application.cpp)
target_link_libraries(${PROJECT_NAME} ge_cpp_engine)
target_include_directories(${PROJECT_NAME} PRIVATE ge_cpp_engine)
# Include sub-projects.
#add_subdirectory(Game)
add_subdirectory(Engine)


install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib
                      ARCHIVE DESTINATION lib
                      RUNTIME DESTINATION bin
                      COMPONENT library)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")
include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

include(CPack)