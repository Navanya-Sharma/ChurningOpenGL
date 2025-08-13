#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"


using std::vector;

struct vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCords;
};

struct picture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class MyMesh
{
public:
	vector<vertex> VertList;
	vector <unsigned int > IndList;
	vector <picture> TextList;

	MyMesh(vector<vertex> Vert,	vector <unsigned int > Ind,	vector <picture> Text);
	void Draw(Shader& mShader);
private:
	VertexArray	m_vertArr;
	VertexBuffer m_vertBuff;
	IndexBuffer	m_indBuff;

	void SetupMesh();
};

