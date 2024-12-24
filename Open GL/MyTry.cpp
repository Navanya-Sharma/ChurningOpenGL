#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int colorState = 0;
float red=1.0f, green=0.0f, blue=0.0f;
void updateColors() {
	// State machine to cycle through rainbow colors
	float colorSpeed = 0.05f;
	switch (colorState) {
	case 0: // Red to Yellow
		green += colorSpeed;
		if (green >= 1.0f) colorState = 1;
		break;
	case 1: // Yellow to Green
		red -= colorSpeed;
		if (red <= 0.0f) colorState = 2;
		break;
	case 2: // Green to Cyan
		blue += colorSpeed;
		if (blue >= 1.0f) colorState = 3;
		break;
	case 3: // Cyan to Blue
		green -= colorSpeed;
		if (green <= 0.0f) colorState = 4;
		break;
	case 4: // Blue to Magenta
		red += colorSpeed;
		if (red >= 1.0f) colorState = 5;
		break;
	case 5: // Magenta to Red
		blue -= colorSpeed;
		if (blue <= 0.0f) colorState = 0;
		break;
	}
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = NULL;
	win = glfwCreateWindow(500, 500, "Cute Navanya", NULL, NULL);

	if (win == NULL) {
		printf("Null");
		return -1;
	}

	glfwMakeContextCurrent(win);
	glfwSwapInterval(2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD not initialized");
		return -1;
	}
{
	float vert[] = {
			-0.5f,-0.5f, 0.0f, 0.0f,
			0.5f,-0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f,0.0f,1.0
	};
	unsigned int ind[6] = {
		0,1,2,
		0,2,3
	};
	Renderer gRenderer;

	VertexArray va;
	VertexBuffer veboj(vert, 4*4 * sizeof(float));

	BufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(veboj, layout);
	
	IndexBuffer inbu(ind, 6);

	Shader shh("res/Basic.shader");
	shh.Bind();
	shh.SetUniform4f("n_color", red, green, blue, 1.0f);

	Texture image("res/nav.jpg");
	image.Bind();
	shh.SetUniform1i("u_Texture", 0);

	va.Unbind();
	veboj.Unbind();
	inbu.Unbind();
	shh.Unbind();

	while (!glfwWindowShouldClose(win))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		updateColors();	

		gRenderer.Draw(va, inbu, shh);
        shh.SetUniform4f("n_color", red, green, blue, 1.0f);
		
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}
	glfwTerminate();
	return 0;
}
 
