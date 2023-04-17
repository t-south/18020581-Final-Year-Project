#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
out vec3 ourColor;

void main()
{
	ourColor = aColor; 
	gl_Position = uProjMat * uViewMat * vec4(aPos, 1.0);
}