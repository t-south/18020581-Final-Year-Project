cmake_minimum_required(VERSION 3.8)
project(ge_cpp_engine VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_subdirectory(External/glm)
add_subdirectory(External/GLFW)
add_subdirectory(External/spdlog)
add_subdirectory(External/GLAD)
add_subdirectory(External/box2d)
file(GLOB_RECURSE sources "src/*.cpp")
add_library(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} 
	PUBLIC	
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		glad
		ImGui
		spdlog
		glfw			
		glm
		box2d
		${OPENGL_LIBRARIES}
		
		
)
set(IMGUI_PATH  "External/imgui")
set(IMGUI_BACKENDS "External/imgui/backends")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp) 
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_link_libraries(${PROJECT_NAME} 
	PUBLIC 
		glad
		ImGui		
		spdlog	
		glfw		
		glm
		box2d
		${OPENGL_LIBRARIES}	
		
)






