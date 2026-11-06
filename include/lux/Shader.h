#pragma once

#include "lux/Core.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace lux {

class Shader {
public:
    Shader() = default;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, i32 value) const;
    void setFloat(const std::string& name, f32 value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    u32 getID() const { return m_id; }

    static std::shared_ptr<Shader> fromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Shader> fromSource(const std::string& vertexSrc, const std::string& fragmentSrc);

private:
    u32 m_id = 0;
    mutable std::unordered_map<std::string, i32> m_uniformCache;

    i32 getUniformLocation(const std::string& name) const;
    bool compile(const std::string& vertexSrc, const std::string& fragmentSrc);
    std::string readFile(const std::string& path);
};

} // namespace lux