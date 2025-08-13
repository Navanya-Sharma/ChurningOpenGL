#include "Model.h"
#include "Texture.h"

void Model::Draw(Shader& shader)
{
    for (int i = 0;i < MeshList.size();i++)
        MeshList[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP:: %s", import.GetErrorString() );
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes;i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshList.push_back(processMesh(mesh, scene));
    }
    for (int i = 0;i < node->mNumChildren;i++) {
        processNode(node->mChildren[i], scene);
    }
}

MyMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<vertex> aiVert;
    vector<unsigned int> aiInd;
    vector<picture> aiText;

    //vertice
    for (int i = 0;i < mesh->mNumVertices;i++) {
        vertex n;
        n.Position.x = mesh->mVertices[i].x;
        n.Position.y = mesh->mVertices[i].y;
        n.Position.z = mesh->mVertices[i].z;

        n.Normal.x = mesh->mNormals[i].x;
        n.Normal.y = mesh->mNormals[i].y;
        n.Normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            n.TexCords.x = mesh->mTextureCoords[0][i].x;
            n.TexCords.y = mesh->mTextureCoords[0][i].y;
        }
        else
            n.TexCords = glm::vec2(0.0f, 0.0f);
        aiVert.push_back(n);
    }
    //Indices
    for (int i = 0; i < mesh->mNumFaces;i++) {
        for(int j=0;j<mesh->mFaces[i].mNumIndices;j++)
            aiInd.push_back(mesh->mFaces[i].mIndices[j]);
    }
    //Material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        vector<picture> diff = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        aiText.insert(aiText.end(), diff.begin(), diff.end());
        vector<picture> spec = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        aiText.insert(aiText.end(), spec.begin(), spec.end());

    }

	return MyMesh(aiVert,aiInd,aiText);
}

unsigned int Model::TextureFromFile(const char* path, const std::string directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    Texture ntext(filename);
    return ntext.GetId();
}

vector<picture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    vector<picture> t;

    for (int i = 0;i < mat->GetTextureCount(type);i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool f = false;
        for (int j = 0; j < Texture_loaded.size(); j++) {
            if (std::strcmp(Texture_loaded[j].path.data(), str.C_Str()) == 0)
            {
                t.push_back(Texture_loaded[j]);
                f = true; 
            }
        }

        if (!f) {
            picture nt;

            nt.type = typeName;
            nt.id = TextureFromFile(str.C_Str(), directory);

            t.push_back(nt);
            nt.path = str.C_Str();
        }
    }
	return t;
}


