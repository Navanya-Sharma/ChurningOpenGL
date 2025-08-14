#pragma once
#include "Shader.h"
#include <vector>
#include <iostream>
#include "MyMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using std::vector;

class Model
{
public:
	Model(const char* path) {
		loadModel(path);
	}
	void Draw(Shader& shader);

private:
	vector<MyMesh> MeshList;
	std::string directory ;
	vector<picture> Texture_loaded;


	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	MyMesh processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int TextureFromFile(const char* path,const std::string directory);

	vector <picture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

