cmake_minimum_required(VERSION 3.8)
project(ge_cpp_engine VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


find_package(OpenGL REQUIRED)
add_subdirectory(External/GLFW)
add_subdirectory(External/spdlog)

file(GLOB_RECURSE sources "src/*.cpp")
add_library(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} 
	PUBLIC	
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		spdlog
		glfw
		${OPENGL_LIBRARIES}
		
	PRIVATE
		
)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC 
		spdlog
		glfw		
		${OPENGL_LIBRARIES}	
	PRIVATE
		
)
