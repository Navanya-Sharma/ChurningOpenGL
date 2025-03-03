#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 offset;

out vec3 vPos;

void main()
{
	gl_Position = vec4((aPos)* (gl_InstanceID / 100.0)+vec3(offset,0),1);
	vPos=aPos;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 color;

in vec3 vPos;

void main()
{
	color=vec4(1,1,1,1);
};
