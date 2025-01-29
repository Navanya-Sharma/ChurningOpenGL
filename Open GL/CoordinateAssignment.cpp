#include "CoordinateAssignment.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"
#include <iostream>

CoordinateAssignment::CoordinateAssignment() :
	m_vertArr(), m_vertBuff(NULL, 8*6* sizeof(float)),
	m_indBuff(NULL, 36 * sizeof(unsigned int)), m_shader("res/CoordinateAssignment.shader"),
	m_texture("res/wood.jpg")
{
}

void CoordinateAssignment::Init()
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
	
	/*float vert[] = {
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
	*/
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


void CoordinateAssignment::UpdateImGui()
{
	ImGui::Text("Coordinate Assignment");
	static float r = 5.0f, theta=0, alpha=0;
	constexpr float halfPI=glm::half_pi<float>();

	ImGui::SliderFloat("Move Side", &theta, 0.0f, 4 * halfPI);
	ImGui::SliderFloat("Move Up", &alpha, 0.0f, 2*halfPI-0.01f);
	ImGui::SliderFloat("Radius", &r, 1.0f, 10.0f);

	float x = sin(theta) * cos(alpha+halfPI) * r;
	float y =			   sin(alpha+halfPI) * r;
	float z = cos(theta) * cos(alpha+halfPI) * r;	

	/*glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, rotAxis);
	glm::mat4 view = glm::lookAt(glm::vec3(x, y, z),
								 glm::vec3( 0.0f, 0.0f, 0.0f),
								 glm::vec3( 0.0f, 1.0f, 0.0f));
	std::cout << "look at" << std::endl;*/
	glm::mat4 proj;
	static bool pres = true;
	if (ImGui::Button("Ortho/Prespective")) {
		pres = !pres;
	}
	
	proj = pres ? glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f): 
				  glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 100.0f);

	glm::vec3 dir = glm::normalize(glm::vec3(x,y,z)-glm::vec3(0,0,0));
	glm::vec3 right= glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), dir));
	glm::vec3 up = glm::normalize(glm::cross(dir, right));

	glm::mat4 view =glm::mat4(glm::vec4(right.x,up.x,dir.x,0),
					glm::vec4(right.y, up.y, dir.y,0),
					glm::vec4(right.z, up.z, dir.z,0),
					glm::vec4(0,0,0,1)) * glm::translate(glm::mat4(1.0f),glm::vec3(-x,-y,-z));
	
	glm::mat4 MVP = proj*view;
	m_shader.SetUniformMat4("uMVP", MVP);
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

	

	//pitch = aplha ,yaw = theta

}
void CoordinateAssignment::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.EnableDepthTest();
	gRenderer.ClearDepthBuffer();
	
	//gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
	//gRenderer.DrawArray(m_vertArr,m_shader,GL_TRIANGLES,36);
}



void CoordinateAssignment::Close()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}