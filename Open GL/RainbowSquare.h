#pragma once
#include "Scene.h"
//#include "VertexArray.h"
//#include "IndexBuffer.h"

class RainbowSquare : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	float m_color[3];
	void UpdateColor();

public:
	RainbowSquare();
	void Init() override;
	void Update(Renderer* gRenderer) override;
	void Close() override;
};