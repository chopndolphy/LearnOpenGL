#include "SimpleGui.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void SimpleGui::renderWindow(float color[]) {
ImGui::Begin("Simple Window");
ImGui::ColorEdit4("Light Color", color);
ImGui::End();
}