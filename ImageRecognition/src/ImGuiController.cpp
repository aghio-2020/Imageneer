#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuiController.h"

#include <iostream>

// call the bindings and set preferences for the window style
void ImGuiController::Init(GLFWwindow* window, char const* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiController::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiController::Update()
{
    ImGui::Begin("window");
    ImGui::BeginChild("child", ImVec2(ImGui::GetContentRegionAvail().x / 3, 50), true, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::Button("print"))
        std::cout << "print" << std::endl;
    ImGui::EndChild();
    ImGui::End();
}

void ImGuiController::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiController::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
