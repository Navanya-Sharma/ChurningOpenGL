#pragma once
#include "Scene.h"
#include "Texture.h"

class TransformAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Texture m_texture;

public:
	TransformAssignment();
	void Init() override;
	void Update() override;
	void Close() override;
};

