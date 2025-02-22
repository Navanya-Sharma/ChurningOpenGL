#pragma once
#include "Scene.h"
#include "glm/gtc/matrix_transform.hpp"

class Sphere : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Shader	light_shader;
	glm::mat4x4 proj;
	glm::mat4x4 view;
	glm::vec3 lightColor;

	void CameraImGui();
	void LightImGui();

public:
	Sphere();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;
};

