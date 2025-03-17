#include "./compute_shader.h"

unsigned int createComputeProgram(const std::string& shaderPath) {
    unsigned int shader = Functional::compileShader(shaderPath, GL_COMPUTE_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        spdlog::error("Shader Program Linking Failed: \n{}", infoLog);
        exit(-1);
    }
    
    glDeleteShader(shader);

    return shaderProgram;
}

ComputeShader::ComputeShader(const std::string& shaderPath) {
    ID = createComputeProgram(shaderPath);
    threads[0] = threads[1] = threads[2] = 1;

    memset(buffers, 0, sizeof(buffers));
}

void ComputeShader::use() {
    glUseProgram(ID);
}

unsigned int ComputeShader::getID() {
    return ID;
}

void ComputeShader::launch(int x, int y, int z) {

    auto X = (x + threads[0] - 1) / threads[0];
    auto Y = (y + threads[1] - 1) / threads[1];
    auto Z = (z + threads[2] - 1) / threads[2];

    glUseProgram(ID);
    glDispatchCompute(X, Y, Z);

    if (int error = glGetError(); error != GL_NO_ERROR) {
        spdlog::error("Compute Shader Error: {}", error);
        exit(-1);
    }
}

void ComputeShader::join() {
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void ComputeShader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setMat4(const std::string& name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void ComputeShader::setVec3(const std::string& name, const float* value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void ComputeShader::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ComputeShader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec2(const std::string& name, const float* value) {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void ComputeShader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeShader::setUint(const std::string& name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}