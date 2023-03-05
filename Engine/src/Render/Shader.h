#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace geProject {
	class Shader {
	public:
		Shader(const char* vertexFile, const char* fragmentFile);
		~Shader();
		void use();
		void detach();
		void setBool(const std::string& name, bool value) ;
		void setInt(const std::string& name, int value) ;
		void setFloat(const std::string& name, float value) ;
		void setMat4f(const std::string& name, glm::mat4 value) ;
		void setTexture(const std::string& name, unsigned int val) ;
		void setFloatArray(const std::string& name, float value[]);
		void setIntArray(const std::string& name, int value[]);
	private:
		unsigned int shaderId;
		std::string vertexSrc;
		std::string fragmentSrc;
		std::string readFromFile(const char* filePath);
		bool checkCompile(int id);
		bool checkLink(int id);
		void configureShader();
		
	};
}