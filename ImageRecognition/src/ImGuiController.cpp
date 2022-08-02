#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "nfd.h"

#include "ImGuiController.h"

#include <iostream>
#include <cstdlib>

namespace gui
{

const ImVec2 window_size(W_HEIGHT, W_WIDTH);

// call the bindings and set preferences for the window style
void ImGuiController::Init(GLFWwindow* window, char const* glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
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
    /* TODO: develop two views for the app
    
    if (imageIsLoaded)
        showViewLoaded();
    else
        showViewSearchImage();

    */

    ImGui::Begin("Image Recognition");

    ImGui::SetWindowSize(window_size);
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::StyleColorsDark();

    //TODO: make the child size equal to the resolution of the image loaded, or load the image in the same size as the child
    ImGui::BeginChild("image", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x / 2), true, ImGuiWindowFlags_HorizontalScrollbar);

    //TODO: load image from imgui

    ImGui::EndChild();

    //TODO: add functionality with OpenCV or call OpenCV external application to process image and return new one
    //TODO: make static library with openCV required functionality and link to this app to use the classes

    //TODO: open native file explorer to look for an image (jpeg or png)
    if (ImGui::Button("Search Image", ImVec2(100, 30)))
    {
        char* outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("png,jpg,bmp", NULL, &outPath);

        if (result == NFD_OKAY) {
            puts("Success!");
            puts(outPath);
            free(outPath);
        }
        else if (result == NFD_CANCEL) {
            puts("User pressed cancel.");
        }
        else {
            printf("Error: %s\n", NFD_GetError());
        }

    }

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

} // gui
