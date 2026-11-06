#include "lux/Mesh.h"
#include <glad/gl.h>
#include <cmath>
#include <glm/gtc/constants.hpp>

namespace lux {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices) {
    m_vertexCount = (u32)vertices.size();
    m_indexCount = (u32)indices.size();
    m_hasIndices = !indices.empty();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    // Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    if (m_hasIndices) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}

void Mesh::bind() const {
    glBindVertexArray(m_vao);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}

void Mesh::draw() const {
    bind();
    if (m_hasIndices) {
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    }
    unbind();
}

Mesh Mesh::createCube(f32 size) {
    f32 s = size / 2.0f;
    std::vector<Vertex> vertices = {
        // Front face
        {{-s, -s,  s}, {0,0,1}, {0,0}, {1,0,0}},
        {{ s, -s,  s}, {0,0,1}, {1,0}, {0,1,0}},
        {{ s,  s,  s}, {0,0,1}, {1,1}, {0,0,1}},
        {{-s,  s,  s}, {0,0,1}, {0,1}, {1,1,0}},
        // Back face
        {{-s, -s, -s}, {0,0,-1}, {1,0}, {1,0,1}},
        {{-s,  s, -s}, {0,0,-1}, {1,1}, {0,1,1}},
        {{ s,  s, -s}, {0,0,-1}, {0,1}, {1,1,1}},
        {{ s, -s, -s}, {0,0,-1}, {0,0}, {0.5f,0.5f,0.5f}},
        // Top face
        {{-s,  s, -s}, {0,1,0}, {0,0}, {1,0,0}},
        {{-s,  s,  s}, {0,1,0}, {0,1}, {0,1,0}},
        {{ s,  s,  s}, {0,1,0}, {1,1}, {0,0,1}},
        {{ s,  s, -s}, {0,1,0}, {1,0}, {1,1,0}},
        // Bottom face
        {{-s, -s, -s}, {0,-1,0}, {0,1}, {1,0,1}},
        {{ s, -s, -s}, {0,-1,0}, {1,1}, {0,1,1}},
        {{ s, -s,  s}, {0,-1,0}, {1,0}, {1,1,1}},
        {{-s, -s,  s}, {0,-1,0}, {0,0}, {0.5f,0.5f,0.5f}},
        // Right face
        {{ s, -s, -s}, {1,0,0}, {0,0}, {1,0,0}},
        {{ s,  s, -s}, {1,0,0}, {0,1}, {0,1,0}},
        {{ s,  s,  s}, {1,0,0}, {1,1}, {0,0,1}},
        {{ s, -s,  s}, {1,0,0}, {1,0}, {1,1,0}},
        // Left face
        {{-s, -s, -s}, {-1,0,0}, {1,0}, {1,0,1}},
        {{-s, -s,  s}, {-1,0,0}, {0,0}, {0,1,1}},
        {{-s,  s,  s}, {-1,0,0}, {0,1}, {1,1,1}},
        {{-s,  s, -s}, {-1,0,0}, {1,1}, {0.5f,0.5f,0.5f}},
    };

    std::vector<u32> indices;
    for (u32 i = 0; i < 24; i += 4) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::createPlane(f32 width, f32 height) {
    f32 hw = width / 2.0f;
    f32 hh = height / 2.0f;
    std::vector<Vertex> vertices = {
        {{-hw, 0, -hh}, {0,1,0}, {0,0}, {0.8f,0.8f,0.8f}},
        {{ hw, 0, -hh}, {0,1,0}, {1,0}, {0.8f,0.8f,0.8f}},
        {{ hw, 0,  hh}, {0,1,0}, {1,1}, {0.8f,0.8f,0.8f}},
        {{-hw, 0,  hh}, {0,1,0}, {0,1}, {0.8f,0.8f,0.8f}},
    };
    std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
    return Mesh(vertices, indices);
}

Mesh Mesh::createQuad(f32 size) {
    f32 s = size / 2.0f;
    std::vector<Vertex> vertices = {
        {{-s, -s, 0}, {0,0,1}, {0,0}, {1,1,1}},
        {{ s, -s, 0}, {0,0,1}, {1,0}, {1,1,1}},
        {{ s,  s, 0}, {0,0,1}, {1,1}, {1,1,1}},
        {{-s,  s, 0}, {0,0,1}, {0,1}, {1,1,1}},
    };
    std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
    return Mesh(vertices, indices);
}

Mesh Mesh::createSphere(f32 radius, u32 segments) {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    for (u32 lat = 0; lat <= segments; ++lat) {
        f32 theta = lat * glm::pi<f32>() / segments;
        f32 sinTheta = sin(theta);
        f32 cosTheta = cos(theta);

        for (u32 lon = 0; lon <= segments; ++lon) {
            f32 phi = lon * 2.0f * glm::pi<f32>() / segments;
            f32 sinPhi = sin(phi);
            f32 cosPhi = cos(phi);

            glm::vec3 pos = glm::vec3(
                radius * sinTheta * cosPhi,
                radius * cosTheta,
                radius * sinTheta * sinPhi
            );
            glm::vec3 normal = glm::normalize(pos);
            glm::vec2 texCoord = glm::vec2((f32)lon / segments, (f32)lat / segments);

            vertices.push_back({pos, normal, texCoord, glm::vec3(1.0f)});
        }
    }

    for (u32 lat = 0; lat < segments; ++lat) {
        for (u32 lon = 0; lon < segments; ++lon) {
            u32 first = lat * (segments + 1) + lon;
            u32 second = first + segments + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return Mesh(vertices, indices);
}

} // namespace lux