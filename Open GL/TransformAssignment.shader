#shader vertex
#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCord;

out vec2 vTexCord;

uniform mat4 uMVP;

void main()
{
	glPosition = uMVP*vec4(position,0.0f,1.0f);
	vTexCord = texCord;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec2 vTexCord;

uniform sampler2D uTexture;

void main()
{
	vec4 texColor= texture(uTexture, vTexCord);
	color=texColor;
};