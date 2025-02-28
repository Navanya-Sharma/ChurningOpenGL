#pragma once
#include "Scene.h"
#include "Texture.h"
#include "frustCull.h"

class LightingMaps : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	VertexArray m_PointArr;
	VertexBuffer m_PointsBuff;

	VertexArray	light_vertArr;
	VertexBuffer light_vertBuff;
	IndexBuffer	light_indBuff;
	Shader	light_shader;

	Texture diffuseMap;
	Texture specularMap;

	FrustCull CamFrust;

	void checkPoints();

public:
	LightingMaps();
	void Init() override;
	void Update() override;
	void UpdateImGui() override;
	void Close() override;
};

