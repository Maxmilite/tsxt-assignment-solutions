#ifndef OBJECTS_H__
#define OBJECTS_H__

class Texture;
class Vertex;
class Mesh;
class Model;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <tiny_obj_loader.h>

#include "../renderer/renderer.h"

class Model {
public:
    Model(const std::string& path, float size);
    Model() = default;

    void draw(Shader& shader);

    std::vector<Mesh>& getMeshes() { return meshes; }

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    void loadModel(const std::string& path, float size);
};

class Texture {
public:
    Texture(const std::string& path);
    Texture() = default;

    void bind();
    void unbind();

    std::string getType() { return type; }
    void setType(const std::string& t) { type = t; }

    unsigned int getID() { return m_id; }
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    
    std::string getPath() { return path; }

    glm::vec3 getColor(float u, float v);

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
    unsigned char* m_data;
    std::string type;
    std::string path;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(const std::string& path);
    
    void draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;
    void setup();
};

#endif