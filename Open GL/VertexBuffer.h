#pragma once
class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void SetData(float * data, unsigned int size, unsigned int offset = 0) const;
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() { return m_RendererID; }

};

