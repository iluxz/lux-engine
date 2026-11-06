#pragma once

#include "lux/Core.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace lux {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color = glm::vec3(1.0f);
};

class Mesh {
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
    ~Mesh();

    void bind() const;
    void unbind() const;
    void draw() const;

    u32 getVertexCount() const { return m_vertexCount; }
    u32 getIndexCount() const { return m_indexCount; }

    // Primitive generators
    static Mesh createCube(f32 size = 1.0f);
    static Mesh createSphere(f32 radius = 1.0f, u32 segments = 32);
    static Mesh createPlane(f32 width = 1.0f, f32 height = 1.0f);
    static Mesh createQuad(f32 size = 1.0f);

private:
    u32 m_vao = 0;
    u32 m_vbo = 0;
    u32 m_ebo = 0;
    u32 m_vertexCount = 0;
    u32 m_indexCount = 0;
    bool m_hasIndices = false;
};

} // namespace lux