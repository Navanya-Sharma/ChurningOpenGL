#pragma once
#include "Scene.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>


class ModelLoading : public Scene {
private:
	std::unique_ptr<VertexBuffer> m_vertBuff;
	VertexArray	m_vertArr;
	std::unique_ptr<IndexBuffer> m_indBuff;
	Shader	m_shader;
	Model Backpack;

public:
	ModelLoading();
	void Init() override;
	void Update() override;
	void Close() override;
};

