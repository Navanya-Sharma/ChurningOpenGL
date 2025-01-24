#include "CoordinateAssignment.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"

CoordinateAssignment::CoordinateAssignment() :
	m_vertArr(), m_vertBuff(NULL, 5*36* sizeof(float)),
	m_indBuff(NULL, 36 * sizeof(unsigned int)), m_shader("res/CoordinateAssignment.shader"),
	m_texture("res/wood.jpg")
{
}

void CoordinateAssignment::Init()
{
	/*float vert[8 * 6] = {
		//Cordinates		Colors          front Face
		-0.5f,-0.5f, 0.0f,  0.0f,0.0f,  //0 - Red
		 0.5f,-0.5f, 0.0f,  1.0f,0.0f,  //1 - White
		 0.5f, 0.5f, 0.0f,  1.0f,1.0f,  //2 - Green
		-0.5f, 0.5f, 0.0f,  0.0f,1.0f,  //3 - White

		//Cordinates		Colors          back Face
		-0.5f,-0.5f,-1.0f,  0.0f,0.0f,  //4 - White
		 0.5f,-0.5f,-1.0f,  1.0f,0.0f,  //5 - Green
		 0.5f, 0.5f,-1.0f,  1.0f,1.0f,  //6 - White
		-0.5f, 0.5f,-1.0f,  0.0f,1.0f,  //7 - Blue

	};*/

	float vert[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	m_vertBuff.SetData(vert, 5 * 36 * sizeof(float));

	BufferLayout lay;
	lay.Push<float>(3);
	lay.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, lay);

	m_indBuff.SetData(ind, 36 * sizeof(unsigned int));
	m_shader.SetUniform1i("uText", 0);
	m_texture.Bind();

	
}

void CoordinateAssignment::UpdateImGui()
{
	ImGui::Text("Coordinate Assignment");
	static glm::vec3 rotAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	ImGui::SliderFloat("X - Axis", &rotAxis.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Y - Axis", &rotAxis.y, -1.0f, 1.0f);
	ImGui::SliderFloat("Z - Axis", &rotAxis.z, -1.0f, 1.0f);

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), rotAxis);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 MVP = proj*view*model;
	m_shader.SetUniformMat4("uMVP", MVP);
}
void CoordinateAssignment::Update(Renderer* gRenderer)
{
	
	gRenderer->EnableDepthTest();
	gRenderer->ClearDepthBuffer();
	
	//gRenderer->Draw(m_vertArr, m_indBuff, m_shader);
	gRenderer->DrawArray(m_vertArr,m_shader,GL_TRIANGLES,36);
}



void CoordinateAssignment::Close()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}