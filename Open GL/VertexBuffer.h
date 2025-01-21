#pragma once
#include <glad/glad.h>
class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, GLenum type=GL_ARRAY_BUFFER);
	~VertexBuffer();

	void SetData(float * data, unsigned int size, unsigned int offset = 0, GLenum type=GL_ARRAY_BUFFER) const;
	void Bind(GLenum type = GL_ARRAY_BUFFER) const;
	void Unbind(GLenum type = GL_ARRAY_BUFFER) const;
	void BindBase(unsigned int bind,unsigned int offset, unsigned int size) const;

	inline unsigned int GetID() { return m_RendererID; }

};

