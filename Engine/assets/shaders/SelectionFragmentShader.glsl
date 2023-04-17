#version 450 core
in vec4 ourColor;
in vec2 oTexCoords;
in float oTexId;
in float oEntity;

layout(binding = 1) uniform sampler2D texture1;
layout(binding = 2) uniform sampler2D texture2;
layout(binding = 3) uniform sampler2D texture3;
layout(binding = 4) uniform sampler2D texture4;
layout(binding = 5) uniform sampler2D texture5;
layout(binding = 6) uniform sampler2D texture6;
layout(binding = 7) uniform sampler2D texture7;
layout(binding = 8) uniform sampler2D texture8;
out vec3 FragColor;


void main()
{	
	vec4 texColor = vec4(1, 1, 1, 1);
	int id = int(oTexId);
	
	if(id == 0){
		texColor = ourColor;
	}
	if(id == 1){
		texColor = texture(texture1, oTexCoords);
	}
	if(id == 2){
		texColor = texture(texture2, oTexCoords);
	}
	if(id == 3){
		texColor = texture(texture3, oTexCoords);
	}
	if(id == 4){
		texColor = texture(texture4, oTexCoords);
	}
	if(id == 5){
		texColor = texture(texture5, oTexCoords);
	}
	if(id == 6){
		texColor = texture(texture6, oTexCoords);
	}
	if(id == 7){
		texColor = texture(texture7, oTexCoords);
		
	}
	
	if(texColor.a < 0.5){
		discard;
	}
	FragColor = vec3(oEntity, oEntity, oEntity);
	//if(id > 0){		
		//FragColor = ourColor;
		//FragColor = ourColor * texture(textures[id], oTexCoord);
		//FragColor = texture(textures[id], oTexCoords);
	    //FragColor = texture(textures, vec3(oTexCoords.xy, oTexId));
	

};