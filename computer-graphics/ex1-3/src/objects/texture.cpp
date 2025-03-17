#include "./objects.h"

Texture::Texture(const std::string& path) {

    this->path = path;

    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!m_data) {
        spdlog::error("Failed to load texture: {}", path);
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (m_channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
    } else if (m_channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    } else {
        spdlog::error("Unknown number of channels: {}", m_channels);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(m_data);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_id == 0) {
        spdlog::error("Texture Not Loaded.");
        exit(-1);
    }

    // check if GL ERROR
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        spdlog::error("OpenGL Error: {}", err);
        exit(-1);
    }
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec3 Texture::getColor(float u, float v) {
    if (m_data == nullptr) {
        return glm::vec3(1.0f);
    }

    int x = u * m_width;
    int y = v * m_height;

    if (x < 0) x = 0;
    if (x >= m_width) x = m_width - 1;
    if (y < 0) y = 0;
    if (y >= m_height) y = m_height - 1;

    int index = (y * m_width + x) * m_channels;

    float r = m_data[index] / 255.0f;
    float g = m_data[index + 1] / 255.0f;
    float b = m_data[index + 2] / 255.0f;

    return glm::vec3(r, g, b);
}