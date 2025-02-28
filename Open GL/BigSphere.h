#pragma once
#include "Scene.h"
#include "Fuller.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FrustCull.h"

class BigSphere : public Scene {
private:
	
	VertexArray	vertArr2[11];
	VertexBuffer vertBuff2[11];
	VertexBuffer instBuff;
	IndexBuffer	indBuff2[11];

	VertexArray m_PointArr;
	VertexBuffer m_PointsBuff;

	Shader	m_shader;
	Shader simple;
	Shader	light_shader;
	glm::mat4x4 proj;
	glm::vec3 viewPos;
	glm::vec3 lightColor;

	void CameraImGui();
	void LightImGui();

	Fuller ico2;
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shine;
	};
	static const Material materials[15];

	FrustCull CamFrust;
	void FrustRender(glm::vec3 BotLeftF, float dis, int depth);
	void SimpleFrustRender(glm::vec3 BotLeftF, float dis, int depth);
	void Render(glm::vec3 BotLeftF, float dis, int depth);
	void SimpleRender(glm::vec3 BotLeftF, float dis, int depth);
	void checkPoints();

public:
	BigSphere();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;
};

