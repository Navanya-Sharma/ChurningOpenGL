#shader vertex
#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 inColor;

out vec3 vertColour;
out vec4 vertCord;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP*vec4(position,0.0f,1.0f);
	vertColour=inColor;
	vertCord=gl_Position;
};

#shader fragment
#version 460 core

in vec3 vertColour;
in vec4 vertCord;

out vec4 fragColor;

void main()
{
	fragColor=vec4(vertColour,1.0f);
	
};