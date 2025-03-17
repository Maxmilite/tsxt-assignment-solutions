#include "./gui.h"

namespace GUI {

void modelManagement();
void transformation();
void lighting();
void camera();
void renderingMode();
void rayTracing();

void animation();

std::map<std::string, std::string>& getGUIConfig() {
    static std::map<std::string, std::string> config;
    return config;
}

void init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

#ifdef _WIN32
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/msyh.ttc",
        18.0f,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull()
    );
    IM_ASSERT(font != nullptr);
#endif
}

void render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGuiIO& io = ImGui::GetIO(); (void) io;

        if (getRenderer().getFocus()) {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        }

        ImGui::Begin("Renderer");

        if (getRenderer().getFocus()) {
            ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f),
                "Focus mode is enabled.\n"
                "Mouse control to this window is disabled.\n"
            );
            ImGui::TextWrapped(
                "Press ` (or ~) to toggle focus mode.\n"
                "Press ESC to exit.\n"
                "Press W, A, S, D to move the camera.\n"
                "Press Left Ctrl to accelerate.\n"
                "Press Up, Down, Left, Right to rotate the camera.\n"
                "Press Space and Left Shift to move the camera up and down.\n"
                "Move the mouse to rotate the camera."
            );
            ImGui::Separator();
        } else {
            ImGui::TextWrapped(
                "Press ` (or ~) to toggle focus mode.\n"
                "Press ESC to exit.\n"
            );
            ImGui::Separator();
        }

        modelManagement();
        renderingMode();
        transformation();
        lighting();
        camera();
        rayTracing();

        static bool enableAnimation = false;
        ImGui::Checkbox("Show Animation Window", &enableAnimation);

        ImGui::End();

        if (enableAnimation) {
            ImGui::Begin("Animation");
            animation();
            ImGui::End();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void modelManagement() {

    auto& globalConfig = getGlobalConfig();

    if (ImGui::CollapsingHeader("Model Management")) {
        
        ImGui::Text("Object Path: ");
        ImGui::SameLine();
        static char modelPath[256];
        ImGui::InputText("##modelPath", modelPath, IM_ARRAYSIZE(modelPath));
        if (strlen(modelPath) == 0) strcpy(modelPath, globalConfig.get<std::string>("model.path").c_str());

        ImGui::Text("Model Normalized Size: ");
        ImGui::SameLine();
        static float modelSize = globalConfig.get<float>("model.size");
        ImGui::InputFloat("##Model Normalized Size", &modelSize);

        if (ImGui::Button("Set Model")) {
            spdlog::info("Setting model to {}", modelPath);
            auto oldModel = globalConfig.set("model.path", std::string(modelPath));
            auto oldSize = globalConfig.set("model.size", modelSize);
            try {
                getRenderer().setModel();
            } catch (const std::exception& e) {
                globalConfig.set("model.path", oldModel);
                strcpy(modelPath, oldModel.c_str());
                globalConfig.set("model.size", oldSize);
                spdlog::error(e.what());
            }
        }
        
    }
}

void renderingMode() {
    auto &renderer = getRenderer();

    if (ImGui::CollapsingHeader("Rendering Mode")) {

        ImGui::SeparatorText("Rendering Mode");

        static int renderingMode = renderer.getRenderingMode();
        ImGui::PushID("RenderingMode");
        ImGui::RadioButton("None", &renderingMode, 0);
        ImGui::RadioButton("Rasterization", &renderingMode, 1);
        ImGui::RadioButton("Raytracing", &renderingMode, 2);
        ImGui::RadioButton("Normal", &renderingMode, 3);
        ImGui::RadioButton("Depth", &renderingMode, 4);
        ImGui::PopID();

        if (renderingMode != renderer.getRenderingMode()) {
            renderer.setRenderingMode(static_cast<Renderer::RenderingMode>(renderingMode));
        }

    }
}

void transformation() {
    auto& renderer = getRenderer();

    if (ImGui::CollapsingHeader("Transformation")) {

        ImGui::SeparatorText("Transformation");

        static glm::vec3 translation;
        static glm::vec3 rotation;
        static glm::vec3 scale;
        translation = renderer.getTranslation();
        rotation = renderer.getRotationByEulerAngles();
        rotation = glm::degrees(rotation);
        scale = renderer.getScale();

        ImGui::Text("Translation: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Translation", glm::value_ptr(translation), 0.01f);

        ImGui::Text("Rotation: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.5f);

        ImGui::Text("Scale: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.01f);

        static bool uniformScale = true;
        ImGui::Checkbox("Uniform Scale", &uniformScale);
        
        if (uniformScale) {
            auto avg = (scale.x + scale.y + scale.z) / 3.f;
            scale = glm::vec3(avg, avg, avg);
        }

        rotation = glm::radians(rotation);

        if (translation != renderer.getTranslation() || rotation != renderer.getRotationByEulerAngles() || scale != renderer.getScale()) {
            renderer.setTranslation(translation);
            renderer.setRotationByEulerAngles(rotation);
            renderer.setScale(scale);
        }

        ImGui::SeparatorText("Reset");

        if (ImGui::Button("Reset")) {
            renderer.getMotion().reset();
        }

        ImGui::SeparatorText("Interaction Control");

        ImGui::TextWrapped("To interact with the object, select the type of transformation you want to apply.");

        static int interaction = 0;
        ImGui::PushID("Interaction");
        ImGui::RadioButton("None", &interaction, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Translation", &interaction, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Rotation", &interaction, 2);
        ImGui::SameLine();
        ImGui::RadioButton("Scale", &interaction, 3);
        ImGui::PopID();


        if (interaction != 0) {
            renderer.handleInteractiveTransformation(interaction);
        }

    }
}

void lighting() {
    auto& renderer = getRenderer();

    if (ImGui::CollapsingHeader("Lighting")) {
        if (renderer.getRenderingMode() == Renderer::RenderingMode::RAYTRACING) {
            ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f),
                "Lighting settings are disabled in Ray Tracing mode."
            );
            return;
        }

        static bool showLighting = renderer.shouldShowLighting();

        static glm::vec3 lightPosition = renderer.getLight().getPosition();
        static glm::vec3 lightAmbient = renderer.getLight().getAmbient();
        static glm::vec3 lightDiffuse = renderer.getLight().getDiffuse();
        static glm::vec3 lightSpecular = renderer.getLight().getSpecular();

        ImGui::Checkbox("Show Lighting", &showLighting);

        ImGui::Text("Light Position: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Light Position", glm::value_ptr(lightPosition), 0.01f, -5.f, 5.f);

        ImGui::Text("Light Ambient:  ");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Light Ambient", glm::value_ptr(lightAmbient));

        ImGui::Text("Light Diffuse:  ");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Light Diffuse", glm::value_ptr(lightDiffuse));

        ImGui::Text("Light Specular: ");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Light Specular", glm::value_ptr(lightSpecular));

        if (
            lightPosition != renderer.getLight().getPosition() || 
            lightAmbient != renderer.getLight().getAmbient() || 
            lightDiffuse != renderer.getLight().getDiffuse() || 
            lightSpecular != renderer.getLight().getSpecular() ||
            showLighting != renderer.shouldShowLighting()
        ) {
            renderer.getLight().setPosition(lightPosition);
            renderer.getLight().setAmbient(lightAmbient);
            renderer.getLight().setDiffuse(lightDiffuse);
            renderer.getLight().setSpecular(lightSpecular);
            renderer.setLighting(showLighting);
        }
    }
}

void camera() {
    auto& renderer = getRenderer();

    if (ImGui::CollapsingHeader("Camera")) {

        ImGui::SeparatorText("Interaction Control");

        static bool focus;
        focus = renderer.getFocus();
        ImGui::Checkbox("Focus", &focus);
        renderer.setFocus(focus);

        if (focus) ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f),
            "Focus mode is enabled.\n"
            "Press ` (or ~) to toggle focus mode."
        );

        ImGui::SeparatorText("Camera Information");

        static glm::vec3 cameraPosition;
        cameraPosition = renderer.getCamera().getPosition();
        static glm::vec3 cameraFront;
        cameraFront = renderer.getCamera().getFront();
        static glm::vec3 cameraUp;
        cameraUp = renderer.getCamera().getUp();
        static glm::vec3 cameraRight;
        cameraRight = renderer.getCamera().getRight();

        static float cameraPitch;
        cameraPitch = renderer.getCamera().getPitch();
        static float cameraYaw;
        cameraYaw = renderer.getCamera().getYaw();

        static float cameraSpeed = renderer.getCamera().getSpeed();
        static float cameraSensitivity = renderer.getCamera().getSensitivity();
        static float cameraFov = renderer.getCamera().getFov();

        ImGui::Text("Camera Position: ");
        ImGui::SameLine();
        ImGui::InputFloat3("##Camera Position", glm::value_ptr(cameraPosition), "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Front: ");
        ImGui::SameLine();
        ImGui::InputFloat3("##Camera Front", glm::value_ptr(cameraFront), "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Up: ");
        ImGui::SameLine();
        ImGui::InputFloat3("##Camera Up", glm::value_ptr(cameraUp), "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Right: ");
        ImGui::SameLine();
        ImGui::InputFloat3("##Camera Right", glm::value_ptr(cameraRight), "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Pitch: ");
        ImGui::SameLine();
        ImGui::InputFloat("##Camera Pitch", &cameraPitch, 0.1f, 0.1f, "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Yaw: ");
        ImGui::SameLine();
        ImGui::InputFloat("##Camera Yaw", &cameraYaw, 0.1f, 0.1f, "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Camera Speed: ");
        ImGui::SameLine();
        ImGui::DragFloat("##Camera Speed", &cameraSpeed, 0.01f, 0.01f, 10.f);

        ImGui::Text("Camera Sensitivity: ");
        ImGui::SameLine();
        ImGui::DragFloat("##Camera Sensitivity", &cameraSensitivity, 0.01f, 0.01f, 10.f);

        ImGui::Text("Camera FOV: ");
        ImGui::SameLine();
        ImGui::DragFloat("##Camera FOV", &cameraFov, 0.1f, 1.f, 179.f);

        if (
            cameraSpeed != renderer.getCamera().getSpeed() || 
            cameraSensitivity != renderer.getCamera().getSensitivity() ||
            cameraFov != renderer.getCamera().getFov()
        ) {
            renderer.getCamera().setSpeed(cameraSpeed);
            renderer.getCamera().setSensitivity(cameraSensitivity);
            renderer.getCamera().setFov(cameraFov);
        }
    }
}

