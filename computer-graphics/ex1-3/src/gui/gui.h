#ifndef GUI_H__
#define GUI_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

#include "../renderer/renderer.h"

namespace GUI {
    void init(GLFWwindow* window);
    void render();
    void shutdown();
    std::map<std::string, std::string>& getGUIConfig();
};

#endif