#pragma once
class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void SetData(unsigned int* data, unsigned int size, unsigned int offset = 0) const;
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() { return m_RendererID; }
	inline unsigned int GetCount() { return m_Count; }

};

