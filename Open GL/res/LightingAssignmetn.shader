#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP*vec4(aPos,1.0f);
};

#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

uniform vec4 uLight;

void main()
{
	fColor=uLight;
};