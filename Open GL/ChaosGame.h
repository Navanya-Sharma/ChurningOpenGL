#pragma once
#include "Scene.h"
#include "Renderer.h"
#include "Texture.h"


class ChaosGame : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	VertexArray	 m_pointsArr;
	VertexBuffer m_pointsBuff;
	Shader	     m_shaderCompute;
	Texture m_texture;

public:
	ChaosGame();
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void Close() override;
};