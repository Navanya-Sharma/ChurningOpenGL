#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 sNorm;

void main(){
	gl_Position = vec4(aPos,1);
	sNorm =aNorm;
};

#shader geometry
#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

in vec3 sNorm[];

out vec3 vNorm;
out vec3 vPos;
out vec3 color;

struct vert{
	vec3 Pos;
	vec3 Norm;
	};

uniform mat4 uMVP;
uniform mat4 uModel;

void EmitTriangle(vert v0, vert v1, vert v2){
	vert v[3]={v0,v1,v2};
	
	for(int i = 0 ; i < 3; i++){
		gl_Position = uMVP * vec4(normalize(v[i].Pos),1.0f);
		vNorm = v[i].Norm;
		vPos = (uModel * vec4(v[i].Pos,1.0f)).xyz;
		EmitVertex();
	}
	EndPrimitive();
}

vert MidVert(vert v0, vert v1){
	vert v;
	v.Pos = (v0.Pos + v1.Pos) / 2;
	v.Norm = normalize(v.Pos);
	
	return v;
}

void MakeFuller(vert v0, vert v1, vert v2, int depth){
	if(depth==1){
		EmitTriangle(v0,MidVert(v1,v0),MidVert(v2,v0));
		EmitTriangle(v1,MidVert(v2,v1),MidVert(v0,v1));
		EmitTriangle(v2,MidVert(v0,v2),MidVert(v1,v2));
		EmitTriangle(MidVert(v2,v0),MidVert(v1,v0),MidVert(v2,v1));
		return;
	}

	vert v01 = MidVert(v0,v1);
	vert v12 = MidVert(v1,v2);
	vert v20 = MidVert(v2,v0);
	

}

void main()
{
	// 2
	//0 1
	
	vert tri[3];
	for (int i=0;i<3;i++){
		tri[i].Pos=gl_in[i].gl_Position.xyz;
		tri[i].Norm=sNorm[i];
	}
	
	MakeFuller(tri[0],tri[1],tri[2],1);
	
};


#shader fragment
#version 460 core

layout(location=0) out vec4 fColor;

in vec3 vNorm;
in vec3 vPos;
in vec3 color;

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
	//fColor=vec4(color,1.0f);
};