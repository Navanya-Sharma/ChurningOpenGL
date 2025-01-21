#shader compute
#version 460 core


layout(local_size_x = 256) in;

// Bind vertex buffer as shader storage buffer
layout(std430, binding = 0) buffer vbo {
    vec2 positions[];
} vertexBuffer;

layout(std430, binding = 1) buffer triangle {
    float vertices[6];
} triVert;

#define H(p) (sin(fract(sin(dot(p, vec2(12.9898, 78.233)))) * 43758.5453)*10.)*.5+.5 

void main() {
    uint index = gl_GlobalInvocationID.x;
            
    // Read position
    vec2 pos = vertexBuffer.positions[index];
            
    int r = int(H(pos))%3;
    vec2 curvert = vec2(triVert.vertices[2*r],triVert.vertices[2*r+1]);

    vertexBuffer.positions[index+25600]=vec2((pos.x + curvert.x)/2, (pos.y +curvert.y)/2);
}