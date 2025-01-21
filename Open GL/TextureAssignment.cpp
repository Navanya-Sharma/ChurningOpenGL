#include "TextureAssignment.h"
#include "BufferLayout.h"

TextureAssignment::TextureAssignment() :
	m_vertArr(), m_vertBuff(NULL, 4*7 * sizeof(float)),
	m_indBuff(NULL, 6 * sizeof(unsigned int)), m_shader("res/TextureAssignment.shader"),
	face("res/emoji.png"),wood("res/wood.jpg",1)
{
}

void TextureAssignment::Init()
{
	float vert[4*7] = {
		//Cordinates   Colors          Texture Cordiated
		-0.8f,-0.8f,  1.0f,0.0f,0.0f,  0.0f,0.0f, //0.6f ,0.5f,
		 0.8f,-0.8f,  0.0f,1.0f,0.0f,  1.0f,0.0f, //0.65f,0.5f,
		 0.8f, 0.8f,  0.0f,0.0f,1.0f,  1.0f,1.0f, //0.65f,0.55f,
		-0.8f, 0.8f,  1.0f,0.0f,1.0f,  0.0f,1.0f  //0.6f ,0.55f
	};
	unsigned int ind[6] = {
		0,1,2,
		0,2,3
	};

	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vert, 4 * 7 * sizeof(float));

	BufferLayout lay;
	lay.Push<float>(2);
	lay.Push<float>(3);
	lay.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, lay);

	m_indBuff.SetData(ind, 6 * sizeof(unsigned int));

	m_shader.SetUniform1i("face", 0);
	m_shader.SetUniform1i("wood", 1);
	
	face.Bind();
	wood.Bind(1);

}

void TextureAssignment::Update(Renderer* gRenderer)
{
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

}

void TextureAssignment::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}