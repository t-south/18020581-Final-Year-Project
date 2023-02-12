#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 uProjMat;
uniform mat4 uViewMat;
out vec4 ourColor;
out vec2 oTexCoord;
void main()
{
	oTexCoord = aTexCoord;
	ourColor = vec4(aColor, 1.0f); 
	gl_Position = uProjMat * uViewMat * vec4(aPos, 1.0);
}