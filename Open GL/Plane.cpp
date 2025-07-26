#include "Plane.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"

Plane::Plane() :
	m_vertArr(), m_vertBuff(NULL, 4*2 * sizeof(float)),
	m_indBuff(NULL, 3*2 * sizeof(unsigned int)), m_shader("res/Plane.shader")
{
}

void Plane::Init()
{
	float vert[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	unsigned int ind[] = {
		0,1,2,
		0,2,3
	};

	m_vertBuff.SetData(vert, 4*2 * sizeof(float));
	BufferLayout lay;
	lay.Push<float>(2);
	m_indBuff.SetData(ind, 3*2 * sizeof(unsigned int), 0);
	m_vertArr.AddBuffer(m_vertBuff, lay);


}

void Plane::Update()
{
	

	Renderer& gRenderer = Renderer::GetRenderer();
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
}


void Plane::UpdateImGui() {
	static glm::mat4x4 trans(1.0f);
	static glm::mat4x4 scale(1.0f);
	static glm::mat4x4 rotate(1.0f);
	static glm::mat4x4 pivotN(1.0f);
	static glm::mat4x4 pivotP(1.0f);
	static bool f = true;

	static float px = 0.2f, py = 0.2f;
	ImGui::DragFloat("Xp", &px, 0.005f);
	ImGui::DragFloat("Yp", &py, 0.005f);
	if (ImGui::Button("Set Pivot")) {
		pivotN = glm::translate(glm::mat4x4(1.0f), glm::vec3(-px, -py, 0));
		pivotP = glm::translate(glm::mat4x4(1.0f), glm::vec3(px, py, 0));

	}
	static float x = 0.2f, y = 0.2f;
	ImGui::DragFloat("X", &x, 0.005f);
	ImGui::DragFloat("Y", &y, 0.005f);
	if (ImGui::Button("Translaste")) {
		trans = glm::translate(glm::mat4x4(1.0f), glm::vec3(x, y, 0));
		f = true;
	}
	
	static float rx = 0.2f;
	ImGui::DragFloat("Xr", &rx, 0.005f);
	if (ImGui::Button("Rotate")) {
		rotate = glm::rotate(glm::mat4x4(1.0f),rx,glm::vec3(0,0,1));
		f = true;

	}
	static float sx = 0.2f, sy = 0.2f;
	ImGui::DragFloat("Xs", &sx, 0.005f);
	ImGui::DragFloat("Ys", &sy, 0.005f);
	if (ImGui::Button("Scale")) {
		scale = glm::scale(glm::mat4x4(1.0f), glm::vec3(sx, sy, 0));
		f = true;

	}
	if (f) {
		m_shader.SetUniformMat4("uMVP", trans * pivotP * rotate * scale * pivotN);
		f = false;
	}
}

void Plane::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}