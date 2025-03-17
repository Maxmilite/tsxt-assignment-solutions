#include "./renderer.h"

#include <fstream>

namespace Functional {
    void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	    glViewport(0, 0, width, height);
    }

    unsigned int compileShader(const std::string& path, GLenum type) {
        std::ifstream file(path);
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char* src = source.c_str();

        spdlog::debug("Compiling Shader: {}", path);

        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            spdlog::error("Shader Compilation Failed: \n{}", infoLog);
            exit(-1);
        }

        return shader;
    }

    unsigned int createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
        unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            spdlog::error("Shader Program Linking Failed: \n{}", infoLog);
            exit(-1);
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
        glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);
        return glm::normalize(normal);
    }

    void calcVerticesNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
        std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));
        for (int i = 0; i < (int) indices.size(); i += 3) {
            glm::vec3 v1 = vertices[indices[i]].position;
            glm::vec3 v2 = vertices[indices[i + 1]].position;
            glm::vec3 v3 = vertices[indices[i + 2]].position;
            glm::vec3 normal = calcNormal(v1, v2, v3);
            normals[indices[i]] += normal;
            normals[indices[i + 1]] += normal;
            normals[indices[i + 2]] += normal;
        }
        for (int i = 0; i < (int) normals.size(); i++) {
            normals[i] = glm::normalize(normals[i]);
        }
        for (int i = 0; i < (int) vertices.size(); i++) {
            vertices[i].normal = normals[i];
        }
    }

    float rayTriangleIntersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
        glm::vec3 edge1, edge2, tvec, pvec, qvec;
        float det, inv_det, u, v, t;

        edge1 = v1 - v0;
        edge2 = v2 - v0;

        pvec = glm::cross(dir, edge2);
        det = glm::dot(edge1, pvec);

        if (det < 0.0001f) {
            return 0;
        }

        tvec = orig - v0;
        u = glm::dot(tvec, pvec);
        if (u < 0.0f || u > det) {
            return 0;
        }

        qvec = glm::cross(tvec, edge1);
        v = glm::dot(dir, qvec);
        if (v < 0.0f || u + v > det) {
            return 0;
        }

        t = glm::dot(edge2, qvec);
        inv_det = 1.0f / det;
        t *= inv_det;
        return t;
    }
};