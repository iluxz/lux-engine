#include "lux/Shader.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace lux {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSrc = readFile(vertexPath);
    std::string fragmentSrc = readFile(fragmentPath);
    compile(vertexSrc, fragmentSrc);
}

Shader::~Shader() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
}

void Shader::bind() const {
    glUseProgram(m_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

i32 Shader::getUniformLocation(const std::string& name) const {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) return it->second;

    i32 location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        LUX_WARN("Uniform '%s' not found", name.c_str());
    }
    m_uniformCache[name] = location;
    return location;
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), (i32)value);
}

void Shader::setInt(const std::string& name, i32 value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, f32 value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

bool Shader::compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
    // Vertex shader
    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    i32 success;
    char infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        LUX_ERROR("Vertex shader compilation failed:\n%s", infoLog);
        glDeleteShader(vertexShader);
        return false;
    }

    // Fragment shader
    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
        LUX_ERROR("Fragment shader compilation failed:\n%s", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    // Link
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);
        LUX_ERROR("Shader linking failed:\n%s", infoLog);
        glDeleteProgram(m_id);
        m_id = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    LUX_INFO("Shader compiled successfully (ID: %u)", m_id);
    return m_id != 0;
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LUX_ERROR("Failed to open shader file: %s", path.c_str());
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::shared_ptr<Shader> Shader::fromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    return std::make_shared<Shader>(vertexPath, fragmentPath);
}

std::shared_ptr<Shader> Shader::fromSource(const std::string& vertexSrc, const std::string& fragmentSrc) {
    auto shader = std::make_shared<Shader>();
    shader->compile(vertexSrc, fragmentSrc);
    return shader;
}

} // namespace lux