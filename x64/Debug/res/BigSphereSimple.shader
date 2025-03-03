#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 offset;


uniform mat4 uMVP;

void main(){
	gl_Position = uMVP*vec4(aPos+offset,1);
	
};