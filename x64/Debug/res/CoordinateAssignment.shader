#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP*vec4(aPos,1.0f);
	vColor = aColor;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec3 vColor;

void main()
{
	//fColor=texture(uText,vColor);
	fColor=vec4(vColor,0.0f);
};