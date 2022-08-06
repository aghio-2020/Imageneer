#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <nfd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "UIController.h"

#include <iostream>
#include <cstdlib>

namespace gui
{

    const ImVec2 kWindowInitSize(kInitWidth, kInitHeight);
    const ImVec2 kWindowMinSize(kMinWidth, kMinHeight);

    const char* kMainWindowName = "Image Recognition";

    //TODO: make class to show logs in another window, initialize here passing glfw window

    // call the bindings
    void UIController::Init(GLFWwindow* window, char const* glsl_version)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        mWindow = window;
    }

    void UIController::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }



    void UIController::Update()
    {
        /* TODO: develop two views for the app

        if (imageIsLoaded)
            showViewLoaded();
        else
            showViewSearchImage();

        */

        ImGui::Begin(kMainWindowName);

        ImGui::SetWindowPos(ImVec2(0, 0));

        ImGui::SetWindowSize(kMainWindowName, kWindowInitSize);

        ImGui::StyleColorsDark();

        //TODO: add functionality with OpenCV or call OpenCV external application to process image and return new one
        //TODO: make static library with openCV required functionality and link to this app to use the classes

        if (ImGui::Button("Search Image", ImVec2(100, 30)))
        {
            if (OpenFileExplorerDialog())
            {
                LoadTextureFromFile();
            }
        }

        ImGui::BeginChild("Image Display", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x / 2), true, ImGuiWindowFlags_HorizontalScrollbar);
        
        if (mImageData.loaded)
        {
            //TODO: make function to show image and determine the height and width to know how to display it
            ImGui::Image((void*)(intptr_t)mImageData.texture, ImVec2(mImageData.width, mImageData.height));
        }
        
        ImGui::EndChild();

        ImGui::End();
    }



    void UIController::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void UIController::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UIController::LoadTextureFromFile()
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(mImageData.filePath, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            puts("NULL image data");
            return;
        }

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        mImageData.texture = image_texture;
        mImageData.width = image_width;
        mImageData.height = image_height;

        mImageData.loaded = true;
    }

    bool UIController::OpenFileExplorerDialog()
    {
        char* outPath = NULL;
        auto result = NFD_OpenDialog("png,jpg,bmp", NULL, &outPath);

        if (result == NFD_OKAY) {
            mImageData.filePath = outPath;

            puts("Success!"); //TODO: add some sort of logging system
            free(outPath);

            return true;
        }
        else if (result == NFD_CANCEL) {
            puts("User pressed cancel.");
            return true;
        }
        else {
            printf("Error: %s\n", NFD_GetError());
            return false;
        }
    }

} // gui

