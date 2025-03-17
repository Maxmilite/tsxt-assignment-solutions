#include "./renderer.h"

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

Renderer::Renderer() {}

Renderer::~Renderer() {
    terminate();
}

void Renderer::resizeWindow(int width, int height) {
    this->width = width;
    this->height = height;
}

void Renderer::init() {

    auto& globalConfig = getGlobalConfig();
    this->width = globalConfig.get<int>("window.width", 1280);
    this->height = globalConfig.get<int>("window.height", 720);
    this->fpsLimit = globalConfig.get<int>("window.fpsLimit", INT_MAX);
    
    spdlog::info("Renderer Config: {}x{} | FPS Limit: {}", width, height, fpsLimit == INT_MAX ? "Unlimited" : std::to_string(fpsLimit));


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Renderer", nullptr, nullptr);
	if (!window) {
        spdlog::error("Failed to create GLFW window.");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
		exit(-1);
	}

    glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, Functional::framebufferSizeCallback);

    spdlog::info("Initialized GLFW Window.");

    GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // config
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (bool vSync = globalConfig.get<bool>("window.vsync", false); vSync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    // ray tracing
    rayTracer = std::make_unique<RayTracer>();

    loadShader();
    setModel();

    // rendering mode
    renderingMode = RenderingMode::RASTERIZATION;
    
    // light
    light = Light();

    // transformation
    motion.reset();

    // option
    setFocus(false);
    setLighting(true);

    // GUI
    GUI::init(window);

    spdlog::info("Renderer Initialized.");

}

void Renderer::setModel(Model& model) {
    // model
    this->model = std::make_unique<Model>(model);
}

void Renderer::setModel() {
    
    auto& globalConfig = getGlobalConfig();

    this->model = std::make_unique<Model>(Model(
		globalConfig.get<std::string>("model.path", "./assets/models/spot/spot_triangulated.obj"),
		globalConfig.get<float>("model.size", 1.f)
	));

    spdlog::info("Model Loaded: {}", globalConfig.get<std::string>("model.path", "./assets/models/spot/spot_triangulated.obj"));
}

void Renderer::loadShader() {
    
    // rasterization
    shaders.push_back(Shader("./assets/shaders/rasterization/object.vert",      "./assets/shaders/rasterization/object.frag"));
    shaders.push_back(Shader("./assets/shaders/rasterization/axis.vert",        "./assets/shaders/rasterization/axis.frag"));
    shaders.push_back(Shader("./assets/shaders/rasterization/lighting.vert",    "./assets/shaders/rasterization/lighting.frag"));

    // normal
    shaders.push_back(Shader("./assets/shaders/normal/object.vert",             "./assets/shaders/normal/object.frag"));

    // depth
    shaders.push_back(Shader("./assets/shaders/depth/object.vert",              "./assets/shaders/depth/object.frag"));
}

bool Renderer::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Renderer::processInput(float deltaTime) {
    static bool isFocusKeyPressed = false;

    bool moved = false;
        
    // keyboard input
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
        isFocusKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE) {
        if (isFocusKeyPressed) {
            setFocus(!focus);
        }
        isFocusKeyPressed = false;
    }

    if (focus) {
        float speed = camera.getSpeed();
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.setSpeed(2 * speed);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.moveUp(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.moveDown(deltaTime);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera.rotateLeft(deltaTime * 300);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera.rotateRight(deltaTime * 300);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera.rotateUp(deltaTime * 300);
            moved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.rotateDown(deltaTime * 300);
            moved = true;
        }

        // acceleration: reset
        camera.setSpeed(speed);
    }

    // mouse input

    static double lastX = 0., lastY = 0.;
    static bool firstMouse = true;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if ((xoffset != 0 || yoffset != 0) && focus) {
        moved = true;
    }

    camera.rotateRight(focus ? xoffset : 0.f);
    camera.rotateUp(focus ? yoffset : 0.f);

    if (moved) {
        rayTracer->rerender();
    }
}

