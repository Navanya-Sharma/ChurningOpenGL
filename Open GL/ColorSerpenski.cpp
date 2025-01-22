#include "ColorSerpenski.h"
#include "BufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

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
	glm::mat4 mvp = glm::mat4(1.0f);
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 1, 0));
	mvp = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0))*rot;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	DrawSerpent(glm::vec2(0, 0), 0.25, 1, gRenderer);
}

void ColorSerpenski::DrawSerpent(glm::vec2 center, float scale, int depth, Renderer* gRenderer)
{
	if (depth == 7) 
	{
		return;
	}

	glm::vec2 c1(center.x , center.y+(2*std::pow(3,0.5)*scale)/3);
	glm::vec2 c2(center.x - scale, center.y - scale / std::pow(3, 0.5));
	glm::vec2 c3(center.x + scale, center.y - scale / std::pow(3, 0.5));

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale,- scale, 0));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(c1, 0));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(), glm::vec3(0,1,0));
	glm::mat4 mvp = trans * scaleMat*rot;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(c2, 0));
	mvp = trans * scaleMat * rot;;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(c3, 0));
	mvp = trans * scaleMat * rot;
	m_shader.SetUniformMat4("uMVP", mvp);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);

	DrawSerpent(c1, scale * 0.5f, depth + 1, gRenderer);
	DrawSerpent(c2, scale * 0.5f, depth + 1, gRenderer);
	DrawSerpent(c3, scale * 0.5f, depth + 1, gRenderer);
}

void ColorSerpenski::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}