#shader vertex
#version 460 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = position;
	v_TexCoord = TexCoord;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 color;
in vec2 v_TexCoord;

//uniform vec4 u_color;
uniform usampler2D u_texture;

void main()
{
	vec4 data=texture(u_texture, v_TexCoord);
	float a = data.r; 

	color = vec4(a,a,a,a);

};