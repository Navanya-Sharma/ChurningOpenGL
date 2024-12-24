#pragma once

#include "VertexBuffer.h"
//#include "BufferLayout.h" This has been excluded and a class declaration has been added to make sure cyclic inclusion of renderer is removed
class BufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout );

	inline unsigned int GetID() { return m_RendererID; }
};

