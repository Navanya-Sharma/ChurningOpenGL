#pragma once
#include "Scene.h"

class Base : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

public:
	Base();
	void Init() override;
	void Update() override;
	void Close() override;
};

