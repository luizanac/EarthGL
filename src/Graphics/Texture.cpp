#include "Texture.hpp"
#include "stb_image.h"

#define SHADER_RESOURCE_PATH "Resources/Textures/"

Texture::Texture(const char *fileName, GLenum channelFormat) {
//    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned char *buffer = stbi_load(
            fmt::format("{}{}", SHADER_RESOURCE_PATH, fileName).c_str(),
            &m_Width, &m_Height,
            &m_NrChannels, 0);

    if (buffer) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, channelFormat, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << fmt::format("Failed to load texture: {}", fileName) << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(buffer);
}

void Texture::ActivateAndBind(GLenum texIndex) {
    m_Index = texIndex;
    glActiveTexture(m_Index);
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

unsigned int Texture::GetId() const {
    return m_Id;
}

int Texture::GetWidth() const {
    return m_Width;
}

int Texture::GetHeight() const {
    return m_Height;
}

int Texture::GetNrChannels() const {
    return m_NrChannels;
}

int Texture::GetIndex() const {

    switch (m_Index) {
        case GL_TEXTURE0:
            return 0;
        case GL_TEXTURE1:
            return 1;
        case GL_TEXTURE2:
            return 2;
        case GL_TEXTURE3:
            return 3;
        case GL_TEXTURE4:
            return 4;
        case GL_TEXTURE5:
            return 5;
        case GL_TEXTURE6:
            return 6;
    }

    return -1;
}

unsigned int Texture::LoadCubemap(std::vector<std::string> faces) {
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
            } else {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            }
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return textureId;
    }
}

