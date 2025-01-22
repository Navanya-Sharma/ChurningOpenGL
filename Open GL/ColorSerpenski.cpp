#include "ColorSerpenski.h"
#include "BufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

ColorSerpenski::ColorSerpenski() :
	m_vertArr(), m_vertBuff(NULL, 15 * sizeof(float)),
	m_indBuff(NULL, 3 * sizeof(unsigned int)), m_shader("res/ColorSerpenski.shader")
{
}

void ColorSerpenski::Init()
{
	float vert[5 * 3] = {
		-0.5f,-0.289f,1.0f,0.5f,0.5f,
		 0.5f,-0.289f,0.5f,1.0f,0.5f,
		 0.0f, 0.577f,0.5f,0.5f,1.0f
	};
	unsigned int ind[3] = { 0,1,2 };

	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vert, 15 * sizeof(float));

	BufferLayout lay;
	lay.Push<float>(2);
	lay.Push<float>(3);

	m_vertArr.AddBuffer(m_vertBuff, lay);

	m_indBuff.SetData(ind, 3 * sizeof(unsigned int));
}

void ColorSerpenski::Update(Renderer* gRenderer)
{
	float scaleFactor = 1.0f;
	for(int i=0;i<10;i++)
	{
		glm::mat4 mvp = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, std::pow(-1,i)*scaleFactor, 0));
		m_shader.SetUniformMat4("uMVP", mvp);
		gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
		scaleFactor *= 0.5f;
	}
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, -0.25, 0));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0,std::pow(3,0.5)/6,0));
	glm::mat4 mvp = trans * scale;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -std::pow(3, 0.5) / 12, 0));
	mvp = trans * scale;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25, -std::pow(3, 0.5) / 12, 0));
	mvp = trans * scale;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
}

void ColorSerpenski::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}