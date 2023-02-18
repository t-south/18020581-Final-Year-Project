#version 330 core
in vec4 ourColor;
//in vec2 oTexCoord;
//uniform sampler2D textureSampler;
out vec4 FragColor;
void main()
{
	FragColor = ourColor;//texture(textureSampler, oTexCoord);
};