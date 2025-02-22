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
private:
	static Renderer inst;
	Renderer() {};

public:
	inline static Renderer& GetRenderer() { return inst; };
	void EnableBlending();
	void EnablePointSize(int size);
	void EnableDepthTest();
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& sh) const;
	void DrawArray(VertexArray& va, Shader& sh, GLenum type, int count);
	void Clear();
	void ClearDepthBuffer();
	void SetClearColor(float* color);
};

