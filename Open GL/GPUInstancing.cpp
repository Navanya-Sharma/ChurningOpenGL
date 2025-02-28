#include "GPUInstancing.h"
#include "BufferLayout.h"

GPUInstancing::GPUInstancing() :
	m_vertArr(), m_vertBuff(NULL, 4*3* sizeof(float)), m_InstanceBuff(NULL, 200 * sizeof(float)),
	m_indBuff(NULL, 6 * sizeof(unsigned int)), m_shader("res/GPUInstancing.shader")
{
}

void GPUInstancing::Init()
{
	float vert[] = {
		-0.05f, -0.05f, 0.0f, 
		 0.05f, -0.05f, 0.0f, 
		 0.05f,  0.05f, 0.0f, 
		-0.05f,  0.05f, 0.0f };
	unsigned int ind[] = {
		0, 1, 2,
		2, 3, 0
	};
	m_vertBuff.SetData(vert, 4 * 3 * sizeof(float));
	m_indBuff.SetData(ind, 6 * sizeof(unsigned int));
	BufferLayout lay;
	lay.Push<float>(3);
	m_vertArr.AddBuffer(m_vertBuff, lay);

	float translations[200];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			translations[index++] = (float)x / 10.0f + offset;
			translations[index++] = (float)y / 10.0f + offset;
		}
	}

	m_InstanceBuff.Bind();
	m_InstanceBuff.SetData(translations, 200 * sizeof(float));
	
	BufferLayout lay2;
	lay2.Push<float>(2);
	m_vertArr.AddInstanced(m_InstanceBuff, lay2, 1);
}

void GPUInstancing::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.DrawInstance(m_vertArr, m_indBuff, m_shader, 100);
}

void GPUInstancing::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}