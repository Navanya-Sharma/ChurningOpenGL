#pragma once
#include "Scene.h"

class ShaderAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

public:
	ShaderAssignment();
	void Init() override;
	void Update() override;
	void Close() override;
};

