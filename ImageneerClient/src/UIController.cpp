#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <nfd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "UIController.h"
#include "ComputerVisionFunc.h"

#include <iostream>
#include <cstdlib>
#include <csignal>

namespace gui
{
    const char* kMainWindowName = "Imageneer";

    //TODO: make class to show logs in another window, initialize here passing glfw window

    // call the bindings
    void UIController::Init(GLFWwindow* window, char const* glsl_version)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;
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

    //TODO: add some sort of logging system

    //TODO: this UI controlls the data that is used for managing the rest of the application
    //COULD: make a singleton for the data used for control of the application
    
    //TODO: would make sense to have a ImGuiUIController and a CVUIController for each tipe of display
          //or just modify the images and load them here 
    

    //TODO: make class for making different effects to the images and use here
    void UIController::Update()
    {
        ImGui::Begin(kMainWindowName);

        ImGui::SetWindowPos(ImVec2(0, 0));

        ImGui::StyleColorsLight();

        int width, height;
        glfwGetWindowSize(mWindow, &width, &height);
        ImGui::SetWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));

        //TODO: add functionality with OpenCV or call OpenCV external application to process image and return new one

        if (ImGui::Button("Search Image", ImVec2(100, 30)))
        {
            if (OpenFileExplorerDialog())
            {
                LoadTextureFromFile();
                mShowImage = true;
            }
        }

        //TODO: make separate thread to execute all the opencv features
        //TODO: implement some sort of messaging/signal mechanism between the different threads
        ImGui::SameLine();
        if (!mCameraOpened)
        {
            if (ImGui::Button("Open Camera", ImVec2(100, 30)))
            {
                mCVCameraThread = std::thread(cvFunc::openCamera);
                mCameraOpened = true;
            }
        }
        else
        {
            if (ImGui::Button("Close Camera", ImVec2(100, 30)))
            {
                std::raise(SIGINT);
                mCameraOpened = false;
                mCVCameraThread.detach();
            }
        }

        if (mShowImage)
        {
            ImGui::BeginChild("Image Display", ImVec2(ImGui::GetContentRegionAvail().x,
                ImGui::GetContentRegionAvail().y / 1.1f), true, ImGuiWindowFlags_HorizontalScrollbar);

            if (mImageData.loaded)
            {
                //TODO: make function to determine the height and width of window to know how to display image
                ImGui::Image((void*)(intptr_t)mImageData.texture,
                    ImVec2(static_cast<float>(mImageData.width), static_cast<float>(mImageData.height)));
            }

            ImGui::EndChild();

            if (ImGui::Button("Close Image", ImVec2(100, 30)))
            {
                mShowImage = false;
                mImageData.Clear();
            }
        }

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
        if (mCVCameraThread.joinable()) { mCVCameraThread.join(); }
        if (mCVImgProcThread.joinable()) { mCVImgProcThread.join(); }
    }

    void UIController::LoadTextureFromFile()
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(mImageData.filePath, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            std::cout << "NULL image data\n";
            return;
        }

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
        nfdresult_t result = NFD_OpenDialog("png,jpg,bmp", NULL, &outPath);

        if (result == NFD_OKAY) {
            mImageData.filePath = outPath;

            std::cout << "Success!\n"; 
            std::cout << outPath << std::endl;
            free(outPath);

            return true;
        }
        else if (result == NFD_CANCEL) {
            std::cout << "User pressed cancel.\n";
            return true;
        }
        else {
            std::cout << "Error: " << NFD_GetError() << std::endl;
            return false;
        }
    }

} // gui

