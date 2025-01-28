#include "CameraAssignment.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"
#include <iostream>

CameraAssignment::CameraAssignment() :
	m_vertArr(), m_vertBuff(NULL, 8*6* sizeof(float)),
	m_indBuff(NULL, 36 * sizeof(unsigned int)), m_shader("res/CameraAssignment.shader"),
	m_texture("res/wood.jpg")
{
}

void CameraAssignment::Init()
{
	float vert[8 * 6] = {
		//Cordinates		Colors          front Face
		-0.5f,-0.5f, 0.5f,  0.0f,1.0f,0.0f,  //0 - Green
		 0.5f,-0.5f, 0.5f,  0.0f,1.0f,1.0f,  //1 - Cyn
		 0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,  //2 - White
		-0.5f, 0.5f, 0.5f,  1.0f,1.0f,0.0f,  //3 - Yellow

		//Cordinates		Colors          back Face
		-0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  //4 - Black
		 0.5f,-0.5f,-0.5f,  0.0f,0.0f,1.0f,  //5 - Blue
		 0.5f, 0.5f,-0.5f,  1.0f,0.0f,1.0f,  //6 - Magenta
		-0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,  //7 - Red

	};
	unsigned int ind[36] = {
		0,1,2,//Front Face
		0,2,3,
		4,5,6,//Back Face
		4,6,7,
		0,1,4,//Down Face
		1,5,4,
		3,2,7,//Upper Face
		2,6,7,
		1,5,2,//Left Face
		2,5,6,
		3,0,7,//Right Face
		0,7,4
	};

	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vert, 8 * 6 * sizeof(float));

	BufferLayout lay;
	lay.Push<float>(3);
	lay.Push<float>(3);
	m_vertArr.AddBuffer(m_vertBuff, lay);

	m_indBuff.SetData(ind, 36 * sizeof(unsigned int));
	
	m_texture.Bind();

	
}


void CameraAssignment::UpdateImGui()
{
	ImGui::Text("Camera Assignment");
	static float r = 5.0f, theta=180.0f, alpha=0, speed=0.05f,sens=0.05f;
	static ImVec2 lastMousePos(500,500);
	
	ImGuiIO& io = ImGui::GetIO();
	float deltaX = io.MousePos.x - lastMousePos.x;
	float deltaY = io.MousePos.y-lastMousePos.y;  
	lastMousePos = io.MousePos;

	theta += deltaX * sens;
	alpha += deltaY * sens;
	
	float x = sin(glm::radians(theta)) * cos(glm::radians(alpha)) * r;
	float y = sin(glm::radians(alpha)) * r;
	float z = cos(glm::radians(theta)) * cos(glm::radians(alpha)) * r;
	static glm::vec3 eye(0.0f, 0.0f, 3.0f);

	glm::vec3 cameraFront(x, y, z);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraFront));

	if (ImGui::IsKeyDown(ImGuiKey_UpArrow) || ImGui::IsKeyDown(ImGuiKey_W)) eye += speed * cameraFront;
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow) || ImGui::IsKeyDown(ImGuiKey_S)) eye -= speed * cameraFront;
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)|| ImGui::IsKeyDown(ImGuiKey_A)) eye += right * speed;
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow) || ImGui::IsKeyDown(ImGuiKey_D)) eye -= right * speed;
	eye.y=0.0f;
	glm::mat4 view = glm::lookAt(eye, eye+cameraFront, glm::vec3( 0.0f, 1.0f, 0.0f));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 MVP = proj*view;
	m_shader.SetUniformMat4("uMVP", MVP);
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

	//pitch = aplha ,yaw = theta

}
void CameraAssignment::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.EnableDepthTest();
	gRenderer.ClearDepthBuffer();
	
	//gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
	//gRenderer.DrawArray(m_vertArr,m_shader,GL_TRIANGLES,36);
}



void CameraAssignment::Close()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}