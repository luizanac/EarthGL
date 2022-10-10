#pragma once

#include "../EarthGL.hpp"
#include "Texture.hpp"

class Shader {
public:
    unsigned int m_ProgramId;

    Shader(const char *vertexPath, const char *fragmentPath);

    void Use() const;

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;

    void SetMat4(const std::string &name, glm::mat4 value) const;

    void SetVec4(const std::string &name, glm::vec4 value) const;

    void SetTexture(const char *uName, const Texture &texture);

    ~Shader() {
        glDeleteProgram(m_ProgramId);
    }

private:
    static void CheckCompileErrors(unsigned int shader, const std::string &type);

};


