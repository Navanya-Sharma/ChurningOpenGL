#pragma once
#include "Scene.h"

class Plane : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

public:
	Plane();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;
};

