#include "./ray_tracer.h"

namespace RT {

void Mesh::loadModel(std::unique_ptr<Model>& model) {

    spdlog::debug("Mesh::loadModel called.");
    triangles = std::vector<Triangle>();

    if (model == nullptr) {
        spdlog::error("Model is not set.");
        exit(-1);
    }

    if (model->getMeshes().empty()) {
        spdlog::error("Model has no mesh.");
        exit(-1);
    }
    spdlog::debug("Cleanup finished.");

    {
        int size = 0;
        for (auto& mesh : model->getMeshes()) {
            size += mesh.indices.size();
        }

    }

    for (auto& mesh : model->getMeshes()) {

        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            Triangle triangle;
            triangle.v0 = mesh.vertices[mesh.indices[i]].position;
            triangle.v1 = mesh.vertices[mesh.indices[i + 1]].position;
            triangle.v2 = mesh.vertices[mesh.indices[i + 2]].position;

            triangle.n0 = mesh.vertices[mesh.indices[i]].normal;
            triangle.n1 = mesh.vertices[mesh.indices[i + 1]].normal;
            triangle.n2 = mesh.vertices[mesh.indices[i + 2]].normal;

            // interpolate color by texcoords of 3 vertices
            {
                glm::vec2 tex0 = mesh.vertices[mesh.indices[i]].texCoords;
                glm::vec2 tex1 = mesh.vertices[mesh.indices[i + 1]].texCoords;
                glm::vec2 tex2 = mesh.vertices[mesh.indices[i + 2]].texCoords;

                glm::vec2 tex = (tex0 + tex1 + tex2) / 3.0f;

                // only use the first texture
                triangle.material.color = mesh.textures[0].getColor(tex.x, tex.y);
            }

            triangles.push_back(triangle);
            

        }
    }
    
}

}