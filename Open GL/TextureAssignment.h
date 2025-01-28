#pragma once
#include "Scene.h"
#include "Texture.h"

class TextureAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	Texture face;
	Texture wood;

public:
	TextureAssignment();
	void Init() override;
	void Update() override;
	void Close() override;
};
