#pragma once

#include "lux/Core.h"
#include <string>
#include <glad/gl.h>

namespace lux {

class Texture {
public:
    Texture() = default;
    Texture(const std::string& path);
    Texture(u32 width, u32 height, const void* data = nullptr, i32 format = GL_RGBA);
    ~Texture();

    void bind(u32 slot = 0) const;
    void unbind() const;

    u32 getID() const { return m_id; }
    i32 getWidth() const { return m_width; }
    i32 getHeight() const { return m_height; }

    static std::shared_ptr<Texture> fromFile(const std::string& path);
    static std::shared_ptr<Texture> createWhite();

private:
    u32 m_id = 0;
    i32 m_width = 0;
    i32 m_height = 0;
    i32 m_channels = 0;
};

} // namespace lux