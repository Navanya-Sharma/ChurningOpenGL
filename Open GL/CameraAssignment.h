#pragma once
#include "Scene.h"
#include "Texture.h"

class CameraAssignment : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Texture m_texture;


public:
	CameraAssignment();
	void Init() override;
	void Update() override;
	void UpdateImGui() override;
	void Close() override;
};

