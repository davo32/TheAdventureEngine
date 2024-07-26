#include "gl_core_4_4.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace aie {

    Texture::Texture()
        : m_filename("none"),
        m_width(0),
        m_height(0),
        m_glHandle(0),
        m_format(GL_RGBA), // Default format
        m_loadedPixels(nullptr) {
    }

    Texture::Texture(const char* filename)
        : m_filename("none"),
        m_width(0),
        m_height(0),
        m_glHandle(0),
        m_format(GL_RGBA), // Default format
        m_loadedPixels(nullptr) {
        load(filename);
    }

    Texture::Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels)
        : m_filename("none"),
        m_width(width),
        m_height(height),
        m_format(format),
        m_loadedPixels(nullptr) {
        create(width, height, format, pixels);
    }

    Texture::~Texture() {
        if (m_glHandle != 0) {
            glDeleteTextures(1, &m_glHandle);
        }
        if (m_loadedPixels != nullptr) {
            stbi_image_free(m_loadedPixels);
        }
    }

    bool Texture::load(const char* filename) {
        // Clean up existing texture
        if (m_glHandle != 0) {
            glDeleteTextures(1, &m_glHandle);
            m_glHandle = 0;
        }

        int x = 0, y = 0, comp = 0;
        m_loadedPixels = stbi_load(filename, &x, &y, &comp, STBI_default);

        if (m_loadedPixels != nullptr) {
            glGenTextures(1, &m_glHandle);
            glBindTexture(GL_TEXTURE_2D, m_glHandle);

            GLenum internalFormat = GL_RGBA;
            GLenum format = GL_RGBA;

            switch (comp) {
            case STBI_grey:
                internalFormat = GL_RED;
                format = GL_RED;
                break;
            case STBI_grey_alpha:
                internalFormat = GL_RG;
                format = GL_RG;
                break;
            case STBI_rgb:
                internalFormat = GL_RGB;
                format = GL_RGB;
                break;
            case STBI_rgb_alpha:
                internalFormat = GL_RGBA;
                format = GL_RGBA;
                break;
            default:
                std::cerr << "Unsupported image format." << std::endl;
                stbi_image_free(m_loadedPixels);
                m_loadedPixels = nullptr;
                return false;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, format, GL_UNSIGNED_BYTE, m_loadedPixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);
            m_width = static_cast<unsigned int>(x);
            m_height = static_cast<unsigned int>(y);
            m_filename = filename;

            return true;
        }
        else {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return false;
        }
    }

    void Texture::create(unsigned int width, unsigned int height, Format format, unsigned char* pixels) {
        if (m_glHandle != 0) {
            glDeleteTextures(1, &m_glHandle);
            m_glHandle = 0;
        }

        m_width = width;
        m_height = height;
        m_format = format;

        glGenTextures(1, &m_glHandle);
        glBindTexture(GL_TEXTURE_2D, m_glHandle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum internalFormat = GL_RGBA;
        GLenum formatEnum = GL_RGBA;

        switch (m_format) {
        case RED:
            internalFormat = GL_RED;
            formatEnum = GL_RED;
            break;
        case RG:
            internalFormat = GL_RG;
            formatEnum = GL_RG;
            break;
        case RGB:
            internalFormat = GL_RGB;
            formatEnum = GL_RGB;
            break;
        case RGBA:
        default:
            internalFormat = GL_RGBA;
            formatEnum = GL_RGBA;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, formatEnum, GL_UNSIGNED_BYTE, pixels);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_glHandle);
    }

} // namespace aie