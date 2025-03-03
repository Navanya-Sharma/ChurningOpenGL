#include "ChaosGame.h"
#include "BufferLayout.h"
#include "time.h"

ChaosGame::ChaosGame() :
	m_vertArr(), m_vertBuff(NULL, 2 * 3 * sizeof(float)),
	m_indBuff(NULL, 1 * 3 * sizeof(unsigned int)), m_shader("res/ChaosGame.shader"),
	m_pointsArr(),m_pointsBuff(NULL, 2 * 10 * 25600 * sizeof(float),GL_SHADER_STORAGE_BUFFER),
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
	m_vertBuff.BindBase(1,0,6*sizeof(float));
	BufferLayout layRain;
	layRain.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, layRain);
	m_indBuff.SetData(indData, 3 * sizeof(unsigned int));

	srand(time(0));
	float* x =(float*) malloc(25600 * 2 * sizeof(float));
	for (int i = 0; i < 25600*2; i++)
	{
		float randA = (float)rand() / (float)RAND_MAX;
		float randB = ((float)rand() / (float)RAND_MAX) * (1 - randA);

		x[i++] = randA * vertData[0] + randB * vertData[2] + (1 - randA - randB) * vertData[4];
		x[i] = randA * vertData[1] + randB * vertData[3] + (1 - randA - randB) * vertData[5];
	}
	m_pointsArr.Bind();
	m_pointsBuff.Bind(GL_SHADER_STORAGE_BUFFER);
	m_pointsBuff.SetData(x, 25600*2,0,GL_SHADER_STORAGE_BUFFER);
	m_pointsBuff.Bind();
	m_pointsArr.AddBuffer(m_pointsBuff, layRain);

	m_pointsBuff.Bind(GL_SHADER_STORAGE_BUFFER);
	m_shaderCompute.Bind();
	for(int i=0;i<10;i++){
		m_pointsBuff.BindBase(0,2*25600*i*sizeof(float),2*25600 * sizeof(float));
		GLCall(glDispatchCompute(100, 1, 1));
		GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));
	}

	free(x);
}

void ChaosGame::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	m_shader.SetUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
	m_shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	//
	// gRenderer->Draw(m_pointsArr, m_indBuff, m_shader,GL_POINTS);
	m_pointsBuff.Bind(GL_ARRAY_BUFFER);
	m_pointsArr.Bind();
	gRenderer.EnablePointSize(2);
	GLCall(glDrawArrays(GL_POINTS, 0, 10000 * 10));


}

void ChaosGame::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();


}
