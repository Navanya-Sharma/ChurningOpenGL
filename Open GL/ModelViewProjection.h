#pragma once
#include "Scene.h"
#include "Renderer.h"
#include "Texture.h"

class ModelViewProjection : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Texture m_texture;

public:
	ModelViewProjection();
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void Close() override;
};