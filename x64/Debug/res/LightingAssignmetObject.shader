#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 vNorm;
out vec3 vPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
	gl_Position = uMVP*vec4(aPos,1.0f);
	vPos = (uModel*vec4(aPos,1.0f)).xyz;
	vNorm = aNorm;
};

#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec3 vNorm;
in vec3 vPos;

struct material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shine;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform material uMat;
uniform Light uLight;
uniform vec3 uVeiw;


void main()
{
	vec3 ambient=uLight.ambient*uMat.ambient;

	vec3 lightDir = normalize(uLight.position- vPos);
	vec3 norm = normalize(vNorm);
	float perp=dot(norm, lightDir);
	vec3 diff = max(perp, 0.0)*uMat.diffuse*uLight.diffuse;

	vec3 viewDir = normalize(uVeiw - vPos);
	vec3 reflectDir = -lightDir+2.0f*perp*norm;
	float spec = 0.5*pow(max(dot(viewDir, reflectDir), 0.0), uMat.shine);
	vec3 specular= uMat.specular*uLight.specular*spec;


	fColor=vec4((ambient+diff+specular),1.0f);
	
};