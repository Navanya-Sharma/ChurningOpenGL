#include "ModelViewProjection.h"
#include "BufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

ModelViewProjection::ModelViewProjection():
	m_vertArr(), m_vertBuff(NULL, 4 * 4 * sizeof(float)), 
	m_indBuff(NULL, 2 * 3 * sizeof(unsigned int)), m_shader("res/MVP.Shader"), 
	m_texture("res/nav.png")
{
}

void ModelViewProjection::Init()
{
	float vertData[16] = {
		 300.f,200.f, 0.0f, 0.0f,
		 900.f,200.f, 1.0f, 0.0f,
		 900.f,600.f, 1.0f, 1.0f,
		 300.f,600.f, 0.0f, 1.0f

	};
	unsigned int indData[6] = {
		0,1,2,
		0,2,3
	};

	m_vertBuff.SetData(vertData, 4 * 4 * sizeof(float));
	BufferLayout layRain;
	layRain.Push<float>(2);
	layRain.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, layRain);
	m_indBuff.SetData(indData, 6 * sizeof(unsigned int));

	
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
	//glm::mat4 mvp = proj * veiw * model;

	glm::vec3 transA(0, 0, 0);
	glm::vec3 transB(400, 200, 0);

	//m_shader.SetUniformMat4("u_MVP", mvp);
	m_shader.SetUniform1i("u_Texture", 0);

}

void ModelViewProjection::Update(Renderer* gRenderer)
{
	glm::mat4 proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	/*0.0025	0	 0	 -1
		0	0.0025	 0	 -1
		0	   0	-1	  0
		0	   0	 0    1*/
	glm::mat4 veiw = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	/*	1   0  0  -100
		0   1  0    0
		0   0  1    0
		0   0  0    1 */
	m_texture.Bind();
	gRenderer->EnableBlending();
	static int i = 0;
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-250,i++, 0));
		glm::mat4 mvp = proj * veiw * model;
		m_shader.SetUniformMat4("u_MVP", mvp);
		gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
	}
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(250, -i, 0));
		glm::mat4 mvp = proj * veiw * model;
		m_shader.SetUniformMat4("u_MVP", mvp);
		gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
	}

	/*ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Your Options");

	//ImGui::SliderFloat3("float A", &transA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::SliderFloat3("float B", &transB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", color, ImGuiColorEditFlags_NoInputs); // Edit 3 floats representing a color

	static int selected = -1;
	int changed = 0;
	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();

	//ImGui::ShowDemoWindow();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}*/

}

void ModelViewProjection::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
	m_texture.~Texture();
}
