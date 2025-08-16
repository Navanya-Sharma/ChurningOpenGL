#include "ModelLoading.h"
#include "BufferLayout.h"



ModelLoading::ModelLoading() :
	m_vertArr(), m_shader("res/ModelLoading.shader")
	//Backpack("res/backpack/backpack.obj")
{
}

void ModelLoading::Init()
{
	vector<vertex> vert;
	vertex a;

	a.Position=glm::vec3(-0.5f, -0.5f, 0.0f);
	a.Normal=glm::vec3(0.0f, 0.0f, 1.0f);
	a.TexCords = glm::vec2(0.0f, 0.0f);
	vert.push_back(a);

	a.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	a.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	a.TexCords = glm::vec2(1.0f, 0.0f);
	vert.push_back(a);

	a.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	a.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	a.TexCords = glm::vec2(1.0f, 1.0f);
	vert.push_back(a);

	a.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	a.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	a.TexCords = glm::vec2(0.0f, 1.0f);
	vert.push_back(a);


	//m_vertBuff = std::make_unique<VertexBuffer>(vert, 2 * 3 * sizeof(float));

	unsigned int ind[6] = { 0,1,2, 0,2,3 };

	vector<unsigned int> indv(ind, ind + 6);

	//Texture p("res/emoji.png");
	//p.type = "texture_diffuse";
	//p.Bind();
	vector<Texture> pic;
	//pic.push_back(p);

	//MyMesh b(vert, indv);

	krish = std::make_unique<MyMesh>(vert,indv,pic);
	//m_indBuff = std::make_unique<IndexBuffer>(ind, 3);

	/*BufferLayout lay;
	lay.Push<float>(2);
	m_vertArr.AddBuffer(*m_vertBuff, lay);*/

	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile("res/backpack/backpack.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
}

void ModelLoading::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	krish->Draw(m_shader);
	//Backpack.Draw(m_shader);
	//gRenderer.Draw(m_vertArr, *m_indBuff, m_shader);
}

void ModelLoading::Close()
{
	m_vertArr.~VertexArray();
	//m_vertBuff->~VertexBuffer();
	m_shader.~Shader();
	//m_indBuff->~IndexBuffer();
}