#shader vertex
#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCord;

out vec3 vColor;
out vec2 vTexCord;

void main(){
	gl_Position=vec4(aPos,0.0f,1.0f);
	vColor=aColor;
	vTexCord=aTexCord;
}

//fragment
#shader fragment
#version 460 core

in vec3 vColor;
in vec2 vTexCord;

out vec4 fColor;

uniform sampler2D face;
uniform sampler2D wood;

void main(){
	//fColor=vec4(vColor,1.0f);
	//fColor=texture(wood,vTexCord);
	//fColor=texture(face,vTexCord);
	vec2 facecord=vec2(1-vTexCord.x,vTexCord.y);
	fColor=vec4(vColor,1.0f)*mix(texture(face,facecord),texture(wood,vTexCord),0.5);
}