void Renderer::render() {

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    float currentFrame = glfwGetTime();
    if (currentFrame - lastFrame < 1.f / fpsLimit) {
        return;
    }

    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwSetWindowTitle(window, fmt::format("Renderer | FPS: {:.2f}", 1.f / deltaTime).c_str());

    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        resizeWindow(width, height);
    }

    if (model == nullptr) {
        spdlog::error("Model is not set.");
        exit(-1);
    }
    
    processInput(deltaTime);

    if (isAnimationPlaying) {
        animation.update(deltaTime);
    }

    glClearColor(1e-3f, 1e-3f, 1e-3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (renderingMode == RenderingMode::RASTERIZATION) {
        handleRasterization();
    } else if (renderingMode == RenderingMode::RAYTRACING) {
        handleRaytracing();
    } else if (renderingMode == RenderingMode::NORMAL) {
        handleNormal();
    } else if (renderingMode == RenderingMode::DEPTH) {
        handleDepth();
    }

    // error check
    {
        int code = glGetError();
        if (code != GL_NO_ERROR) {
            spdlog::error("OpenGL Error: {}", code);
            exit(-1);
        }
    }

    glfwPollEvents();

    GUI::render();

    glfwSwapBuffers(window);


}

void Renderer::handleRasterization() {
    // Object
    {
        auto& shader = shaders[0];
        shader.use();
        shader.setVec3("viewPos", glm::value_ptr(camera.getPosition()));
        
        // lighting
        shader.setVec3("light.position", glm::value_ptr(light.getPosition()));
        shader.setVec3("light.ambient", glm::value_ptr(light.getAmbient()));
        shader.setVec3("light.diffuse", glm::value_ptr(light.getDiffuse()));
        shader.setVec3("light.specular", glm::value_ptr(light.getSpecular()));

        shader.setFloat("material0.shininess", 32.0f);

        // camera
        glm::mat4 modelMatrix = getModelMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        shader.setMat4("model", glm::value_ptr(modelMatrix));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        model->draw(shaders[0]);

    }

    // Axis
    {

        auto& shader = shaders[1];

        shader.use();

        float axisVertices[] = {
            // X-axis
            0.0f, 0.0f, 0.0f,
            2.0f, 0.0f, 0.0f,

            // Y-axis
            0.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 0.0f,

            // Z-axis
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f 
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        shader.setMat4("model", glm::value_ptr(modelMatrix));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        glBindVertexArray(VAO);

        shader.setVec3("axisColor", 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_LINES, 0, 2);
        shader.setVec3("axisColor", 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_LINES, 2, 2);
        shader.setVec3("axisColor", 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 4, 2);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // Lighting Sphere
    if (showLighting) {

        auto& shader = shaders[2];

        shader.use();

        float lightVertex[] = {light.getPosition().x, light.getPosition().y, light.getPosition().z};

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertex), lightVertex, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        const float lightRadius = 200.f;

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        shader.setVec3("lightPosition", glm::value_ptr(light.getPosition()));
        shader.setFloat("lightRadius", lightRadius);
        shader.setVec3("lightColor", glm::value_ptr(light.getDiffuse()));
        shader.setVec2("screenSize", width, height);

        glBindVertexArray(VAO);
        glPointSize(lightRadius * 2);
        glDrawArrays(GL_POINTS, 0, 1);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void Renderer::handleRaytracing() {
    rayTracer->render();
}

void Renderer::handleNormal() {
    
    auto& shader = shaders[3];

    shader.use();

    // camera
    glm::mat4 modelMatrix = getModelMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    shader.setMat4("model", glm::value_ptr(modelMatrix));
    shader.setMat4("view", glm::value_ptr(view));
    shader.setMat4("projection", glm::value_ptr(projection));

    model->draw(shader);
}

void Renderer::handleDepth() {
    auto& shader = shaders[4];

    shader.use();

    // camera

    glm::mat4 modelMatrix = getModelMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    shader.setMat4("model", glm::value_ptr(modelMatrix));
    shader.setMat4("view", glm::value_ptr(view));
    shader.setMat4("projection", glm::value_ptr(projection));

    model->draw(shader);
}

std::pair<glm::vec3, glm::vec3> Renderer::getRay() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float x = (2.0f * xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * ypos) / height;
    float z = 1.0f;

    glm::vec3 ray_nds = glm::vec3(x, y, z);
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

    glm::mat4 projection = camera.getProjectionMatrix();
    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

    glm::mat4 view = camera.getViewMatrix();
    glm::vec4 ray_wor = glm::inverse(view) * ray_eye;
    ray_wor = glm::normalize(ray_wor);

    glm::vec3 rayDir = glm::vec3(ray_wor.x, ray_wor.y, ray_wor.z);
    glm::vec3 rayOrigin = camera.getPosition();

    return {rayOrigin, rayDir};
}

glm::vec3 Renderer::getMouseModelIntersection() {
    auto [rayOrigin, rayDir] = getRay();

    float minDistance = std::numeric_limits<float>::max();
    glm::vec3 intersection = glm::vec3(-1.f);

    const auto near = camera.getNear();

    auto modelMatrix = getModelMatrix();

    for (auto& mesh : model->getMeshes()) {

        auto& vertices = mesh.vertices;
        auto& indices = mesh.indices;

        for (int i = 0; i < (int) indices.size(); i += 3) {
            glm::vec3 v1 = vertices[indices[i]].position;
            glm::vec3 v2 = vertices[indices[i + 1]].position;
            glm::vec3 v3 = vertices[indices[i + 2]].position;

            v1 = glm::vec3(modelMatrix * glm::vec4(v1, 1.0f));
            v2 = glm::vec3(modelMatrix * glm::vec4(v2, 1.0f));
            v3 = glm::vec3(modelMatrix * glm::vec4(v3, 1.0f));

            float t = Functional::rayTriangleIntersect(rayOrigin, rayDir, v1, v2, v3);

            if (t < near) {
                continue;
            }

            glm::vec3 p = rayOrigin + t * rayDir;

            if (glm::distance(p, rayOrigin) < minDistance) {
                minDistance = glm::distance(p, rayOrigin);
                intersection = p;
            }
        }
    }

    return intersection;
}

void Renderer::handleInteractiveTransformation(int type) {
    static bool isMousePressed = false;
    static glm::vec3 lastIntersection = glm::vec3(-1.f);

    static glm::vec2 lastMousePos = glm::vec2(0.f);

    if (focus) {
        return;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        isMousePressed = false;
        lastIntersection = glm::vec3(-1.f);
        lastMousePos = glm::vec2(0.f);
        return;
    }

    if (!isMousePressed) {
        lastIntersection = getMouseModelIntersection();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastMousePos = glm::vec2(xpos, ypos);
        isMousePressed = true;
    }

    if (lastIntersection == glm::vec3(-1.f)) {
        return;
    }

    auto [rayOrigin, rayDir] = getRay();

    // to move the intersection point to the ray

    float t = glm::distance(lastIntersection, rayOrigin) / glm::distance(rayOrigin, rayOrigin + rayDir);

    glm::vec3 intersection = rayOrigin + t * rayDir;


    switch (type) {
        case 1: {
            glm::vec3 diff = intersection - lastIntersection;
            motion.translation += diff;
            break;
        }
        case 2: {

            glm::vec2 currentMousePos = glm::vec2(0.f);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            currentMousePos = glm::vec2(xpos, ypos);

            glm::vec2 diff = currentMousePos - lastMousePos;

            auto right = camera.getRight();
            auto up = camera.getUp();
            
            glm::quat currentRotation = motion.rotation;

            glm::quat rightRotation = glm::angleAxis(glm::radians(diff.y), right);
            glm::quat upRotation = glm::angleAxis(glm::radians(diff.x), up);

            glm::quat newRotation = upRotation * rightRotation * currentRotation;

            motion.rotation = newRotation;

            lastMousePos = currentMousePos;

            break;
        }
        case 3: {

            glm::vec3 diff = intersection - lastIntersection;

            motion.scale += diff;

            break;
        }
    }

    lastIntersection = intersection;

}

void Renderer::terminate() {
    GUI::shutdown();
    glfwTerminate();
}

Renderer& getRenderer() {
    static Renderer renderer;
    return renderer;
}