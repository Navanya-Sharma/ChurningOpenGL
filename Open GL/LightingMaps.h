#pragma once
#include "Scene.h"
#include "Texture.h"

class LightingMaps : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	VertexArray	light_vertArr;
	VertexBuffer light_vertBuff;
	IndexBuffer	light_indBuff;
	Shader	light_shader;

	Texture diffuseMap;
	Texture specularMap;

public:
	LightingMaps();
	void Init() override;
	void Update() override;
	void UpdateImGui() override;
	void Close() override;
};

