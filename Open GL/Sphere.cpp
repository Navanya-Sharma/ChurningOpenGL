#include "Sphere.h"
#include "BufferLayout.h"
#include "IMGui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>
#include <iostream>
// Helper struct for vertex
struct Vertex {
    float x, y, z;
    float nx, ny, nz;
};


constexpr float M_PI = glm::pi<float>();

Sphere::Sphere() :
	m_vertArr(), m_vertBuff(NULL, 5000 * sizeof(float)),
	m_indBuff(NULL, 5000 * sizeof(unsigned int)), m_shader("res/Sphere.shader"),
    light_shader("res/SphereLightObject.shader"), proj(1.0f), view(1.0f), lightColor(1.0f, 1.0f, 1.0f)
{
}

void createSphere(float radius, int sectors, int stacks,
    std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;

    // Generate vertices
    for (int i = 0; i <= stacks; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;      // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectors+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // vertex tex coord between [0, 1]
            s = (float)j / sectors;
            t = (float)i / stacks;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    // Generate indices
    int k1, k2;
    for (int i = 0; i < stacks; ++i) {
        k1 = i * (sectors + 1);     // beginning of current stack
        k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}



// Helper function to calculate middle point of two vertices
int getMiddlePoint(int p1, int p2, std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& cache, float radius) {
    // First check if we have it already
    bool firstIsSmaller = p1 < p2;
    int smallerIndex = firstIsSmaller ? p1 : p2;
    int greaterIndex = firstIsSmaller ? p2 : p1;
    std::string key = std::to_string(smallerIndex) + "_" + std::to_string(greaterIndex);

    auto it = cache.find(key);
    if (it != cache.end()) {
        return it->second;
    }

    // Not in cache, calculate it
    Vertex& v1 = vertices[p1];
    Vertex& v2 = vertices[p2];

    // Middle point is halfway between vertices
    Vertex middle;
    middle.x = (v1.x + v2.x) * 0.5f;
    middle.y = (v1.y + v2.y) * 0.5f;
    middle.z = (v1.z + v2.z) * 0.5f;

    // Normalize to radius
    float length = std::sqrt(middle.x * middle.x + middle.y * middle.y + middle.z * middle.z);
    middle.x /= length;
    middle.y /= length;
    middle.z /= length;

    // Scale to the desired radius
    middle.x *= radius;
    middle.y *= radius;
    middle.z *= radius;

    // Set normal (same as position for a sphere, just normalized)
    middle.nx = middle.x / radius;
    middle.ny = middle.y / radius;
    middle.nz = middle.z / radius;

    // Add vertex to array and store in cache
    int newIndex = vertices.size();
    vertices.push_back(middle);
    cache[key] = newIndex;

    return newIndex;
}

void createIcoSphere(float radius, int subdivision,
    std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    // Clear input arrays
    vertices.clear();
    indices.clear();

    // Temp array to store vertices with their components grouped
    std::vector<Vertex> tempVertices;
    std::unordered_map<std::string, int> middlePointCache;

    // Create 12 vertices of an icosahedron
    float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

    // Normalize vertices to have length 1
    float length = std::sqrt(1.0f + t * t);
    float normalizedT = t / length;
    float normalized1 = 1.0f / length;

    // Initialize 12 vertices of icosahedron
    tempVertices = {
        {-normalized1 * radius, normalizedT * radius, 0 * radius, -normalized1, normalizedT, 0},
        {normalized1 * radius, normalizedT * radius, 0 * radius, normalized1, normalizedT, 0},
        {-normalized1 * radius, -normalizedT * radius, 0 * radius, -normalized1, -normalizedT, 0},
        {normalized1 * radius, -normalizedT * radius, 0 * radius, normalized1, -normalizedT, 0},

        {0 * radius, -normalized1 * radius, normalizedT * radius, 0, -normalized1, normalizedT},
        {0 * radius, normalized1 * radius, normalizedT * radius, 0, normalized1, normalizedT},
        {0 * radius, -normalized1 * radius, -normalizedT * radius, 0, -normalized1, -normalizedT},
        {0 * radius, normalized1 * radius, -normalizedT * radius, 0, normalized1, -normalizedT},

        {normalizedT * radius, 0 * radius, -normalized1 * radius, normalizedT, 0, -normalized1},
        {normalizedT * radius, 0 * radius, normalized1 * radius, normalizedT, 0, normalized1},
        {-normalizedT * radius, 0 * radius, -normalized1 * radius, -normalizedT, 0, -normalized1},
        {-normalizedT * radius, 0 * radius, normalized1 * radius, -normalizedT, 0, normalized1}
    };

    // Create 20 triangles of the icosahedron
    std::vector<std::vector<int>> faces = {
        // 5 faces around point 0
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        // 5 adjacent faces
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        // 5 faces around point 3
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        // 5 adjacent faces
        {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
    };

    /* Refine triangles
    for (int i = 0; i < subdivision; i++) {
        std::vector<std::vector<int>> newFaces;

        // Refine each triangle
        for (const auto& face : faces) {
            // Get the three vertices of the face
            int a = face[0];
            int b = face[1];
            int c = face[2];

            // Get the midpoints
            int ab = getMiddlePoint(a, b, tempVertices, middlePointCache, radius);
            int bc = getMiddlePoint(b, c, tempVertices, middlePointCache, radius);
            int ca = getMiddlePoint(c, a, tempVertices, middlePointCache, radius);

            // Create 4 new triangles from the original one
            newFaces.push_back({ a, ab, ca });
            newFaces.push_back({ b, bc, ab });
            newFaces.push_back({ c, ca, bc });
            newFaces.push_back({ ab, bc, ca });
        }

        // Replace old faces with new ones
        faces = newFaces;
    }

    // Add faces to final indices array
    for (const auto& face : faces) {
        indices.push_back(face[0]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
    }

    // Convert temp vertices to flat array
    for (const auto& v : tempVertices) {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);
        vertices.push_back(v.nx);
        vertices.push_back(v.ny);
        vertices.push_back(v.nz);
    }*/
}





void Sphere::Init()
{
	
	/*float vert[10*10*8*3];
	int f = 0;
	for(float i=-halfPI;i<halfPI;i+=halfPI/10)
	{
		float z=sinf(i);
		for(float j=0;j<4*halfPI;j+= halfPI / 10)
		{
			float x = cosf(j) * cosf(i);
			float y = sinf(j) * cosf(i);
			vert[f++] =  x; vert[f++] =  y; vert[f++] = z; 
			printf("%d : %f %f %f\n", f - 3, vert[f - 3], vert[f - 2], vert[f - 1]);
		}
	}
	f = 0;
	float ind[4800];
	for (int i = 0; i < 20; i++)
	{
		
	}*/

	//std::vector<float> vertices;
	
	float radius = 1.0f;
	int sectors = 36;
	int stacks = 18;
	//createSphere(radius, sectors, stacks, vertices, indices);
    //createIcoSphere(radius, 3, vertices, indices);
    // constants
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    std::vector<float> vertices(12 * 6);    // array of 12 vertices (x,y,z)
    int i1, i2;                             // indices
    float z, xy;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;                // start from -90 deg at 2nd row

    // the first top vertex at (0, 0, r)
    vertices[0] = 0;
    vertices[1] = 0;
    vertices[2] = radius;
    vertices[3] = 0;
    vertices[4] = 0;
    vertices[5] = radius;

    // compute 10 vertices at 1st and 2nd rows
    for (int i = 1; i <= 5; ++i)
    {
        i1 = i * 6;         // index for 1st row
        i2 = (i + 5) * 6;   // index for 2nd row

        z = radius * sinf(V_ANGLE);            // elevaton
        xy = radius * cosf(V_ANGLE);            // length on XY plane

        vertices[i1] = xy * cosf(hAngle1);      // x
        vertices[i2] = xy * cosf(hAngle2);
        vertices[i1 + 1] = xy * sinf(hAngle1);  // y
        vertices[i2 + 1] = xy * sinf(hAngle2);
        vertices[i1 + 2] = z;                   // z
        vertices[i2 + 2] = -z;

        vertices[i1 + 3] = vertices[i1] / radius;  // nx
        vertices[i2 + 3] = vertices[i2] / radius;
        vertices[i1 + 4] = vertices[i1 + 1] / radius;  // ny
        vertices[i2 + 4] = vertices[i2 + 1] / radius;
        vertices[i1 + 5] = vertices[i1 + 2] / radius;  // nz
        vertices[i2 + 5] = vertices[i2 + 2] / radius;

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // the last bottom vertex at (0, 0, -r)
    i1 = 11 * 6;
    vertices[i1] = 0;
    vertices[i1 + 1] = 0;
    vertices[i1 + 2] = -radius;
    vertices[i1 + 3] = 0;
    vertices[i1 + 4] = 0;
    vertices[i1 + 5] = -radius;

        for (size_t i = 0; i < 12*6; i += 6) {
    std::cout << std::setprecision(5);
            std::cout << i/6 <<"Vertex: ("
                << vertices[i] << ", "
                << vertices[i + 1] << ", "
                << vertices[i + 2] << ") Norm("
              
                << vertices[i + 3] << ", "
                << vertices[i + 4] << ", "
                << vertices[i + 5] << ")\n";
        }
    
    std::cout<<vertices.size();
    unsigned int indices[20 * 3] = {
        0,1,2,    0, 2, 3,    0, 3, 4,    0, 4, 5,    0, 5, 1,
        11,7,6,   11,8,7,    11,9,8,    11,10,9,   11,6,10,
        1,6,2,    2,7,3,    3,8,4,    4,9,5,    5,10,1,
       6,7,2,    7,8,3,    8,9,4,    9,10,5,    10,6,1
    };

    m_indBuff.SetData(indices, 60 * sizeof(unsigned int));
    m_vertBuff.SetData(vertices.data(), vertices.size() * sizeof(float));

    BufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    m_vertArr.AddBuffer(m_vertBuff, layout);
}

void Sphere::CameraImGui() {
    ImGui::Text("Sphere");

    constexpr float halfPI = glm::half_pi<float>();
    static float r = 5.0f, theta = halfPI*2, alpha = halfPI;

    ImGui::SliderFloat("Move Side", &theta, 0.0f, 4 * halfPI);
    ImGui::SliderFloat("Move Up", &alpha, 0.01f, 2 * halfPI - 0.01f);
    ImGui::SliderFloat("Radius", &r, 1.0f, 10.0f);

    float x = sin(theta) * cos(alpha +halfPI) * r;
    float y = sin(alpha+halfPI) * r;
    float z = cos(theta) * cos(alpha+halfPI) * r;

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
void Sphere::LightImGui() {
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

void Sphere::UpdateImGui() {
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

void Sphere::Update()
{
	Renderer& gRenderer = Renderer::GetRenderer();
    gRenderer.EnableDepthTest();
    gRenderer.ClearDepthBuffer();
    gRenderer.EnablePointSize(10);
   // gRenderer.DrawArray(m_vertArr, light_shader, GL_POINTS, 12);
	gRenderer.Draw(m_vertArr, m_indBuff, m_shader);
    //gRenderer.Draw(m_vertArr, m_indBuff, light_shader);
    
}

void Sphere::Close()
{
	m_vertArr.~VertexArray();
	m_vertBuff.~VertexBuffer();
	m_shader.~Shader();
	m_indBuff.~IndexBuffer();
    Renderer& gRenderer = Renderer::GetRenderer();
    GLCall(glDisable(GL_DEPTH_TEST));
}