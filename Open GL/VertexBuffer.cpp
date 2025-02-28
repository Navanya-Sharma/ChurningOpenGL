#include "VertexBuffer.h"
#include "Renderer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum type )
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(type, m_RendererID));
	GLCall(glBufferData(type, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::SetData(float* data, unsigned int size, unsigned int offset, GLenum type) const
{
	Bind();
	GLCall(glBufferSubData(type,offset,size,data));
}

void VertexBuffer::Bind(GLenum type ) const
{
	GLCall(glBindBuffer(type, m_RendererID));

}

void VertexBuffer::Unbind(GLenum type ) const
{
	GLCall(glBindBuffer(type, 0));
}

void VertexBuffer::BindBase(unsigned int bind, unsigned int offset,unsigned int size) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, m_RendererID);
	//glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bind, m_RendererID, offset, size);
}
