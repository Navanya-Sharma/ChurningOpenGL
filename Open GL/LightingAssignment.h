#pragma once
#include "Scene.h"

class LightingAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	VertexArray	light_vertArr;
	VertexBuffer light_vertBuff;
	IndexBuffer	light_indBuff;
	Shader	light_shader;

	Shader gouraud_shader;

public:
	LightingAssignment();
	void Init() override;
	void Update() override;
	void UpdateImGui() override;
	void Close() override;
};

