#pragma once
#include "Scene.h"
#include "Cube.h"

class Collision: public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	VertexArray	m_vertArr2;
	VertexBuffer m_vertBuff2;
	IndexBuffer	m_indBuff;
	Shader	m_shader;
	Cube biki;
	Cube guria;
	float e;

public:
	Collision();
	bool CheckCollision();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;
};

