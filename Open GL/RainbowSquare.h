#pragma once
#include "Scene.h"
#include "Picking_Technique.h"
#include "Picking_Texture.h"
//#include "IndexBuffer.h"

class RainbowSquare : public Scene {
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;
	Shader	m_shader;

	float m_color[3];
	void UpdateColor();
	struct Click {
		bool IsPressed = false;
		int x;
		int y;
	} m_leftMouseButton;
	PickingTexture m_pickingTexture;
	//PickingTechnique m_pickingEffect;

	void PickingPhase();

public:
	RainbowSquare();
	void Init() override;
	void Update() override;
	void Close() override;
	void UpdateImGui() override;

};