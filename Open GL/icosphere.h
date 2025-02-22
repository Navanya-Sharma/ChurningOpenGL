#ifndef ICOSPHERE_RENDERER_H
#define ICOSPHERE_RENDERER_H

#include <vector>
#include <unordered_map>
#include <cmath>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class IcosphereRenderer {
private:
    struct Vertex {
        float x, y, z;      // Position
        float nx, ny, nz;   // Normal
    };

    GLuint vao, vbo, ibo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int subdivisionLevel;

    // Helper method to create a key from two vertex indices
    uint64_t getEdgeKey(int i1, int i2);

    // Normalize a vector
    void normalize(float& x, float& y, float& z);

    // Create initial icosahedron
    void createIcosahedron();

    // Subdivide the mesh
    void subdivide();

    // Get or create midpoint vertex between two vertices
    int getMidpointIndex(std::unordered_map<uint64_t, int>& cache, int i1, int i2);

public:
    // Constructor: creates an icosphere with specified subdivision level
    IcosphereRenderer(int subdivisions = 3);

    // Destructor: cleans up OpenGL resources
    ~IcosphereRenderer();

    // Set up OpenGL buffers
    void setupBuffers();

    // Basic render method
    void render();

    // Render with custom shader
    void render(GLuint shaderProgram);

    // Utility function to get vertex count
    size_t getVertexCount() const;

    // Utility function to get triangle count
    size_t getTriangleCount() const;
};

// Helper function to easily create and render an icosphere
void renderIcosphere(float radius, int subdivisions, float x, float y, float z, GLuint shaderProgram);

#endif // ICOSPHERE_RENDERER_H