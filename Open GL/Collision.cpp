#include "Collision.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "IMGui/imgui.h"
#include "IMGui/imgui_impl_glfw.h"
#include "IMGui/imgui_impl_opengl3.h"

Collision::Collision() :
	m_vertArr(), m_vertBuff(NULL, 4 * 2 * sizeof(float)), m_vertArr2(), m_vertBuff2(NULL, 4 * 2 * sizeof(float)),
	m_indBuff(NULL, 3 * 2 * sizeof(unsigned int)), m_shader("res/Collision.shader"), biki(), e(1.0f)
{	
	biki.Position.x = 3.0f;
	biki.Color = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	guria.Color = Vector4(0.2f, 0.7f, 0.2f, 1.0f);
	guria.Size = Vector2(2.0f, 2.0f);
}

void Collision::Init()
{
	float vert[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	unsigned int ind[] = {
		0,1,2,
		0,2,3
	};

	m_vertArr.Bind();
	m_vertBuff.SetData(vert, 4 * 2 * sizeof(float));
	BufferLayout lay;
	lay.Push<float>(2);
	m_indBuff.SetData(ind, 3 * 2 * sizeof(unsigned int), 0);
	m_vertArr.AddBuffer(m_vertBuff, lay);


	vert[2] = guria.Size.x;
	vert[4] = guria.Size.x;
	vert[5] = guria.Size.y;
	vert[7] = guria.Size.y;
	m_vertArr2.Bind();
	m_vertBuff2.SetData(vert, 4 * 2 * sizeof(float));
	m_indBuff.Bind();
	m_vertArr2.AddBuffer(m_vertBuff2, lay);
	
	//printf("%f %f %f %f ", biki.Color.x, biki.Color.y, biki.Color.z, biki.Color.w);
}

void Collision::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();

	if (CheckCollision()) {
			//printf("Collision gurira %f biki %f\n", guria.Position.x,biki.Position.x);
			biki.Acceleration = Vector2(0.0f, 0.0f);
			guria.Acceleration = Vector2(0.0f, 0.0f);

			guria.Velocity.x = (guria.Mass - e * biki.Mass) * guria.Velocity.x + (1 + e) * biki.Mass * biki.Velocity.x;
			guria.Velocity.x /= (guria.Mass + biki.Mass);
			biki.Velocity.x = (biki.Mass - e * guria.Mass) * biki.Velocity.x + (1 + e) * guria.Mass * guria.Velocity.x;
			biki.Velocity.x /= (guria.Mass + biki.Mass);
		}
	//else { printf("Not \n"); }

	glm::mat4x4 Model = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f);
	glm::mat4x4 MVP, trans;

	biki.Velocity += biki.Acceleration * gRenderer.GetDeltaTime();
	biki.Position += biki.Velocity *gRenderer.GetDeltaTime();
	trans = glm::translate(glm::mat4x4(1.0f),
		glm::vec3(biki.Position.x, biki.Position.y, 0.0f));

	MVP = Model * trans;
	m_shader.SetUniformMat4("uMVP", MVP);
	m_shader.SetUniform4f("uColor", biki.Color.x,biki.Color.y,biki.Color.z,biki.Color.w);
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

	guria.Velocity += guria.Acceleration * gRenderer.GetDeltaTime();
	guria.Position += guria.Velocity*gRenderer.GetDeltaTime();

	trans = glm::translate( glm::mat4x4(1.0f),
		glm::vec3(guria.Position.x, guria.Position.y, 0.0f));
	MVP = Model * trans;
	m_shader.SetUniformMat4("uMVP", MVP);
	m_shader.SetUniform4f("uColor", guria.Color.x, guria.Color.y, guria.Color.z, guria.Color.w);


	gRenderer.Draw(m_vertArr2, m_indBuff, m_shader);

	
}

bool Collision::CheckCollision()
{
	float GuriaLeft, GuriaRight, GuriaTop, GuriaBottom;
	float BikiLeft, BikiRight, BikiTop, BikiBottom;
	GuriaLeft = guria.Position.x;
	GuriaRight = guria.Position.x + guria.Size.x;
	GuriaBottom = guria.Position.y;
	GuriaTop = guria.Position.y + guria.Size.y;
	BikiLeft = biki.Position.x;
	BikiRight = biki.Position.x + biki.Size.x;
	BikiBottom = biki.Position.y;
	BikiTop = biki.Position.y + biki.Size.y;
	if (BikiRight < GuriaLeft) {
		//printf("Br < Gl b %f g %f\n", BikiRight, GuriaLeft);
		return false;
	}
	if (BikiLeft > GuriaRight){
		//printf("Bl > Gr b %f g %f\n", BikiLeft, GuriaRight);
		return false;}
	if ( BikiBottom > GuriaTop){
		//printf("Bb > Gt b %f g %f\n", BikiBottom, GuriaTop);
		return false;}
	if ( BikiTop < GuriaBottom){
		//printf("Bt < Gb b %f g %f\n", BikiTop, GuriaBottom);
		return false;}
	return true;
}

void Collision::UpdateImGui() {

	//float pos = biki.Position.x;

	ImGui::DragFloat2("Velocity", &biki.Velocity.x, 0.05f);
	ImGui::DragFloat2("Acceleration", &biki.Acceleration.x, 0.05f);
	if (ImGui::Button("Reset", ImVec2(100,20))) {
		biki.Position=Vector2(3.0f,0.0f);
		biki.Velocity=Vector2(0.0f,0.0f);
		biki.Acceleration=Vector2(0.0f,0.0f);

	}

	ImGui::DragFloat2("GVelocity", &guria.Velocity.x, 0.05f);
	ImGui::DragFloat2("GAcceleration", &guria.Acceleration.x, 0.05f);
	if (ImGui::Button("GReset", ImVec2(100,20))) {
		guria.Position = Vector2(0.0f, 0.0f);
		guria.Velocity = Vector2(0.0f, 0.0f);
		guria.Acceleration = Vector2(0.0f, 0.0f);
	}
	ImGui::DragFloat("e (Coff of Restitution)", &e, 0.05f, 0.0f, 1.0f);

}

void Collision::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}