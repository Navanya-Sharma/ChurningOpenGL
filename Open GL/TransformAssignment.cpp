#include "TransformAssignment.h"
#include "BufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

TransformAssignment::TransformAssignment() :
	m_vertArr(), m_vertBuff(NULL, 16 * sizeof(float)),
	m_indBuff(NULL, 6 * sizeof(unsigned int)), 
	m_shader("res/TransformAssignment.shader"),
	m_texture("res/emoji.png")
{
}

void TransformAssignment::Init()
{
	float vertData[16] = {
			-0.8f,-0.8f, 0.0f, 0.0f,
			 0.8f,-0.8f, 1.0f, 0.0f,
			 0.8f, 0.8f, 1.0f, 1.0f,
			-0.8f, 0.8f, 0.0f, 1.0f

	};
	unsigned int indData[6] = {
		0,1,2,
		0,2,3
	};

	m_vertArr.Bind();
	m_vertBuff.SetData(vertData, 4 * 4 * sizeof(float));
	BufferLayout layRain;
	layRain.Push<float>(2);
	layRain.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, layRain);
	m_indBuff.SetData(indData, 6 * sizeof(unsigned int));

	
	m_shader.SetUniform1i("u_Texture", 0);

	m_texture.Bind();
}

void TransformAssignment::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 1.0f));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 1, 0));
	glm::mat4 mvp = trans*scale* rot ;
	m_shader.SetUniformMat4("u_MVP", mvp);

	gRenderer.EnableBlending();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.3f, 0));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(cos(glfwGetTime()), cos(glfwGetTime()), 1.0f));
	mvp = trans * scale;

	m_shader.SetUniformMat4("u_MVP", mvp);

	gRenderer.EnableBlending();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
}

void TransformAssignment::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}