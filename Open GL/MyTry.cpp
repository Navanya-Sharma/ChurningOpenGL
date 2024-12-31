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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"

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

void printMatrix(const glm::mat4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			std::cout << matrix[col][row] << " ";
		}
		std::cout << std::endl;
	}
}
/*
static float* ortho(float x1, float x2, float y1, float y2, float z1, float z2) {
	float mat[16] = {
		2/(x2-x1),		   0,			   0,		0,
			0,		    2/(y2-y1),         0,		0,
			0,			   0,          2/(z1-z2),   0,
	(x1+x2)/(x1-x2),(y1+y2)/(y1-y2),(z1+z2)/(z1-z2),1

	};
	return mat;
}
*/
int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = NULL;
	win = glfwCreateWindow(800, 800, "Cute Navanya", NULL, NULL);

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
			-0.8f,-0.8f, 0.0f, 0.0f,
			0.8f,-0.8f, 1.0f, 0.0f,
			//0.8f,0.8f, 1.0f, 1.0f,

			0.0f,0.8f, 1.0f, 1.0f,
			//-0.8f, 0.8f,0.0f,1.0
	};
	unsigned int ind[6] = {
		0,1,2,
		//0,2,3
	};


	VertexArray va;
	VertexBuffer veboj(vert, 4*4 * sizeof(float));

	BufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(veboj, layout);
	
	IndexBuffer inbu(ind, 6);

	//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	//float* proj2 = ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	/*0.0025	0	 0	 -1
		0	0.0025	 0	 -1
		0	   0	-1	  0
	    0	   0	 0    1*/
	//printMatrix(proj);
	/*printf("My mat\n");
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			std::cout << proj[col][row] << " ";
		}
		std::cout << std::endl;
	}*/
	glm::mat4 veiw = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	/*	1   0  0  -100
		0   1  0    0
		0   0  1    0
	    0   0  0    1 */
	
	//std::cout << mvp << std::endl;

	Shader shh("res/Basic.shader");
	
	Shader CompShader("res/Compute.shader");

	Texture TextComp("");
	TextComp.Bind();
	CompShader.Bind();
	
	srand(time(0));
	float x[20000];
	int i = 0;
	for (int i = 0; i < 20000; i++)
	{
		float randA = (float)rand() / (float)RAND_MAX;
		float randB = ((float)rand() / (float)RAND_MAX) * (1 - randA);

		x[i++] = randA * vert[0] + randB * vert[4] + (1 - randA - randB) * vert[8];
		x[i] = randA * vert[1] + randB * vert[5] + (1 - randA - randB) * vert[9];
	}

	TextComp.Write(x);

	float xn[20000*10];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, xn);
	/*for (auto d : xn) {
		std::cout << d << "x ";
	}*/
	std::cout << std::endl;
	glDispatchCompute(10000, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	for (int i = 0;i < 20000 * 10;i += 20000) {
		glDispatchCompute(10000, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, xn+i);
		
	}
	/*for (auto d : xn) {
			std::cout << d << "New x ";
		}*/
	
	/*VertexArray ArrPoints;
	VertexBuffer BuffPoints(xn, 2 * 100 * sizeof(float));

	BufferLayout layPoints;
	layPoints.Push<float>(2);
	ArrPoints.AddBuffer(BuffPoints, layPoints);*/
	//VertexArray ArrPoints;ArrPoints.Bind();
	unsigned int vaob;
	GLCall(glGenVertexArrays(1, &vaob));
	GLCall(glBindVertexArray(vaob));

	unsigned int b;
	GLCall(glGenBuffers(1, &b));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, b));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 2 * 10000*10 * sizeof(float), xn, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));

	Shader shPoints("res/Basic2.shader");
	unsigned int shaderb = shPoints.get();

	shh.Bind();

	Texture image("res/nav.png");
	image.Bind();
	shh.SetUniform1i("u_Texture", 0);

	va.Unbind();
	veboj.Unbind();
	inbu.Unbind();
	shh.Unbind();	
	
	Renderer gRenderer;
	gRenderer.EnableBlending();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glm::vec3 transA(0, 0, 0);
	glm::vec3 transB(400, 200, 0);


	while (!glfwWindowShouldClose(win))
	{
		

		//CompShader.Bind();
		TextComp.Bind();
		

		//image.Bind();
		gRenderer.SetClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		gRenderer.Clear();
		//gRenderer.Draw(va, inbu, shh);
		gRenderer.Draw(va, inbu, shh);

		GLCall(glUseProgram(shaderb));
		GLCall(glBindVertexArray(vaob));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, b));
		GLCall(glDrawArrays(GL_POINTS, 0, 10000*10));
		
		//gRenderer.Draw(ArrPoints, inbu, shPoints,GL_POINTS);
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{



			ImGui::Begin("wind");
			ImGui::SliderFloat3("float A", &transA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat3("float B", &transB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}


		// Rendering
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		shh.Bind();
		updateColors();
		{
			
		glm::mat4 model = glm::translate(glm::mat4(1.0f), transA);
		glm::mat4 mvp = proj * veiw * model;
		shh.SetUniformMat4("u_MVP", mvp);
		}
		/* {
			gRenderer.Draw(va, inbu, shh);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), transB);
			glm::mat4 mvp = proj * veiw * model;
			shh.SetUniformMat4("u_MVP", mvp);
		}*/

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
 
