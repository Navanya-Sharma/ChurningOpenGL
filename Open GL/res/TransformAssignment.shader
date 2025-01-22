#shader vertex
#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCord;

out vec2 v_TexCord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP*vec4(position,0.0f,1.0f);
	v_TexCord = texCord;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 color;

in vec2 v_TexCord;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor= texture(u_Texture, v_TexCord);
	color=texColor;
};