#include "MyMesh.h"
#include "BufferLayout.h"
#include "Renderer.h"
#include <memory>


MyMesh::MyMesh(vector<vertex> Vert, vector<unsigned int> Ind, vector <Texture> Text)
{
	VertList = Vert;
	IndList = Ind;
	//TextList = Text;
    Texture a("res/emoji.png");
    TextList.push_back(a);
    
    TextList[0].Bind();
	SetupMesh();
}

void MyMesh::Draw(Shader& mShader)
{
    /*int diff = 1;
    int spec = 1;

    for (int i = 0;i < TextList.size();i++) {
        std::string name;
        if (TextList[i].type == "texture_diffuse") {
            name = std::to_string(diff++);
        }
        else {
            name = std::to_string(spec++);
        }
        name = "material" + TextList[i].type + name;
        mShader.SetUniform1i(name, i);
    }*/
    TextList[0].Bind(1);
    mShader.SetUniform1i("materialtexture_diffuse1", 0);
   // m_vertArr.Bind();
    Renderer& gRenderer = Renderer::GetRenderer();
    gRenderer.Draw(m_vertArr, *m_indBuff, mShader);

}

void MyMesh::SetupMesh()
{
    m_vertArr.Bind();
	//m_vertBuff.Bind();
    //float vert[6] = {0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    //m_vertBuff.SetData(vert, 6 * sizeof(float));

    m_vertBuff = std::make_unique<VertexBuffer>(&VertList[0], VertList.size() * sizeof(vertex));
    //std::cout << m_vertBuff->GetID() << std::endl;
    
    //m_vertBuff->Bind();
   // GLCall(glBufferData(GL_ARRAY_BUFFER, 0, VertList.size()*sizeof(vertex), &VertList[0]));
    m_indBuff = std::make_unique<IndexBuffer>(&IndList[0], IndList.size());
    //GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, IndList.size() * sizeof(unsigned int), &IndList[0]));

    BufferLayout lay;
    lay.Push<float>(3);
    lay.Push<float>(3);
    lay.Push<float>(2);

    m_vertArr.AddBuffer(*m_vertBuff, lay);
    m_vertArr.Bind();


}
/*
void MyMesh::SetupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &VertList[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}
*/