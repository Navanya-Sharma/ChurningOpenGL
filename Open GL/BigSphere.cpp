#include "BigSphere.h"
#include "BufferLayout.h"
#include "IMGui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include "time.h"

const BigSphere::Material BigSphere::materials[15] = {
    // Metals - Significantly increased ambient values
    {   // Gold
        glm::vec3(0.44725f, 0.3995f, 0.2745f),    // Higher ambient
        glm::vec3(0.85164f, 0.70648f, 0.32648f),  // Increased diffuse
        glm::vec3(0.628281f, 0.555802f, 0.366065f),
        51.2f
    },
    {   // Silver
        glm::vec3(0.39225f, 0.39225f, 0.39225f),  // Higher ambient
        glm::vec3(0.60754f, 0.60754f, 0.60754f),  // Increased diffuse
        glm::vec3(0.508273f, 0.508273f, 0.508273f),
        51.2f
    },
    {   // Copper
        glm::vec3(0.39125f, 0.2735f, 0.2225f),    // Higher ambient
        glm::vec3(0.8038f, 0.37048f, 0.1828f),    // Increased diffuse
        glm::vec3(0.256777f, 0.137622f, 0.086014f),
        12.8f
    },
    {   // Bronze
        glm::vec3(0.4125f, 0.3275f, 0.254f),      // Higher ambient
        glm::vec3(0.814f, 0.5284f, 0.28144f),     // Increased diffuse
        glm::vec3(0.393548f, 0.271906f, 0.166721f),
        25.6f
    },
    // Gems - Much higher ambient and diffuse
    {   // Ruby
        glm::vec3(0.3745f, 0.21175f, 0.21175f),
        glm::vec3(0.81424f, 0.24136f, 0.24136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        76.8f
    },
    {   // Emerald
        glm::vec3(0.2215f, 0.3745f, 0.2215f),
        glm::vec3(0.27568f, 0.81424f, 0.27568f),
        glm::vec3(0.633f, 0.727811f, 0.633f),
        76.8f
    },
    {   // Sapphire
        glm::vec3(0.21175f, 0.21175f, 0.3745f),
        glm::vec3(0.24136f, 0.24136f, 0.81424f),
        glm::vec3(0.626959f, 0.626959f, 0.727811f),
        76.8f
    },
    // Plastics - Higher ambient and diffuse
    {   // Red Plastic
        glm::vec3(0.2f, 0.05f, 0.05f),
        glm::vec3(0.7f, 0.2f, 0.2f),
        glm::vec3(0.7f, 0.6f, 0.6f),
        32.0f
    },
    {   // Green Plastic
        glm::vec3(0.05f, 0.2f, 0.05f),
        glm::vec3(0.2f, 0.55f, 0.2f),
        glm::vec3(0.45f, 0.55f, 0.45f),
        32.0f
    },
    {   // Blue Plastic
        glm::vec3(0.05f, 0.05f, 0.2f),
        glm::vec3(0.2f, 0.2f, 0.7f),
        glm::vec3(0.6f, 0.6f, 0.7f),
        32.0f
    },
    // Special Materials - Significantly adjusted for better visibility
    {   // Pearl
        glm::vec3(0.45f, 0.40725f, 0.40725f),
        glm::vec3(1.0f, 0.829f, 0.829f),
        glm::vec3(0.296648f, 0.296648f, 0.296648f),
        11.264f
    },
    {   // Obsidian - Kept darker but still visible
        glm::vec3(0.25375f, 0.25f, 0.26625f),
        glm::vec3(0.38275f, 0.37f, 0.42525f),
        glm::vec3(0.332741f, 0.328634f, 0.346435f),
        38.4f
    },
    {   // Chrome
        glm::vec3(0.45f, 0.45f, 0.45f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.774597f, 0.774597f, 0.774597f),
        76.8f
    },
    {   // Rubber
        glm::vec3(0.22f, 0.22f, 0.22f),
        glm::vec3(0.31f, 0.31f, 0.31f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        10.0f
    },
    {   // Brushed Aluminum
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        glm::vec3(0.6f, 0.6f, 0.6f),
        20.0f
    }
};

BigSphere::BigSphere() :
    vertArr2(), vertBuff2{ {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 100 * 1092 * sizeof(float)},
                     {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 100 * 1092 * sizeof(float)},
                     {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 100 * 1092 * sizeof(float)},
                     {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 100 * 1092 * sizeof(float)},
                     {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 100 * 1092 * sizeof(float)}, {nullptr, 1000 * 1092 * sizeof(float)} },
    m_PointsBuff(NULL, 1331 * 3 * sizeof(float)),
    instBuff(nullptr,3*1000*sizeof(float)),
    indBuff2{ {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 100 * 960 * sizeof(unsigned int)},
              {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 100 * 960 * sizeof(unsigned int)},
              {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 100 * 960 * sizeof(unsigned int)},
              {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 100 * 960 * sizeof(unsigned int)},
              {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 100 * 960 * sizeof(unsigned int)}, {nullptr, 1000 * 960 * sizeof(unsigned int)} },
    m_shader("res/BigSphere.shader"),simple("res/BigSphereSimple.Shader"), light_shader("res/SphereLightObject.shader"),
    proj(1.0f), viewPos(0.0f), lightColor(1.0f, 1.0f, 1.0f), ico2(2), CamFrust(glm::vec3(1.0f), glm::vec3(0.0f), 1.0f, 100.0f)
{
}

void BigSphere::Init()
{  
    BufferLayout layout, instlay;
    layout.Push<float>(3);
    layout.Push<float>(3);
    instlay.Push<float>(3);
    
    
    ico2.scale(0.1); //set the radius of the sphere to 0.1
     
    unsigned int iSize = ico2.getIndSize() * sizeof(unsigned int);
    unsigned int vSize = ico2.getVertSize() * sizeof(float);
    unsigned int indOffset = ico2.getVertSize()/6;
    
    //vector of all 1000 points
    std::vector<glm::vec3> Points;
    srand(time(0));
    vertArr2[10].Bind();
    //The Code generate a Sphere and then a 10x10x10 Spheres in the space.
    //I have tried to have 10 different materials and also a simple version with one material but not much changed.
    for (int i = 0;i < 10;i++) {
        for (int j = 0;j < 10;j++) {
            for (int k = 0;k < 10;k++) {
                Points.push_back(glm::vec3(((float)(i-5) / 5 + 0.1), ((float)(j-5) / 5 + 0.1), ((float)(k-5) / 5 + 0.1)));
                glm::vec3 p = Points.back();
                ico2.translate(p.x, p.y, p.z);
                indBuff2[10].SetData(ico2.getIndData(), iSize, (i*100+j*10+k) * iSize);
                vertBuff2[10].SetData(ico2.getVertData(), vSize, (i * 100 + j * 10 + k) * vSize);
                ico2.AddIndex(indOffset);
                ico2.translate(-p.x, -p.y, -p.z);

            }
        }
    }
    vertArr2[10].AddBuffer(vertBuff2[10], layout);

    ico2.AddIndex(indOffset * -1000);

    //Using it to generate different materials. So for 10 materials there are randomly alloted points in the space
    //and the sphere is rendered at those points with different materials
    vertArr2[0].Bind();
    random_shuffle(Points.begin(), Points.end());
    for (int i = 0;i<Points.size();i++) {
        int id = i / 100;
        glm::vec3 p = Points[i];
        ico2.translate(p.x, p.y, p.z);
        indBuff2[id].SetData(ico2.getIndData(), iSize, (i%100 ) * iSize);
        vertBuff2[id].SetData(ico2.getVertData(), vSize, (i%100 ) * vSize);
        
        ico2.AddIndex(indOffset);
        ico2.translate(-p.x, -p.y,-p.z);
        if (i % 100 == 99) {
            vertArr2[id].AddBuffer(vertBuff2[id], layout);
            //vertArr2[id].AddInstanced(instBuff, instlay, 2);
            vertArr2[id+1].Bind();
            ico2.AddIndex(indOffset*-100);
        }
	}
    
    /*float cube[11 * 11 * 11 * 3];
    int index = 0;
    for (int x = -10; x <= 10; x += 2) {
        for (int y = -10; y <= 10; y += 2) {
            for (int z = -10; z <= 10; z += 2) {
                cube[index++] = x;
                cube[index++] = y;
                cube[index++] = z;
            }
        }
    }

    m_PointArr.Bind();
    m_PointsBuff.SetData(cube, 1331 * 3 * sizeof(float));
    m_PointArr.AddBuffer(m_PointsBuff, instlay);*/
}

void BigSphere::CameraImGui() {
   

    constexpr float halfPI = glm::half_pi<float>();
    static float r = 5.0f, theta = 2*halfPI, alpha = halfPI;

    ImGui::SliderFloat("Move Side", &theta, 0.0f, 4 * halfPI);
    ImGui::SliderFloat("Move Up", &alpha, 0.01f, 2 * halfPI - 0.01f);
    ImGui::SliderFloat("Radius", &r, 1.0f, 100.0f);

    float x = sin(theta) * cos(alpha +halfPI) * r;
    float y = sin(alpha+halfPI) * r;
    float z = cos(theta) * cos(alpha+halfPI) * r;

    viewPos=glm::vec3(x, y, z);
    glm::mat4x4 view = glm::lookAt(viewPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    
    proj = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 30.0f);
    m_shader.SetUniform3f("uVeiw", x, y, z);
    

}
void BigSphere::LightImGui() {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(lightColor), ImGuiColorEditFlags_NoInputs);

    static float xl = 0.0f, yl = 0.0f, zl = 10.0f;


    ImGui::SliderFloat("x", &xl, -3.0f, 3.0f);
    ImGui::SliderFloat("y", &yl, -3.0f, 3.0f);
    ImGui::SliderFloat("z", &zl, -3.0f, 3.0f);

    glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xl, yl, zl));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
     glm::mat4x4 view = glm::lookAt(viewPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4x4 MVP = proj * view * model * scale;

    light_shader.SetUniformMat4("uMVP", MVP);
    light_shader.SetUniform4f("uLight.ambient", lightColor.x, lightColor.y, lightColor.z, 1.0f);
    light_shader.SetUniform4f("uLight.diffuse", lightColor.x, lightColor.y, lightColor.z, 1.0f);
    light_shader.SetUniform4f("uLight.specular", lightColor.x, lightColor.y, lightColor.z, 1.0f);
    m_shader.SetUniform3f("uLight.position", xl, yl, zl);
}
void BigSphere::UpdateImGui() {
    ImGui::Text("BigSphere");

	CameraImGui();
    LightImGui();
    
    m_shader.SetUniform3f("uLight.ambient", lightColor.x * 0.2f, lightColor.x * 0.2f, lightColor.x * 0.2f);
    m_shader.SetUniform3f("uLight.diffuse", lightColor.x * 0.5f, lightColor.x * 0.5f, lightColor.x * 0.5f);
    m_shader.SetUniform3f("uLight.specular", 1.0f, 1.0f, 1.0f);
    
}

