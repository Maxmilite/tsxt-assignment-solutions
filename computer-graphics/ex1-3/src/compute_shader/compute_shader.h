#ifndef COMPUTE_SHADER_H__
#define COMPUTE_SHADER_H__

class ComputeShader;

#include "../renderer/renderer.h"

const int MAX_BUFFER_NUM = 16;

class ComputeShader {
public:
    ComputeShader(const std::string& shaderPath);
    ComputeShader();
    void use();
    unsigned int getID();
    void launch(int x = 1, int y = 1, int z = 1);
    void join();

    void setThreads(int x = 256, int y = 1, int z = 1) {
        threads[0] = x;
        threads[1] = y;
        threads[2] = z;
    }
    std::vector<int> getThreads() {
        return {threads[0], threads[1], threads[2]};
    }

    template<typename T>
    void setBuffer(unsigned int index, std::vector<T>& data) {
        if (index >= MAX_BUFFER_NUM) {
            throw std::runtime_error("Buffer index out of bounds");
        }

        if (buffers[index] != 0) {
            glDeleteBuffers(1, &buffers[index]);
        }

        GLuint ssbo;
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);

        buffers[index] = ssbo;
    }



    template<typename T>
    std::vector<T> getBuffer(unsigned int index) {
        if (index >= MAX_BUFFER_NUM || buffers[index] == 0) {
            throw std::runtime_error("Buffer index is not bound or out of bounds");
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[index]);
        GLint bufferSize;
        glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &bufferSize);

        std::vector<T> data(bufferSize / sizeof(T));
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, bufferSize, data.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        return data;
    }


    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setUint(const std::string& name, unsigned int value);
    void setMat4(const std::string& name, const float* value);
    void setVec3(const std::string& name, const float* value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec2(const std::string& name, const float* value);
    void setVec2(const std::string& name, float x, float y);

private:
    unsigned int ID;
    int threads[3];
    unsigned int buffers[16];
};

#endif