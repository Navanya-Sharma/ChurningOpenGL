#include <vector>
#include <unordered_map>
#include <cmath>
#include "glad/glad.h"

class IcosphereRenderer {
private:
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
    };

    GLuint vao, vbo, ibo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int subdivisionLevel;

    // Helper method to create a key from two vertex indices
    uint64_t getEdgeKey(int i1, int i2) {
        return (i1 < i2) ? ((uint64_t)i1 << 32) | i2 : ((uint64_t)i2 << 32) | i1;
    }

    // Normalize a vector
    void normalize(float& x, float& y, float& z) {
        float length = sqrt(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;
    }

    // Create initial icosahedron
    void createIcosahedron() {
        // Golden ratio
        const float t = (1.0f + sqrt(5.0f)) / 2.0f;

        // Create 12 vertices of icosahedron
        float vertices_data[] = {
            -1.0f,  t,  0.0f,
             1.0f,  t,  0.0f,
            -1.0f, -t,  0.0f,
             1.0f, -t,  0.0f,

             0.0f, -1.0f,  t,
             0.0f,  1.0f,  t,
             0.0f, -1.0f, -t,
             0.0f,  1.0f, -t,

             t,  0.0f, -1.0f,
             t,  0.0f,  1.0f,
            -t,  0.0f, -1.0f,
            -t,  0.0f,  1.0f
        };

        // Normalize vertices to lie on unit sphere
        for (int i = 0; i < 12; i++) {
            float x = vertices_data[i * 3];
            float y = vertices_data[i * 3 + 1];
            float z = vertices_data[i * 3 + 2];
            normalize(x, y, z);

            Vertex v;
            v.x = x; v.y = y; v.z = z;
            v.nx = x; v.ny = y; v.nz = z;  // Normal = position for sphere
            vertices.push_back(v);
        }

        // Define the 20 triangular faces of the icosahedron
        unsigned int indices_data[] = {
            0, 11, 5,
            0, 5, 1,
            0, 1, 7,
            0, 7, 10,
            0, 10, 11,

            1, 5, 9,
            5, 11, 4,
            11, 10, 2,
            10, 7, 6,
            7, 1, 8,

            3, 9, 4,
            3, 4, 2,
            3, 2, 6,
            3, 6, 8,
            3, 8, 9,

            4, 9, 5,
            2, 4, 11,
            6, 2, 10,
            8, 6, 7,
            9, 8, 1
        };

        for (int i = 0; i < 60; i++) {
            indices.push_back(indices_data[i]);
        }
    }

    // Subdivide the mesh
    void subdivide() {
        for (int i = 0; i < subdivisionLevel; i++) {
            std::vector<unsigned int> newIndices;
            std::unordered_map<uint64_t, int> midVertexCache;

            for (size_t j = 0; j < indices.size(); j += 3) {
                unsigned int i1 = indices[j];
                unsigned int i2 = indices[j + 1];
                unsigned int i3 = indices[j + 2];

                // Get the three original vertices
                Vertex v1 = vertices[i1];
                Vertex v2 = vertices[i2];
                Vertex v3 = vertices[i3];

                // Get or create mid points
                int m1 = getMidpointIndex(midVertexCache, i1, i2);
                int m2 = getMidpointIndex(midVertexCache, i2, i3);
                int m3 = getMidpointIndex(midVertexCache, i3, i1);

                // Create 4 new triangles
                newIndices.push_back(i1); newIndices.push_back(m1); newIndices.push_back(m3);
                newIndices.push_back(i2); newIndices.push_back(m2); newIndices.push_back(m1);
                newIndices.push_back(i3); newIndices.push_back(m3); newIndices.push_back(m2);
                newIndices.push_back(m1); newIndices.push_back(m2); newIndices.push_back(m3);
            }

            indices = newIndices;
        }
    }

    // Get or create midpoint vertex between two vertices
    int getMidpointIndex(std::unordered_map<uint64_t, int>& cache, int i1, int i2) {
        uint64_t key = getEdgeKey(i1, i2);

        auto it = cache.find(key);
        if (it != cache.end()) {
            return it->second;
        }

        // Create new vertex at the midpoint
        Vertex v1 = vertices[i1];
        Vertex v2 = vertices[i2];

        Vertex newVertex;
        newVertex.x = (v1.x + v2.x) * 0.5f;
        newVertex.y = (v1.y + v2.y) * 0.5f;
        newVertex.z = (v1.z + v2.z) * 0.5f;

        // Project to unit sphere
        normalize(newVertex.x, newVertex.y, newVertex.z);

        // Normal is same as position for unit sphere
        newVertex.nx = newVertex.x;
        newVertex.ny = newVertex.y;
        newVertex.nz = newVertex.z;

        // Add the new vertex
        int newIndex = vertices.size();
        vertices.push_back(newVertex);

        // Cache the midpoint
        cache[key] = newIndex;

        return newIndex;
    }

public:
    IcosphereRenderer(int subdivisions = 3) : subdivisionLevel(subdivisions) {
        createIcosahedron();
        subdivide();
        setupBuffers();
    }

    ~IcosphereRenderer() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }

    void setupBuffers() {
        // Create and bind VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Create IBO
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Unbind VAO
        glBindVertexArray(0);
    }

    void render() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Render with custom shader
    void render(GLuint shaderProgram) {
        glUseProgram(shaderProgram);
        render();
    }

    // Utility function to get vertex count
    size_t getVertexCount() const {
        return vertices.size();
    }

    // Utility function to get triangle count
    size_t getTriangleCount() const {
        return indices.size() / 3;
    }
};

// Helper function to easily create and render an icosphere
void renderIcosphere(float radius, int subdivisions, float x, float y, float z) {
    // Create the icosphere
    static IcosphereRenderer icosphere(subdivisions);



    // Render the icosphere
    icosphere.render();
}