#include "ModelLoading.h"
#include "BufferLayout.h"



ModelLoading::ModelLoading() :
	m_vertArr(), m_vertBuff(NULL, 0 * sizeof(float)),
	m_indBuff(NULL, 0 * sizeof(unsigned int)), m_shader("res/ModelLoading.shader")
{
}

void ModelLoading::Init()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("res/backpack/backpack.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
}

void ModelLoading::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
}

void ModelLoading::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}