#include "Shader.hpp"
#include <fstream>
#include <sstream>

#define SHADER_RESOURCE_PATH "Resources/Shaders/"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(fmt::format("{}{}", SHADER_RESOURCE_PATH, vertexPath));
        fShaderFile.open(fmt::format("{}{}", SHADER_RESOURCE_PATH, fragmentPath));
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &ex) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    uint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    m_ProgramId = glCreateProgram();
    glAttachShader(m_ProgramId, vertex);
    glAttachShader(m_ProgramId, fragment);
    glLinkProgram(m_ProgramId);
    CheckCompileErrors(m_ProgramId, "PROGRAM");

    glDetachShader(m_ProgramId, vertex);
    glDetachShader(m_ProgramId, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const {
    glUseProgram(m_ProgramId);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ProgramId, name.c_str()), (int) value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ProgramId, name.c_str()), value);

}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ProgramId, name.c_str()), value);
}

void Shader::SetMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string &name, glm::vec4 value) const {
    glUniform4fv(glGetUniformLocation(m_ProgramId, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::SetTexture(const char *uName, const Texture &texture) {
    this->SetInt(uName, texture.GetIndex());
}
