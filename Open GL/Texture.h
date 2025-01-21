#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path, int b=0);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind();
	void Write(float* values);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	
	inline void SetWidth(int w) { m_Width=w; }
	inline void SetHeight(int h) { m_Height=h; }
};

