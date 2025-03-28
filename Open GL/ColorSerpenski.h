#pragma once
#include "Scene.h"

class ColorSerpenski : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	void DrawSerpent(glm::vec2 center, float scale, int depth);

public:
	ColorSerpenski();
	void Init() override;
	void Update() override;
	void Close() override;
};

