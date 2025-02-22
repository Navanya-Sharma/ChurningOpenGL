#include "LightingMaps.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"
#include <iostream>

LightingMaps::LightingMaps() :
	m_vertArr(), m_vertBuff(NULL, 6*6*8* sizeof(float)),
	m_indBuff(NULL, 36 * sizeof(unsigned int)), 
	m_shader("res/LightingMapsMain.shader"),
	light_vertArr(), light_vertBuff(NULL, 8 * 3 * sizeof(float)),
	light_indBuff(NULL, 36 * sizeof(unsigned int)), 
	light_shader("res/LightingMapsLightSource.shader"),
	diffuseMap("res/container2.png"),
	specularMap("res/container2_specular.png")
{
}

void LightingMaps::Init()
{
	float vert[6*6 * 8]  = {
		// positions          // normals           // texture coords
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	
	};

	unsigned int ind[36] = {
	 0,  1,  2,    // Front face (first triangle)
	 3,  4,  5,    // Front face (second triangle)
	 6,  7,  8,    // Back face (first triangle)
	 9,  10, 11,   // Back face (second triangle)
	 12, 13, 14,   // Left face (first triangle)
	 15, 16, 17,   // Left face (second triangle)
	 18, 19, 20,   // Right face (first triangle)
	 21, 22, 23,   // Right face (second triangle)
	 24, 25, 26,   // Bottom face (first triangle)
	 27, 28, 29,   // Bottom face (second triangle)
	 30, 31, 32,   // Top face (first triangle)
	 33, 34, 35    // Top face (second triangle)
	};

	m_vertArr.Bind();
	m_vertBuff.Bind();
	m_vertBuff.SetData(vert, 6*6 * 8 * sizeof(float));
	BufferLayout lay;
	lay.Push<float>(3);
	lay.Push<float>(3);
	lay.Push<float>(2);
	m_vertArr.AddBuffer(m_vertBuff, lay);
	m_indBuff.SetData(ind, 36 * sizeof(unsigned int));

	float lightvert[8 * 3] = {
		//Cordinates		
		-0.5f,-0.5f, 0.5f,  
		 0.5f,-0.5f, 0.5f,  
		 0.5f, 0.5f, 0.5f,  
		-0.5f, 0.5f, 0.5f,  

		//Cordinates		
		-0.5f,-0.5f,-0.5f,  
		 0.5f,-0.5f,-0.5f,  
		 0.5f, 0.5f,-0.5f,  
		-0.5f, 0.5f,-0.5f,  

	};

	unsigned int lightind[36] = {
		0, 1, 2,    // First triangle
		2, 3, 0,    // Second triangle

		4, 5, 6,
		6, 7, 4,

		0, 3, 7,
		7, 4, 0,

		1, 2, 6,
		6, 5, 1,

		0, 1, 5,
		5, 4, 0,

		2, 3, 7,
		7, 6, 2
	};

	light_vertArr.Bind();
	light_vertBuff.Bind();
	light_vertBuff.SetData(lightvert, 8 * 3 * sizeof(float));
	BufferLayout lay2;
	lay2.Push<float>(3);
	light_vertArr.AddBuffer(light_vertBuff, lay2);
	light_indBuff.SetData(lightind, 36 * sizeof(unsigned int));

}


void LightingMaps::UpdateImGui()
{
	ImGui::Text("Lighting Maps");
	constexpr float halfPI=glm::half_pi<float>();
	static float r = 5.0f, theta=halfPI*2, alpha = halfPI;

	static glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	ImGui::SliderFloat("Move Side", &theta, 0.0f, 4 * halfPI);
	ImGui::SliderFloat("Move Up", &alpha, 0.0f, 2*halfPI-0.01f);
	ImGui::SliderFloat("Radius", &r, 1.0f, 10.0f);

	ImGui::ColorPicker3("Light Color", glm::value_ptr(lightColor), ImGuiColorEditFlags_NoInputs);

	float x = sin(theta) * cos(alpha+halfPI) * r;
	float y =			   sin(alpha+halfPI) * r;
	float z = cos(theta) * cos(alpha+halfPI) * r;	

	glm::vec3 eye(x, y, z);
	glm::mat4 proj;

	glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	
	proj =glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);

	glm::mat4 MVP = proj*view;
	m_shader.SetUniformMat4("uMVP", MVP);
	m_shader.SetUniform3f("uLight.ambient", lightColor.x*0.5f, lightColor.x * 0.5f, lightColor.x * 0.5f);
	m_shader.SetUniform3f("uLight.diffuse", lightColor.x * 0.9f, lightColor.x * 0.9f, lightColor.x * 0.9f);
	m_shader.SetUniform3f("uLight.specular", 1.0f, 1.0f, 1.0f);
	
	diffuseMap.Bind(0);
	specularMap.Bind(1);
	m_shader.SetUniform1i("uMat.diffuse", 0);
	m_shader.SetUniform1i("uMat.specular", 1);
	float shine=32.0f;
	m_shader.SetUniform1fv("uMat.shine", 1, &shine);


	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

	

	static float xl=0.0f, yl=2.0f, zl=0.0f;

	ImGui::SliderFloat("x", &xl, -3.0f, 3.0f);
	ImGui::SliderFloat("y", &yl, -3.0f, 3.0f);
	ImGui::SliderFloat("z", &zl, -3.0f, 3.0f);

	m_shader.SetUniform3f("uLight.position", xl, yl, zl);
	m_shader.SetUniform3f("uVeiw", x, y, z);


	glm::mat4 model=glm::translate(glm::mat4(1.0f), glm::vec3(xl, yl, zl));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.5f));
	MVP= proj * view * model * scale;

	light_shader.SetUniformMat4("uMVP", MVP);
	light_shader.SetUniform4f("uLight", lightColor.x, lightColor.y, lightColor.z, 1.0f);
	gRenderer.Draw(light_vertArr, light_indBuff, light_shader);

}
void LightingMaps::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.EnableDepthTest();
	gRenderer.ClearDepthBuffer();
	
}



void LightingMaps::Close()
{
	GLCall(glDisable(GL_DEPTH_TEST));
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}