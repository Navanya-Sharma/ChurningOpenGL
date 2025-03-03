#include "Spheree.h"
#include "BufferLayout.h"
#include "IMGui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

Spheree::Spheree():
	m_vertArr(), m_vertBuff(NULL, 5200 * sizeof(float)),
	m_indBuff(NULL, 5000 * sizeof(unsigned int)), m_shader("res/Sphere.shader"),
	light_shader("res/SphereLightObject.shader"), proj(1.0f), view(1.0f), lightColor(1.0f, 1.0f, 1.0f), ico(2)
{
}

void Spheree::Init()
{
    BufferLayout lay;
    lay.Push<float>(3);
    lay.Push<float>(3);
    m_vertBuff.SetData(ico.getVertData(),ico.getVertSize()*sizeof(float));
    m_indBuff.SetData(ico.getIndData(),ico.getIndSize()*sizeof(unsigned int));
    m_vertArr.AddBuffer(m_vertBuff,lay);
}

void Spheree::UpdateImGui() {
    CameraImGui();
    LightImGui();

    m_shader.SetUniform3f("uLight.ambient", lightColor.x * 0.2f, lightColor.x * 0.2f, lightColor.x * 0.2f);
    m_shader.SetUniform3f("uLight.diffuse", lightColor.x * 0.5f, lightColor.x * 0.5f, lightColor.x * 0.5f);
    m_shader.SetUniform3f("uLight.specular", 1.0f, 1.0f, 1.0f);
    m_shader.SetUniform3f("uMat.ambient", 1.0f, 0.5f, 0.31f);
    m_shader.SetUniform3f("uMat.diffuse", 1.0f, 0.5f, 0.31f);
    m_shader.SetUniform3f("uMat.specular", 0.5f, 0.5f, 0.5f);
    float shine = 32.0f;
    m_shader.SetUniform1fv("uMat.shine", 1, &shine);


}
void Spheree::CameraImGui() {
    ImGui::Text("Sphere");

    constexpr float halfPI = glm::half_pi<float>();
    static float r = 5.0f, theta = halfPI * 2, alpha = halfPI;

    ImGui::SliderFloat("Move Side", &theta, 0.0f, 4 * halfPI);
    ImGui::SliderFloat("Move Up", &alpha, 0.01f, 2 * halfPI - 0.01f);
    ImGui::SliderFloat("Radius", &r, 1.0f, 10.0f);

    float x = sin(theta) * cos(alpha + halfPI) * r;
    float y = sin(alpha + halfPI) * r;
    float z = cos(theta) * cos(alpha + halfPI) * r;

    proj = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);
    view = glm::lookAt(glm::vec3(x, y, z),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 MVP = proj * view * model;

    m_shader.SetUniformMat4("uMVP", MVP);
    m_shader.SetUniformMat4("uModel", model);
    m_shader.SetUniform3f("uVeiw", x, y, z);
}
void Spheree::LightImGui() {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(lightColor), ImGuiColorEditFlags_NoInputs);

    static float xl = 3.0f, yl = 3.0f, zl = 3.0f;


    ImGui::SliderFloat("x", &xl, -3.0f, 3.0f);
    ImGui::SliderFloat("y", &yl, -3.0f, 3.0f);
    ImGui::SliderFloat("z", &zl, -3.0f, 3.0f);

    glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xl, yl, zl));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4x4 MVP = proj * view * model * scale;

    light_shader.SetUniformMat4("uMVP", MVP);
    light_shader.SetUniform4f("uLight", lightColor.x, lightColor.y, lightColor.z, 1.0f);
    m_shader.SetUniform3f("uLight.position", xl, yl, zl);
}

void Spheree::Update()
{
    Renderer& gRenderer = Renderer::GetRenderer();
    gRenderer.EnableDepthTest();
    gRenderer.ClearDepthBuffer();
    gRenderer.Draw(m_vertArr, m_indBuff, m_shader);

}

void Spheree::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
}