
#include "VertexArray.h"

#include "BufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size();i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normal,
			layout.GetStride(), (const void*)offset));
		offset += element.count * BufferElement::GetSizeOfType(element.type);
	}
}
void VertexArray::AddInstanced(const VertexBuffer& vb, const BufferLayout& layout, int offAttri)
{
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size();i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i + offAttri));
		GLCall(glVertexAttribPointer(i + offAttri, element.count, element.type, element.normal,
			layout.GetStride(), (const void*)offset));
		GLCall(glVertexAttribDivisor(i + offAttri, 1));
		offset += element.count * BufferElement::GetSizeOfType(element.type);
	}
}
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
