#pragma once

#include "../EarthGL.hpp"

class Texture {
private:
    unsigned int m_Id{};
    int m_Width{};
    int m_Height{};
    int m_NrChannels{};
    GLenum m_Index{};
public:
    Texture(const char *fileName, GLenum channelFormat);

    void ActivateAndBind(GLenum texIndex);

    [[nodiscard]] unsigned int GetId() const;

    [[nodiscard]] int GetWidth() const;

    [[nodiscard]] int GetHeight() const;

    [[nodiscard]] int GetNrChannels() const;

    [[nodiscard]] int GetIndex() const;

    [[nodiscard]]static unsigned int LoadCubemap(std::vector<std::string> faces);
};