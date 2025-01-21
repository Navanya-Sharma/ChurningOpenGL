#shader vertex
#version 460 core

layout (location = 0) in vec4 position;

void main()
{
	gl_Position = vec4(position.x,position.y,0,1);
};

#shader fragment
#version 460 core

layout(location=0) out vec4 color;

void main()
{
	color=vec4(1,1,1,1);
};