#pragma once
#include "Scene.h"
#include "Fuller.h"
#include "glm/gtc/matrix_transform.hpp"

class Spheree : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Shader	light_shader;
	glm::mat4x4 proj;
	glm::mat4x4 view;
	glm::vec3 lightColor;

	Fuller ico;

	void CameraImGui();
	void LightImGui();

public:
	Spheree();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;
};

