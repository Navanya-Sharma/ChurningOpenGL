#include "Base.h"
#include "BufferLayout.h"

Base::Base() :
	m_vertArr(), m_vertBuff(NULL, 0 * sizeof(float)),
	m_indBuff(NULL, 0 * sizeof(unsigned int)), m_shader("")
{
}

void Base::Init()
{
	
}

void Base::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
}

void Base::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}