#pragma once
#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
	public:
		void EnableBlending();
		void Draw(VertexArray& va, IndexBuffer& ib, Shader& sh, GLenum Type=NULL) const;
		void Clear();
		void SetClearColor(float r, float b, float g, float a);
};

