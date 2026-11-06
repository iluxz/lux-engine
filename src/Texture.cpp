#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "lux/Texture.h"
#include <glad/gl.h>

namespace lux {

Texture::Texture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (!data) {
        LUX_ERROR("Failed to load texture: %s", path.c_str());
        return;
    }

    GLenum format = GL_RGB;
    if (m_channels == 1) format = GL_RED;
    else if (m_channels == 3) format = GL_RGB;
    else if (m_channels == 4) format = GL_RGBA;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    LUX_INFO("Texture loaded: %s (%dx%d, %d channels)", path.c_str(), m_width, m_height, m_channels);
}

Texture::Texture(u32 width, u32 height, const void* data, i32 format) {
    m_width = (i32)width;
    m_height = (i32)height;
    m_channels = 4;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
    if (m_id) glDeleteTextures(1, &m_id);
}

void Texture::bind(u32 slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture> Texture::fromFile(const std::string& path) {
    return std::make_shared<Texture>(path);
}

std::shared_ptr<Texture> Texture::createWhite() {
    u8 white[] = {255, 255, 255, 255};
    return std::make_shared<Texture>(1, 1, white, GL_RGBA);
}

} // namespace lux