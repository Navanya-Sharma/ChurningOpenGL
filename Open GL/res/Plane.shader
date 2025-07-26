#shader vertex
#version 460 core

layout (location = 0) in vec2 position;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP*vec4(position,0,1);
};

#shader fragment
#version 460 core

layout(location=0) out vec4 color;

void main()
{
	color=vec4(1,1,1,1);
};