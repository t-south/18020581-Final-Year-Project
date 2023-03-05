#version 450 core
in vec4 ourColor;
in vec2 oTexCoords;
in float oTexId;
layout(binding = 1) uniform sampler2D texture1;
layout(binding = 2) uniform sampler2D texture2;
layout(binding = 3) uniform sampler2D texture3;
layout(binding = 4) uniform sampler2D texture4;
layout(binding = 5) uniform sampler2D texture5;
layout(binding = 6) uniform sampler2D texture6;
layout(binding = 7) uniform sampler2D texture7;
layout(binding = 8) uniform sampler2D texture8;
out vec4 FragColor;


void main()
{	
	int id = int(oTexId);
	
	if(id == 0){
		FragColor = ourColor;
	}
	if(id == 1){
		FragColor = texture(texture1, oTexCoords);
	}
	if(id == 2){
		FragColor = texture(texture2, oTexCoords);
	}
	if(id == 3){
		FragColor = texture(texture3, oTexCoords);
	}
	if(id == 4){
		FragColor = texture(texture4, oTexCoords);
	}
	if(id == 5){
		FragColor = texture(texture5, oTexCoords);
	}
	if(id == 6){
		FragColor = texture(texture6, oTexCoords);
	}
	if(id == 7){
		FragColor = texture(texture7, oTexCoords);
		
	}
	
	
	//if(id > 0){		
		//FragColor = ourColor;
		//FragColor = ourColor * texture(textures[id], oTexCoord);
		//FragColor = texture(textures[id], oTexCoords);
	    //FragColor = texture(textures, vec3(oTexCoords.xy, oTexId));
	

};