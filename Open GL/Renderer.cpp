#include "Renderer.h"

#include <iostream>


void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum err = glGetError()) {
		std::string error = "";

		switch (err)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		printf("[Open GL Error] %d %s at Line No %d\n In Function: %s\nUsed in File: %s\n", err, error.c_str(), line, function, file);
		return false;
	}
	return true;
}

void Renderer::EnableBlending()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::EnablePointSize(int size)
{
	GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
	GLCall(glPointSize(size));
}

void Renderer::EnableDepthTest()
{
	GLCall(glEnable(GL_DEPTH_TEST));

}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& sh) const
{
	va.Bind();
	ib.Bind();
	sh.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstance(VertexArray& va, IndexBuffer& ib, Shader& sh, int count)
{
	va.Bind();
	sh.Bind();
	ib.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr, count));
}

void Renderer::DrawArray(VertexArray& va, Shader& sh, GLenum type, int count)
{
	va.Bind();
	sh.Bind();
	glDrawArrays(type, 0, count);
}

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::ClearDepthBuffer()
{
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetClearColor(float* color)
{
	GLCall(glClearColor(color[0], color[1], color[2], color[3]));
}

float Renderer::GetDeltaTime()
{
	return DeltaTime;
}

void Renderer::SetDeltaTime(float time)
{
	DeltaTime = time;
}
