#include "Texture.h"
#include "res/stb_image.h"


Texture::Texture(const std::string& path, int b)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
	m_Height(0),m_BPP(0)
{

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE));

	if (path == "") {
		m_Width = 10000;
		m_Height = 1;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_Width, m_Height, 0, GL_RED, GL_FLOAT, NULL);
		glBindImageTexture(0, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RG32F);
		
	}
	else {
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		if (m_LocalBuffer == nullptr) { ASSERT(0); }
		else { GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));}
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, b));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1,&m_RendererID));
}

void Texture::Bind(unsigned int slot/*=0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0+slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
void Texture::Write(unsigned int* values)
{
	Bind();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_Width, m_Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, values));
	GLCall(glBindImageTexture(0, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI));
}