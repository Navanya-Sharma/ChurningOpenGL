#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCord;

out vec3 vNorm;
out vec3 vPos;
out vec2 vTextCord;

uniform mat4 uMVP;

void main()
{
	gl_Position =uMVP*vec4(aPos,1.0f);
	vPos = (uMVP*vec4(aPos,1.0f)).xyz;
	vNorm = aNorm;
	vTextCord=aTexCord;
};








#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec3 vNorm;
in vec3 vPos;
in vec2 vTextCord;


struct material{
	sampler2D diffuse;
	sampler2D specular;
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
	vec3 ambient=uLight.ambient*vec3(texture(uMat.diffuse,vTextCord));

	vec3 lightDir = normalize(uLight.position- vPos);
	vec3 norm = normalize(vNorm);
	float perp=dot(norm, lightDir);
	vec3 diff = max(perp, 0.0)*vec3(texture(uMat.diffuse,vTextCord))*uLight.diffuse;

	vec3 viewDir = normalize(uVeiw - vPos);
	vec3 reflectDir = -lightDir+2.0f*perp*norm;
	float spec = 0.5*pow(max(dot(viewDir, reflectDir), 0.0), uMat.shine);
	vec3 specular= vec3(texture(uMat.specular,vTextCord))*uLight.specular*spec;


	fColor=vec4((ambient+diff+specular),1.0f);
	
};