void BigSphere::Update()
{   Renderer& gRenderer = Renderer::GetRenderer();
    gRenderer.Clear();
    gRenderer.EnableDepthTest();
    gRenderer.ClearDepthBuffer();
    //gRenderer.EnablePointSize(10);
    //Face Culling
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    CamFrust = FrustCull(-viewPos, viewPos, 1.0f, 30.0f);
    float s = 4.0;

    //Depth Culling
    GLCall(glDepthMask(true));
    GLCall(glColorMask(true, true, true, true));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // first )pass
    GLCall(glColorMask(false, false, false, false));
    GLCall(glDepthMask(true));
    GLCall(glDepthFunc(GL_LESS));
    
    SimpleFrustRender(glm::vec3(-s, -s, s), s*2, 3);

    // second )pass
    GLCall(glColorMask(true, true, true, true));
    GLCall(glDepthMask(false));
    GLCall(glDepthFunc(GL_EQUAL));
    FrustRender(glm::vec3(-s, -s, s), s*2, 3);
   /*  glm::mat4x4 view = glm::lookAt(viewPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 MVP = proj * view;
    m_shader.SetUniformMat4("uMVP", MVP);
    m_shader.SetUniformMat4("uModel", glm::mat4x4(1.0f));

    Material mat = materials[10];
    float matarr[10] = { mat.ambient.x,  mat.ambient.y, mat.ambient.z,
                            mat.diffuse.x,  mat.diffuse.y, mat.diffuse.z,
                            mat.specular.x, mat.specular.y, mat.specular.z,mat.shine };
    m_shader.SetUniform1fv("uMatArr", 10, matarr);
    gRenderer.Draw(vertArr2[10], indBuff2[10], m_shader);
    //Render(glm::vec3(-1, -1, 1), 2, 3);
   
    gRenderer.DrawInstance(vertArr2[10], indBuff2[10], m_shader,1000);*/
}
//FrustRender Does Frustrum Culling with an Octree Structure. It checks if the 8 points of the cube are inside the frustum
//If all 8 points are inside the frustum then it renders the cube. If not then it checks for the 8 sub cubes of the cube
//and repeats the process. If the depth is 0 then it renders the cube if at least 4 points are inside
void BigSphere::FrustRender(glm::vec3 BotLeftF, float dis, int depth)
{
    
    bool inn[8];
    inn[0] = CamFrust.CheckInside(BotLeftF);
    inn[1] = CamFrust.CheckInside(BotLeftF + glm::vec3(dis, 0.0f, 0.0f));
    inn[2] = CamFrust.CheckInside(BotLeftF + glm::vec3( dis, dis, 0.0f));
    inn[3] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f, dis, 0.0f));
    inn[4] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f, 0.0f,-dis));
    inn[5] = CamFrust.CheckInside(BotLeftF + glm::vec3( dis, 0.0f,-dis));
    inn[6] = CamFrust.CheckInside(BotLeftF + glm::vec3( dis,  dis,-dis));
    inn[7] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f,  dis,-dis));

    int sum = inn[0] + inn[1] + inn[2] + inn[3] + inn[4] + inn[5] + inn[6] + inn[7];
    if (depth == 0) {
        if (sum>=4) {
            //printf("Depth 0 Point: %f, %f, %f - %f\n", BotLeftF.x, BotLeftF.y, BotLeftF.z, dis);
            Render(BotLeftF, dis, depth);
           
        } 
        return;
     }
    if (sum==8) {
        //printf("ALL INN");
        //printf(": %f, %f, %f - %f\n", BotLeftF.x, BotLeftF.y, BotLeftF.z, dis);
        Render(BotLeftF, dis, depth);
    }
    else {
        FrustRender(BotLeftF, dis / 2, depth - 1);
        FrustRender(BotLeftF + glm::vec3(dis / 2, 0.0f, 0.0f), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(dis / 2, dis / 2, 0.0f), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(0.0f, dis / 2, 0.0f), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(0.0f, 0.0f, -dis / 2), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(dis / 2, 0.0f, -dis / 2), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(dis / 2, dis / 2,-dis / 2), dis / 2, depth - 1); 
        FrustRender(BotLeftF + glm::vec3(0.0f, dis / 2,-dis / 2), dis / 2, depth - 1); 
    }


}
void BigSphere::SimpleFrustRender(glm::vec3 BotLeftF, float dis, int depth)
{

    bool inn[8];
    inn[0] = CamFrust.CheckInside(BotLeftF);
    inn[1] = CamFrust.CheckInside(BotLeftF + glm::vec3(dis, 0.0f, 0.0f));
    inn[2] = CamFrust.CheckInside(BotLeftF + glm::vec3(dis, dis, 0.0f));
    inn[3] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f, dis, 0.0f));
    inn[4] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f, 0.0f, -dis));
    inn[5] = CamFrust.CheckInside(BotLeftF + glm::vec3(dis, 0.0f, -dis));
    inn[6] = CamFrust.CheckInside(BotLeftF + glm::vec3(dis, dis, -dis));
    inn[7] = CamFrust.CheckInside(BotLeftF + glm::vec3(0.0f, dis, -dis));

    int sum = inn[0] + inn[1] + inn[2] + inn[3] + inn[4] + inn[5] + inn[6] + inn[7];
    if (depth == 0) {
        if (sum >= 4) {
            //printf("Depth 0 Point: %f, %f, %f - %f\n", BotLeftF.x, BotLeftF.y, BotLeftF.z, dis);
            SimpleRender(BotLeftF, dis, depth);

        }
        return;
    }
    if (sum == 8) {
        //printf("ALL INN");
        //printf(": %f, %f, %f - %f\n", BotLeftF.x, BotLeftF.y, BotLeftF.z, dis);
        SimpleRender(BotLeftF, dis, depth);
    }
    else {
        SimpleFrustRender(BotLeftF, dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(dis / 2, 0.0f, 0.0f), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(dis / 2, dis / 2, 0.0f), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(0.0f, dis / 2, 0.0f), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(0.0f, 0.0f, -dis / 2), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(dis / 2, 0.0f, -dis / 2), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(dis / 2, dis / 2, -dis / 2), dis / 2, depth - 1);
        SimpleFrustRender(BotLeftF + glm::vec3(0.0f, dis / 2, -dis / 2), dis / 2, depth - 1);
    }


}
void BigSphere::Render(glm::vec3 BotLeftF, float dis, int depth)
{
    Renderer& gRenderer = Renderer::GetRenderer();

    glm::mat4x4 view = glm::lookAt(viewPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    Material mat = materials[5];
    float matarr[10] = { mat.ambient.x,  mat.ambient.y, mat.ambient.z,
                            mat.diffuse.x,  mat.diffuse.y, mat.diffuse.z,
                            mat.specular.x, mat.specular.y, mat.specular.z,mat.shine };
    m_shader.SetUniform1fv("uMatArr", 10, matarr);

    for (float z = BotLeftF.z-1;z > BotLeftF.z - dis; z -= 2) {
        for (float y = BotLeftF.y+1; y < BotLeftF.y + dis; y += 2) {
            for (float x = BotLeftF.x+1; x < BotLeftF.x + dis; x += 2) {
               
                glm::mat4 model=glm::translate(glm::mat4x4(1.0f), glm::vec3(x, y, z));
                glm::mat4 MVP = proj * view* model;
                m_shader.SetUniformMat4("uMVP", MVP);
                m_shader.SetUniformMat4("uModel", model);
                gRenderer.Draw(vertArr2[10], indBuff2[10], m_shader);

                /*for (int i = 0; i < 10; i++) {

                    Material mat = materials[i];
                    float matarr[10] = { mat.ambient.x,  mat.ambient.y, mat.ambient.z,
                                            mat.diffuse.x,  mat.diffuse.y, mat.diffuse.z,
                                            mat.specular.x, mat.specular.y, mat.specular.z,mat.shine };
                    m_shader.SetUniform1fv("uMatArr", 10, matarr);
                   
                    gRenderer.Draw(vertArr2[i], indBuff2[i], m_shader);
                    
                }*/
            }
        }
    }


   
}

//Simple Render and Simple Furst are for the first depth pass with a simple bare minimun shader
void BigSphere::SimpleRender(glm::vec3 BotLeftF, float dis, int depth)
{
    Renderer& gRenderer = Renderer::GetRenderer();

    glm::mat4x4 view = glm::lookAt(viewPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
   
    for (float z = BotLeftF.z - 1;z > BotLeftF.z - dis; z -= 2) {
        for (float y = BotLeftF.y + 1; y < BotLeftF.y + dis; y += 2) {
            for (float x = BotLeftF.x + 1; x < BotLeftF.x + dis; x += 2) {
                glm::mat4 model = glm::translate(glm::mat4x4(1.0f), glm::vec3(x, y, z));
                glm::mat4 MVP = proj * view * model;
                simple.SetUniformMat4("uMVP", MVP);
                
                gRenderer.Draw(vertArr2[10], indBuff2[10], simple);
            }
        }
    }
}

// Remove testing function - Not used anywhere. Just to check a given list of points are in the Furstrum or not!
void BigSphere::checkPoints() {
   
    int n = 0;
    for (int x = -10; x <= 10; x += 2) {
        for (int y = -10; y <= 10; y += 2) {
            for (int z = -10; z <= 10; z += 2) {
                glm::vec3 point(x, y, z);
                //printf("Point %d %f %f %f : %d\n", i/3, point.x, point.y, point.z, CamFrust.CheckInside(point));
                n += CamFrust.CheckInside(point);

            }
        }
    }
    printf("Total Inside: %d\n", n);

}

void BigSphere::Close()
    {
	vertArr2[0].~VertexArray();
	vertBuff2[0].~VertexBuffer();
	indBuff2[0].~IndexBuffer();
	m_shader.~Shader();
    Renderer& gRenderer = Renderer::GetRenderer();
    GLCall(glDisable(GL_DEPTH_TEST));
}

