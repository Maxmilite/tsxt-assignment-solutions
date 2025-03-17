#include "./ray_tracer.h"

namespace RT {

void RayTracer::setupScreenRendering() {
    float quadVertices[] = {
        // Positions    // TexCoords
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f
    };

    glGenVertexArrays(1, &screenQuadVAO);
    glGenBuffers(1, &screenQuadVBO);
    glBindVertexArray(screenQuadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

    screenShader = std::make_unique<Shader>("./assets/shaders/ray_tracing/final.vert", "./assets/shaders/ray_tracing/final.frag");
}

void RayTracer::setupBuffers() {

    auto& spheres = scene.spheres;
    auto& walls = scene.walls;

    glGenBuffers(1, &spheresSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spheresSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Scene::Sphere) * spheres.size(), spheres.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spheresSSBO);

    glGenBuffers(1, &wallsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, wallsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Scene::Wall) * walls.size(), walls.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, wallsSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RayTracer::init() {
    traceShader = std::make_unique<ComputeShader>("./assets/shaders/ray_tracing/trace.comp");

    traceShader->setThreads(16, 16, 1);

    scene = Scene();
    setupBuffers();
    setupScreenRendering();

    outputTexture = 0;

    maxBounces = 5;
    spp = 4;

    shouldRerender = false;
   
}

void RayTracer::render() {

    auto& renderer = getRenderer();

    auto [width, height] = renderer.getWindowSize();
    if (width != this->width || height != this->height) {
        spdlog::info("Ray Tracing: Window Resized: {}x{}", width, height);
        resize(width, height);
    }

    static unsigned int frame = 0;

    traceShader->use();

    {
        traceShader->setInt("width", width);
        traceShader->setInt("height", height);
        traceShader->setInt("maxBounces", maxBounces);
        traceShader->setVec3("lightPos", scene.lightPos);
        traceShader->setVec3("lightRadiance", scene.lightRadiance);
        traceShader->setFloat("lightRadius", scene.lightRadius);

        if (shouldRerender) {
            traceShader->setInt("rerender", 1);
            shouldRerender = false;
            frame = 0;
        } else {
            traceShader->setInt("rerender", 0);
        }

        traceShader->setUint("frame", ++frame);
        traceShader->setInt("spp", spp);

    }

    {
        glm::mat4 modelMatrix = renderer.getModelMatrix();
        glm::mat4 viewMatrix = renderer.getCamera().getViewMatrix();
        // glm::mat4 projectionMatrix = renderer.getCamera().getProjectionMatrix();

        // auto inverseMatrix = glm::inverse(modelMatrix * viewMatrix);
        auto inverseMatrix = glm::mat4(1.f);
        
        auto cameraPos = renderer.getCamera().getPosition();
        cameraPos = glm::vec3(inverseMatrix * (glm::vec4(cameraPos, 1.0f)));

        auto cameraUp = renderer.getCamera().getUp();
        cameraUp = glm::vec3(inverseMatrix * (glm::vec4(cameraUp, 1.0f)));

        auto cameraRight = renderer.getCamera().getRight();
        cameraRight = glm::vec3(inverseMatrix * (glm::vec4(cameraRight, 1.0f)));

        auto cameraFront = renderer.getCamera().getFront();
        cameraFront = glm::vec3(inverseMatrix * (glm::vec4(cameraFront, 1.0f)));
    
        traceShader->setVec3("cameraUp", cameraUp);
        traceShader->setVec3("cameraRight", cameraRight);
        traceShader->setVec3("cameraFront", cameraFront);
        traceShader->setVec3("cameraPos", cameraPos);
        traceShader->setFloat("cameraFov", glm::radians(renderer.getCamera().getFov()));
    }

    {
        traceShader->setInt("numSpheres", scene.spheres.size());
        traceShader->setInt("numWalls", scene.walls.size());
    }

    traceShader->launch(width, height, 1);
    traceShader->join();

    screenRender();

}

void RayTracer::screenRender() {
    auto& renderer = getRenderer();
    auto [width, height] = renderer.getWindowSize();

    screenShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    screenShader->setInt("screenTex", 0);

    glBindVertexArray(screenQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


}

void RayTracer::terminate() {
    glDeleteBuffers(1, &spheresSSBO);
    glDeleteBuffers(1, &wallsSSBO);
    glDeleteBuffers(1, &screenQuadVBO);
    glDeleteVertexArrays(1, &screenQuadVAO);
    glDeleteTextures(1, &outputTexture);
}

void RayTracer::resize(int width, int height) {
    this->width = width;
    this->height = height;
    image = std::vector<glm::vec4>(width * height, glm::vec4(0.9f));

    shouldRerender = true;

    if (outputTexture) {
        glDeleteTextures(1, &outputTexture);
    }

    glGenTextures(1, &outputTexture);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    glBindImageTexture(0, outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, image.data());
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glBindImageTexture(0, outputTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

}

RayTracer::RayTracer() {
    init();
}

}