void animation() {
    
    auto& renderer = getRenderer();

    if (renderer.getRenderingMode() == Renderer::RenderingMode::RAYTRACING) {
        ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f),
            "Animation is not supported in Ray-Tracing mode."
        );
        return;
    }

    ImGui::SeparatorText("Keyframes");

    if (ImGui::Button("Add Keyframe")) {
        ImGui::OpenPopup("Add Keyframe");
    }
    if (ImGui::BeginPopupModal("Add Keyframe", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        auto motion = renderer.getMotion();
        static float time = 0.f;
        glm::vec3 translation = motion.translation;
        glm::quat rotation = motion.rotation;
        glm::vec3 scale = motion.scale;

        ImGui::Text("Time (in sec.): ");
        ImGui::SameLine();
        ImGui::InputFloat("##Time", &time, 1.f);

        ImGui::Text("Translation: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Translation", glm::value_ptr(translation), 0.01f);

        ImGui::Text("Rotation: ");
        ImGui::SameLine();
        ImGui::DragFloat4("##Rotation", glm::value_ptr(rotation), 0.01f);

        ImGui::Text("Scale: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.01f);

        if (ImGui::Button("OK")) {
            renderer.getAnimation().addKeyframe({
                translation,
                glm::quat(rotation),
                scale
            }, time);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear Keyframes")) {
        renderer.getAnimation().clearKeyframes();
    }

    if (ImGui::BeginTable("Keyframes", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable)) {
        
        ImGui::TableSetupScrollFreeze(0, 1); 

        ImGui::TableSetupColumn("Num.", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Translation", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Rotation", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Scale", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        auto& keyframes = renderer.getAnimation().getKeyframes();
        for (int i = 0; i < (int) keyframes.size(); i++) {
            auto& keyframe = keyframes[i].first;
            auto& time = keyframes[i].second;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%d", i);
            ImGui::TableNextColumn();
            ImGui::Text("(%.2f, %.2f, %.2f)", keyframe.translation.x, keyframe.translation.y, keyframe.translation.z);
            ImGui::TableNextColumn();
            ImGui::Text("(%.2f, %.2f, %.2f, %.2f)", keyframe.rotation.x, keyframe.rotation.y, keyframe.rotation.z, keyframe.rotation.w);
            ImGui::TableNextColumn();
            ImGui::Text("(%.2f, %.2f, %.2f)", keyframe.scale.x, keyframe.scale.y, keyframe.scale.z);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", time);
            ImGui::TableNextColumn();
            ImGui::PushID(("Keyframe-Action-" + std::to_string(i)).c_str());
            if (ImGui::Button("Remove")) {
                renderer.getAnimation().removeKeyframe(i);
            }
            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    ImGui::SeparatorText("Animation Control");

    static bool useAnimation = renderer.isUsingAnimation();
    ImGui::Checkbox("Use Animation", &useAnimation);
    renderer.setUseAnimation(useAnimation);

    static bool playAnimation;
    playAnimation = renderer.isPlayingAnimation();
    ImGui::Button(playAnimation ? "Pause" : "Play");
    if (ImGui::IsItemClicked()) {
        playAnimation = !playAnimation;
        renderer.playAnimation(playAnimation);
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Animation")) {
        renderer.getAnimation().reset();
        renderer.playAnimation(false);
    }

    ImGui::SeparatorText("Animation Information");

    auto motion = renderer.getAnimation().getCurrentMotion();

    ImGui::Text("Current Time: ");
    ImGui::SameLine();
    
    float currentTime = renderer.getAnimation().getCurrentTime();
    float maxTime = renderer.getAnimation().getMaxTime();

    ImGui::SliderFloat("##Current Time", &currentTime, 0.f, maxTime);

    renderer.getAnimation().setCurrentTime(currentTime);

    ImGui::Text("Current Keyframe: ");
    ImGui::SameLine();
    ImGui::Text("%d", renderer.getAnimation().getCurrentKeyframe());

    ImGui::Text("Translation: ");
    ImGui::SameLine();
    ImGui::Text("(%.2f, %.2f, %.2f)", motion.translation.x, motion.translation.y, motion.translation.z);

    ImGui::Text("Rotation: ");
    ImGui::SameLine();
    ImGui::Text("(%.2f, %.2f, %.2f, %.2f)", motion.rotation.x, motion.rotation.y, motion.rotation.z, motion.rotation.w);

    ImGui::Text("Scale: ");
    ImGui::SameLine();
    ImGui::Text("(%.2f, %.2f, %.2f)", motion.scale.x, motion.scale.y, motion.scale.z);

}

void rayTracing() {
    auto& renderer = getRenderer();

    if (renderer.getRenderingMode() != Renderer::RenderingMode::RAYTRACING) {
        return;
    }

    if (ImGui::CollapsingHeader("Ray Tracing")) {

        ImGui::SeparatorText("Configuration");

        static int maxBounces = renderer.getRayTracer()->getMaxBounces();
        ImGui::Text("Max Bounces: ");
        ImGui::SameLine();
        ImGui::InputInt("##Max Bounces", &maxBounces);

        if (maxBounces != renderer.getRayTracer()->getMaxBounces()) {
            if (maxBounces < 0) maxBounces = 0;
            renderer.getRayTracer()->setMaxBounces(maxBounces);
            renderer.getRayTracer()->rerender();
        }

        static int spp = renderer.getRayTracer()->getSPP();
        ImGui::Text("Samples Per Pixel: ");
        ImGui::SameLine();
        ImGui::InputInt("##SPP", &spp);

        if (spp != renderer.getRayTracer()->getSPP()) {
            if (spp < 1) spp = 1;
            renderer.getRayTracer()->setSPP(spp);
            renderer.getRayTracer()->rerender();
        }

        ImGui::SeparatorText("Lighting");

        static glm::vec3 lightPos, lightRadiance;
        static float lightRadius;
        lightPos = renderer.getRayTracer()->getScene().lightPos;
        lightRadiance = renderer.getRayTracer()->getScene().lightRadiance;
        lightRadius = renderer.getRayTracer()->getScene().lightRadius;

        ImGui::Text("Light Position: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Light Position", glm::value_ptr(lightPos), 0.05f);

        ImGui::Text("Light Radiance: ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Light Radiance", glm::value_ptr(lightRadiance), 1.f);

        static bool uniformRadiance = false;
        ImGui::Checkbox("Uniform", &uniformRadiance);

        if (uniformRadiance) {
            auto avg = (lightRadiance.x + lightRadiance.y + lightRadiance.z) / 3.f;
            lightRadiance = glm::vec3(avg, avg, avg);
        }

        ImGui::Text("Light Radius: ");
        ImGui::SameLine();
        ImGui::DragFloat("##Light Radius", &lightRadius, 0.01f);

        if (
            lightPos != renderer.getRayTracer()->getScene().lightPos ||
            lightRadiance != renderer.getRayTracer()->getScene().lightRadiance ||
            lightRadius != renderer.getRayTracer()->getScene().lightRadius
        ) {
            renderer.getRayTracer()->setLight(lightPos, lightRadiance, lightRadius);
            renderer.getRayTracer()->rerender();
        }
    }
}

}