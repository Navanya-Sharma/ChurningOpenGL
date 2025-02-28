#pragma once
#include "Scene.h"

class GPUInstancing : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	VertexBuffer m_InstanceBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

public:
	GPUInstancing();
	void Init() override;
	void Update() override;
	void Close() override;
};

