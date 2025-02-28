#include "Fuller.h"
#include <glm/gtc/matrix_transform.hpp>

Fuller::Fuller(int depth)
{
    int size = 0;
    /*1 VertSize: 72 Indi 60
    2 VertSize: 252 Indi 240
    3 VertSize: 1092 Indi 960
    4 VertSize: 5172 Indi 3840*/
    switch (depth)
    {
    case 0: size = 72; break;
    case 1: size = 252; break;
    case 2: size = 1092; break;
    case 3: size = 5172; break;
    default:
        break;
    }
    vertices = new float[size];
    india = new unsigned int[60 * pow(4, depth)];
    constexpr float PI = glm::pi<float>();

    const float Ang72 = PI / 180 * 72;

    float ang = -PI / 2 - Ang72 / 2;
    float z = sinf(atanf(1.0f / 2)); float xy = cosf(atanf(1.0f / 2));

    vertices[k++] = 0; vertices[k++] = 0; vertices[k++] = 1.0f;
    vertices[k++] = 0; vertices[k++] = 0; vertices[k++] = 1.0f;
    for (int i = 0; i < 5; ++i) {
        vertices[k++] = xy * cosf(ang); vertices[k++] = xy * sinf(ang); vertices[k++] = z;
        vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3];
        ang += Ang72;
    }
    ang = -PI / 2;
    for (int i = 0;i < 5;i++) {// z
        vertices[k++] = xy * cosf(ang); vertices[k++] = xy * sinf(ang); vertices[k++] = -z;
        vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3];
        ang += Ang72;
    }
    vertices[k++] = 0; vertices[k++] = 0; vertices[k++] = -1.0f;
    vertices[k++] = 0; vertices[k++] = 0; vertices[k++] = -1.0f;
    for (int i = 0;i < 12;i++) {
        map[std::to_string(i)] = i;
    }

    unsigned int indices[20 * 3] = {
        0,1,2,    0,2,3,     0,3,4,     0,4,5,     0, 5, 1,
        11,7,6,   11,8,7,    11,9,8,    11,10,9,   11,6,10,
        1,6,2,    2,7,3,     3,8,4,     4,9,5,     5,10,1,
        6,7,2,    7,8,3,     8,9,4,     9,10,5,    10,6,1
    };

    for (int l = 0; l < 60;l += 3) {
        std::string v0 = std::to_string(indices[l]);
        std::string v1 = std::to_string(indices[l + 1]);
        std::string v2 = std::to_string(indices[l + 2]);
        MakeFuller(v0, v1, v2, depth);
    }
}

Fuller::~Fuller()
{
    // IDK why this throws an error
    //delete india;
    //delete [] vertices;
}

void Fuller::scale(float s)
{
    for (int i = 0; i < k; i++) {
        vertices[i] *= s;
    }
}
void Fuller::translate(float x, float y, float z)
{
    for (int i = 0; i < k; i += 6) {
        vertices[i] += x;
        vertices[i + 1] += y;
        vertices[i + 2] += z;
    }
}
void Fuller::AddIndex(unsigned int n)
{
    for (int i = 0;i < id;i++) {
        india[i] += n;
    }
}
void Fuller::Print()
{
    for (int i = 0; i < k; i += 6)
    {
        printf("vertice %d: %f, %f, %f |  %f, %f, %f\n", i / 6,
            vertices[i], vertices[i + 1], vertices[i + 2],
            vertices[i + 3], vertices[i + 4], vertices[i + 5]);
    }
    printf("Indice\n");

    for (int i = 0; i < id;i += 3) {
        printf("%d %d %d \n", india[i], india[i + 1], india[i + 2]);
    }
}

std::string Fuller::Mid(std::string v0, std::string v1) {
    std::string key;

    if (std::stoi(v0) < std::stoi(v1)) {
        key = v1 + "-" + v0;
    }
    else
        key = v0 + "-" + v1;

    if (map.find(key) == map.end()) {
        map[key] = k / 6;
        float x, y, z;
        x = (vertices[map[v0] * 6] + vertices[map[v1] * 6]) / 2;
        y = (vertices[map[v0] * 6 + 1] + vertices[map[v1] * 6 + 1]) / 2;
        z = (vertices[map[v0] * 6 + 2] + vertices[map[v1] * 6 + 2]) / 2;

        glm::vec3 v = glm::normalize(glm::vec3(x, y, z));
        vertices[k++] = v.x; vertices[k++] = v.y; vertices[k++] = v.z;
        vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3]; vertices[k++] = vertices[k - 3];

        // printf("vert %d Mid: %s of %s and %s: (%f,%f,%f) Norm:(%f,%f,%f)\n",map[key], key.c_str(), v0.c_str(), v1.c_str(),
          //   vertices[k-6],vertices[k-5],vertices[k-4], vertices[k - 3], vertices[k - 2], vertices[k - 1]);
    }
    return key;
}

void Fuller::Emit(std::string v0, std::string v1, std::string v2) {
    india[id++] = map[v0];
    india[id++] = map[v1];
    india[id++] = map[v2];
}

void Fuller::MakeFuller(std::string v0, std::string v1, std::string v2, int depth) {
    if (depth == 0) {
        Emit(v0, v1, v2);
        return;
    }

    MakeFuller(v0, Mid(v0, v1), Mid(v0, v2), depth - 1);
    MakeFuller(v1, Mid(v2, v1), Mid(v0, v1), depth - 1);
    MakeFuller(v2, Mid(v0, v2), Mid(v1, v2), depth - 1);
    MakeFuller(Mid(v0, v1), Mid(v1, v2), Mid(v0, v2), depth - 1);
}
