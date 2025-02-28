#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 offset;

out vec3 vNorm;
out vec3 vPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main(){
	gl_Position = uMVP*vec4(aPos,1);
	vNorm = (uModel*vec4(aNorm,0)).xyz;
	vPos = (uModel*vec4(aPos,1)).xyz;
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
	float shine;};
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;};

uniform float uMatArr[10];
uniform Light uLight;
uniform vec3 uVeiw;

void main()
{
	vec3 uAmbient=vec3(uMatArr[0],uMatArr[1],uMatArr[2]);
	vec3 uDiffuse=vec3(uMatArr[3],uMatArr[4],uMatArr[5]);
	vec3 uSpecular=vec3(uMatArr[6],uMatArr[7],uMatArr[8]);
	float uShine=uMatArr[9];

	vec3 ambient=uLight.ambient*uAmbient;
	vec3 lightDir = normalize(uLight.position- vPos);
	vec3 norm = normalize(vNorm);
	float perp=dot(norm, lightDir);
	vec3 diff = max(perp, 0.0)*uDiffuse*uLight.diffuse;

	vec3 viewDir = normalize(uVeiw - vPos);
	vec3 reflectDir = -lightDir+2.0f*perp*norm;
	float spec = 0.5*pow(max(dot(viewDir, reflectDir), 0.0), uShine);
	vec3 specular= uSpecular*uLight.specular*spec;


	fColor=vec4((ambient+diff+specular),1.0f);
	
};