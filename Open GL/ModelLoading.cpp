#include "ModelLoading.h"
#include "BufferLayout.h"



ModelLoading::ModelLoading() :
	m_vertArr(), m_shader("res/ModelLoading.shader"),
	Backpack("res/backpack/backpack.obj")
{
}

void ModelLoading::Init()
{
	/*float vert[6] = {
		-0.5f,-0.5f,
		0.5f,-0.5f,
		0.0f,0.5f
	};
	m_vertBuff = std::make_unique<VertexBuffer>(vert, 2 * 3 * sizeof(float));

	unsigned int ind[3] = { 0,1,2 };
	m_indBuff = std::make_unique<IndexBuffer>(ind, 3);

	BufferLayout lay;
	lay.Push<float>(2);
	m_vertArr.AddBuffer(*m_vertBuff, lay);*/

	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile("res/backpack/backpack.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
}

void ModelLoading::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	Backpack.Draw(m_shader);
	//gRenderer.Draw(m_vertArr, *m_indBuff, m_shader);
}

void ModelLoading::Close()
{
	m_vertArr.~VertexArray();
	//m_vertBuff->~VertexBuffer();
	m_shader.~Shader();
	//m_indBuff->~IndexBuffer();
}