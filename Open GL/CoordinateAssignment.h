#pragma once
#include "Scene.h"
#include "Texture.h"

class CoordinateAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Texture m_texture;


public:
	CoordinateAssignment();
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void UpdateImGui() override;
	void Close() override;
};

