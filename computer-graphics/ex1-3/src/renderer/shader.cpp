#include "./renderer.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    ID = Functional::createShaderProgram(vertexPath, fragmentPath);
}

Shader::Shader() {
    ID = 0;
}

void Shader::use() {
    if (ID == 0) {
        spdlog::error("Shader Program Not Loaded.");
        exit(-1);
    }

    glUseProgram(ID);
}

unsigned int Shader::getID() {
    return ID;
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string& name, const float* value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec2(const std::string& name, const float* value) {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}