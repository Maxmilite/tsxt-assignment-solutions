#include "./objects.h"
#include <set>

Model::Model(const std::string& path, float size = 1.f) {
    loadModel(path, size);
}

void Model::draw(Shader& shader) {

    for (int i = 0; i < (int) meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(const std::string& path, float size = 1.f) {

    tinyobj::attrib_t inattrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    directory = path.substr(0, path.find_last_of('/')) + "/";

    const std::string filename = path.substr(path.find_last_of('/') + 1);

    spdlog::info("Loading Model: {} | Directory: {}", filename, directory);

    std::string err;
    bool ret = tinyobj::LoadObj(&inattrib, &shapes, &materials, &err, path.c_str(), directory.c_str(), true);

    if (!err.empty()) {
        spdlog::error("TinyObjLoader Error: {}", err);
        exit(-1);
    }

    if (!ret) {
        spdlog::error("Failed to load model: {}", path);
        exit(-1);
    }


    spdlog::debug("# of vertices  = {}", (int) (inattrib.vertices.size()) / 3);
    spdlog::debug("# of faces     = {}", (int) (shapes[0].mesh.indices.size()) / 3);
    spdlog::debug("# of normals   = {}", (int) (inattrib.normals.size()) / 3);
    spdlog::debug("# of texcoords = {}", (int) (inattrib.texcoords.size()) / 2);
    spdlog::debug("# of materials = {}", (int) materials.size());
    spdlog::debug("# of shapes    = {}", (int) shapes.size());
    
    for (size_t i = 0; i < materials.size(); i++) {
        spdlog::debug("material[{}].diffuse_texname = {}", int(i),
            materials[i].diffuse_texname.c_str());
    }

    // normalize vertices to -1 ~ 1
    float max_norm = 1e-8f;
    for (auto&& vertex : inattrib.vertices) {
        max_norm = std::max({
            max_norm, 
            vertex
        });
    }

    // vertices & indices
    for (int i = 0; i < (int) shapes.size(); i++) {

        spdlog::debug("Loading Mesh: {}", i);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        spdlog::debug("Indices: {}", shapes[i].mesh.indices.size());

        for (int j = 0; j < (int) shapes[i].mesh.indices.size(); j++) {
            tinyobj::index_t index = shapes[i].mesh.indices[j];

            Vertex vertex;
            vertex.position = {
                inattrib.vertices[3 * index.vertex_index + 0] / max_norm * size,
                inattrib.vertices[3 * index.vertex_index + 1] / max_norm * size,
                inattrib.vertices[3 * index.vertex_index + 2] / max_norm * size
            };
            if (index.normal_index >= 0) {
                vertex.normal = {
                    inattrib.normals[3 * index.normal_index + 0],
                    inattrib.normals[3 * index.normal_index + 1],
                    inattrib.normals[3 * index.normal_index + 2]
                };
            } else {
                vertex.normal = {0.0f, 0.0f, 0.0f};
            }
            if (index.texcoord_index >= 0) {
                vertex.texCoords = {
                    inattrib.texcoords[2 * index.texcoord_index + 0],
                    inattrib.texcoords[2 * index.texcoord_index + 1]
                };
            } else {
                vertex.texCoords = {0.0f, 0.0f};
            }
            vertices.push_back(vertex);
            indices.push_back(j);
        }

        spdlog::debug("Vertices: {}", vertices.size());

        if (inattrib.normals.size() == 0) {
            spdlog::debug("Calculating Normals...");
            Functional::calcVerticesNormals(vertices, indices);
        }

        if (shapes[i].mesh.material_ids.size() > 0) {
            std::set<int> material_ids = std::set<int>(shapes[i].mesh.material_ids.begin(), shapes[i].mesh.material_ids.end());
            if (material_ids.size() > 1) {
                spdlog::warn("Multiple Materials in Mesh: {}", i);
            }
            for (auto material_id : material_ids) {
                spdlog::debug("Material: {}", material_id);
                auto mp = &materials[material_id];

                // diffuse
                if (mp->diffuse_texname.length()) {
                    std::string texname = directory + mp->diffuse_texname;
                    spdlog::debug("Diffuse Texture: {}", texname);
                    {
                        bool skip = false;
                        for (int j = 0; j < (int) textures_loaded.size(); j++) {
                            if (std::strcmp(textures_loaded[j].getPath().c_str(), texname.c_str()) == 0) {
                                textures.push_back(textures_loaded[j]);
                                skip = true;
                                break;
                            }
                        }
                        if (skip) {
                            continue;
                        }
                    }
                    Texture texture(texname.c_str());
                    texture.setType("texture_diffuse");
                    textures_loaded.push_back(texture);
                    textures.push_back(texture);
                }

                // specular
                if (mp->specular_texname.length()) {
                    std::string texname = directory + mp->specular_texname;
                    spdlog::debug("Specular Texture: {}", texname);
                    {
                        bool skip = false;
                        for (int j = 0; j < (int) textures_loaded.size(); j++) {
                            if (std::strcmp(textures_loaded[j].getPath().c_str(), texname.c_str()) == 0) {
                                textures.push_back(textures_loaded[j]);
                                skip = true;
                                break;
                            }
                        }
                        if (skip) {
                            continue;
                        }
                    }
                    Texture texture(texname.c_str());
                    texture.setType("texture_specular");
                    textures_loaded.push_back(texture);
                    textures.push_back(texture);
                }

                // normal
                if (mp->normal_texname.length()) {
                    std::string texname = directory + mp->normal_texname;
                    spdlog::debug("Normal Texture: {}", texname);
                    {
                        bool skip = false;
                        for (int j = 0; j < (int) textures_loaded.size(); j++) {
                            if (std::strcmp(textures_loaded[j].getPath().c_str(), texname.c_str()) == 0) {
                                textures.push_back(textures_loaded[j]);
                                skip = true;
                                break;
                            }
                        }
                        if (skip) {
                            continue;
                        }
                    }
                    Texture texture(texname.c_str());
                    texture.setType("texture_normal");
                    textures_loaded.push_back(texture);
                    textures.push_back(texture);
                }
               
            }
        }

        meshes.push_back(Mesh(vertices, indices, textures));

        spdlog::debug("Loaded Mesh: {}", i);
    }

}