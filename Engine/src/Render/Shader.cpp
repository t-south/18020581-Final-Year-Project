#include "Shader.h"


geProject::Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	vertexSrc = readFromFile(vertexFile);
	fragmentSrc = readFromFile(fragmentFile);
	configureShader();

}

geProject::Shader::~Shader() {}

std::string geProject::Shader::readFromFile(const char* filePath){
	
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(filePath);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();		
		return fileStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}





//Credit to https ://stackoverflow.com/questions/2663220/opengl-glsl-checking-if-shader-compiled-fine-on-intel-cards
bool geProject::Shader::checkLink(int id) {
	int status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
}

bool geProject::Shader::checkCompile(int id) {
	int status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}




void geProject::Shader::configureShader() {
	unsigned int vertexShader, fragmentShader;
	//vertex shader compile and error check
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vert = vertexSrc.c_str();	
	glShaderSource(vertexShader, 1, &vert, NULL);
	glCompileShader(vertexShader);
	if (!checkCompile(vertexShader)) {
		GLint maxlength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxlength);
		std::vector<GLchar> errorLog(maxlength);
		glGetShaderInfoLog(vertexShader, maxlength, &maxlength, &errorLog[0]);
		std::cout << "Error: vertex compilation failed" << std::endl;
		for (auto i : errorLog)
			std::cout << i;
		glDeleteShader(vertexShader);
		return;
	}

	//fragment shader compile and error check
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* frag = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &frag, NULL);
	glCompileShader(fragmentShader);
	if (!checkCompile(fragmentShader)) {
		GLint maxlength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxlength);
		std::vector<GLchar> errorLog(maxlength);
		glGetShaderInfoLog(fragmentShader, maxlength, &maxlength, &errorLog[0]);
		std::cout << "Error: fragment compilation failed" << std::endl;
		for (auto i : errorLog)
			std::cout << i;
		glDeleteShader(fragmentShader);
		return;
	}

	//link vertex and fragment shader into shader program and error check
	shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);
	if (!checkLink(shaderId)) {
		GLint maxlength = 0;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxlength);
		std::vector<GLchar> errorLog(maxlength);
		glGetProgramInfoLog(shaderId, maxlength, &maxlength, &errorLog[0]);
		std::cout << "Error: Linking failed" << std::endl;
		for (auto i : errorLog)
			std::cout << i;
		glDeleteProgram(shaderId);
		return;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}




//credit to https ://learnopengl.com/Getting-started/Shaders
void geProject::Shader::setBool(const std::string& name, bool value) 
{
	geProject::Shader::use();
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
}
void geProject::Shader::setInt(const std::string& name, int value) 
{
	geProject::Shader::use();
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}
void geProject::Shader::setFloat(const std::string& name, float value) 
{
	geProject::Shader::use();
	glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}


void geProject::Shader::setMat4f(const std::string& name, glm::mat4 value) 
{
	geProject::Shader::use();
	glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}


void geProject::Shader::setTexture(const std::string& name, unsigned int value) 
{
	geProject::Shader::use();
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void geProject::Shader::setFloatArray(const std::string& name, float value[])
{
	geProject::Shader::use();
	glUniform1fv(glGetUniformLocation(shaderId, name.c_str()), sizeof(value), &value[0]);
}

void geProject::Shader::setIntArray(const std::string& name, int value[])
{	
	geProject::Shader::use();
	glUniform1iv(glGetUniformLocation(shaderId, name.c_str()), sizeof(value), &value[0]);
}


void geProject::Shader::use() {
	glUseProgram(shaderId);
}

void geProject::Shader::detach() {
	glUseProgram(0);
}