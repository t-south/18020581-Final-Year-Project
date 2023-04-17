#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexId;
layout (location = 4) in float aEntity;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
out vec4 ourColor;
out vec2 oTexCoords;
out float oTexId;
out float oEntity;
//out vec2 oTexCoord;
void main()
{
	oEntity = aEntity;
	ourColor = aColor; 
	oTexCoords = aTexCoord;
	oTexId = aTexId;
	gl_Position = uProjMat * uViewMat * vec4(aPos, 1.0);
}