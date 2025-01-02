#include "RainbowSquare.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


void RainbowSquare::UpdateColor()
{
	static int colorState = 0;
	static float colorSpeed = 0.025f;
	
	switch (colorState) {
	case 0: // Red to Yellow
		m_color[1] += colorSpeed;
		if (m_color[1] >= 1.0f) colorState = 1;
		break;
	case 1: // Yellow to Green
		m_color[0] -= colorSpeed;
		if (m_color[0] <= 0.0f) colorState = 2;
		break;
	case 2: // Green to Cyan
		m_color[2] += colorSpeed;
		if (m_color[2] >= 1.0f) colorState = 3;
		break;
	case 3: // Cyan to Blue
		m_color[1] -= colorSpeed;
		if (m_color[1] <= 0.0f) colorState = 4;
		break;
	case 4: // Blue to Magenta
		m_color[0] += colorSpeed;
		if (m_color[0] >= 1.0f) colorState = 5;
		break;
	case 5: // Magenta to Red
		m_color[2] -= colorSpeed;
		if (m_color[2] <= 0.0f) colorState = 0;
		break;
	
	}
}

RainbowSquare::RainbowSquare():
	m_vertArr(), m_vertBuff(NULL,2*4*sizeof(float)), m_indBuff(NULL,2*3*sizeof(unsigned int)), m_shader("res/Rainbow.Shader"),m_color{1.0f,0.0f,0.0f}
{
}

void RainbowSquare::Init()
{
	float vertData[8] = {
		-0.5f,-0.5f,
		 0.5f,-0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f
	};
	unsigned int indData[6] = {
		0,1,2,
		0,2,3
	};
	
	m_vertBuff.SetData(vertData,2*4*sizeof(float));
	BufferLayout layRain;
	layRain.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, layRain);
	m_indBuff.SetData(indData,6*sizeof(float));

}

void RainbowSquare::Update(Renderer* gRenderer)
{
	UpdateColor();
	m_shader.SetUniform4f("u_color", m_color[0], m_color[1], m_color[2], 1.0f);
	gRenderer->Draw(m_vertArr,m_indBuff,m_shader);
}

void RainbowSquare::Close()
{
	delete(m_color);
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}
