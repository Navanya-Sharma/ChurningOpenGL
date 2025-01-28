#include "ShaderAssignment.h"
#include "BufferLayout.h"

ShaderAssignment::ShaderAssignment() :
	m_vertArr(), m_vertBuff(NULL, 15 * sizeof(float)),
	m_indBuff(NULL, 3 * sizeof(unsigned int)), m_shader("res/ShaderAssignment.shader")
{
}

void ShaderAssignment::Init()
{
	float vert[5 * 3] = {
		-0.5f,-0.5f,1.0f,0.0f,0.0f,
		 0.5f,-0.5f,0.0f,1.0f,0.0f,
		 0.0f, 0.5f,0.0f,0.0f,1.0f
	};
	unsigned int ind[3] = { 0,1,2 };

	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vert, 15 * sizeof(float));

	BufferLayout lay;
	lay.Push<float>(2);
	lay.Push<float>(3);

	m_vertArr.AddBuffer(m_vertBuff, lay);

	m_indBuff.SetData(ind, 3 * sizeof(unsigned int));

}

void ShaderAssignment::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	static float d = 0.0f;
	d += 0.005f;
	m_shader.SetUniform1fv("move", 1, &d);
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
}

void ShaderAssignment::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}