#include "ChaosGame.h"
#include "BufferLayout.h"
#include "time.h"

ChaosGame::ChaosGame() :
	m_vertArr(), m_vertBuff(NULL, 2 * 3 * sizeof(float)),
	m_indBuff(NULL, 1 * 3 * sizeof(unsigned int)), m_shader("res/ChaosGame.shader"),
	m_pointsArr(),m_pointsBuff(NULL, 2 * 10 * 10000 * sizeof(float)),
	m_shaderCompute("res/Compute.shader"), m_texture("")
{}


void ChaosGame::Init()
{
	float vertData[6] = {
		-0.5f,-0.5f,
		 0.5f,-0.5f,
		 0.0f, 0.5f,
	};
	unsigned int indData[3] = {
		0,1,2,
	};
	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vertData, 2 * 3 * sizeof(float));
	BufferLayout layRain;
	layRain.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, layRain);
	m_indBuff.SetData(indData, 3 * sizeof(unsigned int));

	//COMPUTING POINTS- VERY UGLY CODE

	srand(time(0));
	float x[20000];
	int i = 0;
	for (int i = 0; i < 20000; i++)
	{
		float randA = (float)rand() / (float)RAND_MAX;
		float randB = ((float)rand() / (float)RAND_MAX) * (1 - randA);

		x[i++] = randA * vertData[0] + randB * vertData[2] + (1 - randA - randB) * vertData[4];
		x[i] = randA * vertData[1] + randB * vertData[3] + (1 - randA - randB) * vertData[5];
		//printf("%f %f\n", x[i-1], x[i]);

	}
	m_texture.Bind();
	m_texture.Write(x);


	float xn[20000 * 10];
	m_shaderCompute.Bind();
	GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, xn));
	GLCall(glDispatchCompute(10000, 1, 1));
	GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));

	for (int i = 0;i < 20000 * 10;i += 20000) {
		GLCall(glDispatchCompute(10000, 1, 1));
		GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));
		GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, xn + i));
		printf("%f %f\n", xn[i], xn[i + 1]);
	}
	m_pointsArr.Bind();
	m_pointsBuff.Bind();
	m_pointsBuff.SetData(xn, 2 * 10000 * 10 * sizeof(float));
	m_pointsArr.AddBuffer(m_pointsBuff, layRain);

}

void ChaosGame::Update(Renderer* gRenderer)
{
	m_shader.SetUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
	gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
	m_shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	gRenderer->Draw(m_pointsArr, m_indBuff, m_shader,GL_POINTS);
	GLCall(glDrawArrays(GL_POINTS, 0, 10000 * 10));

}

void ChaosGame::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();

}
