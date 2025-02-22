#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec4 vColor;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform vec4 uLPos;
uniform vec4 uLight;
uniform vec4 uVeiw;

void main()
{
	gl_Position = uMVP*vec4(aPos,1.0f);
	vec3 vPos = (uModel*vec4(aPos,1.0f)).xyz;

	vec3 uLightPos = uLPos.xyz;
	float ambient=0.5f;

	vec3 lightDir = normalize(uLightPos - vPos);
	vec3 norm = normalize(aNorm);
	float perp=dot(norm, lightDir);
	float diff = max(perp, 0.0);

	vec3 viewDir = normalize(uVeiw.xyz - vPos);
	vec3 reflectDir = -lightDir+2.0f*perp*norm;
	float spec = 0.5*pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vColor=vec4(0.6f,0.0f,0.0f,1.0f)*uLight*(ambient+diff+spec);
	
};

#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec4 vColor;

void main()
{
	fColor=vColor;
	
};