#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <nfd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ImageneerViewController.h"

#include <iostream>
#include <cstdlib>
#include <csignal>
#include <chrono>

//TODO: implement with MVC

namespace gui
{
    const char* kMainWindowName = "Imageneer";

    //TODO: make class to show logs in another window, initialize here passing glfw window

    void ImageneerController::Init(GLFWwindow* window, char const* glsl_version)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        mWindow = window;
    }

    void ImageneerController::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    //TODO: add some sort of logging system

    //TODO: this UI controlls the data that is used for managing the rest of the application
    //COULD: make a singleton for the data used for control of the application
    
    //TODO: would make sense to have a ImGuiImageneerController and a CVImageneerController for each tipe of display
          //or just modify the images and load them here 
    

    //TODO: make class for making different effects to the images and use here
    void ImageneerController::Update()
    {
        ImGui::Begin(kMainWindowName, nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::SetWindowPos(ImVec2(0, 0));

        ImGui::StyleColorsClassic();

        glfwGetWindowSize(mWindow, &mData.mMainWindowWidth, &mData.mMainWindowHeight);
        ImGui::SetWindowSize(ImVec2(static_cast<float>(mData.mMainWindowWidth), static_cast<float>(mData.mMainWindowHeight)));

        if (ImGui::Button("Search Image", ImVec2(150, 30)))
        {
            if (OpenFileExplorerDialog())
            {
                LoadTextureFromFile();
                mData.mShowImage = true;
            }
        }

        //TODO: implement some sort of messaging/signal mechanism between the different threads
        ImGui::SameLine();

        if (!mData.mCameraOpened)
        {
            if (ImGui::Button("Open Camera", ImVec2(150, 30)))
            {
                //TODO: if camera is disconnected, it fails
                mCVCameraThread = std::thread(&cvFunc::ComputerVisionFunc::OpenCamera, &mCVFunc);
                mData.mCameraOpened = true;
            }
        }
        else if(ImGui::Button("Close Camera", ImVec2(150, 30)))
        {
            mCVFunc.StopShowingCamera();
            mData.mCameraOpened = false;
            mCVCameraThread.detach();
        }

        if (mData.mShowImage)
        {
            ImGui::BeginChild("Image Display", ImVec2(ImGui::GetContentRegionAvail().x,
                ImGui::GetContentRegionAvail().y / 1.1f), true, ImGuiWindowFlags_HorizontalScrollbar);

            if (mData.mImageData.loaded)
            {
                //TODO: make function to determine the height and width of window to know how to display image
                ImGui::Image((void*)(intptr_t)mData.mImageData.texture,
                    ImVec2(static_cast<float>(mData.mImageData.width), static_cast<float>(mData.mImageData.height)));
            }

            ImGui::EndChild();

            //TODO: store the original and the modified image until the user chooses to save
            if (mData.mShowEffectsWindow)
            {
                ShowEffectsWindow();
            }
            else if(ImGui::Button("Effects Window", ImVec2(150, 30)))
            {
                mData.mShowEffectsWindow = true;
            }
            else
            {
                ImGui::SameLine();
            }

            if (ImGui::Button("Close Image", ImVec2(150, 30)))
            {
                mData.mShowImage = false;
                mData.mShowEffectsWindow = false;
                mData.mImageData.Clear();
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(100));

        ImGui::End();
    }



    void ImageneerController::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImageneerController::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        if (mCVCameraThread.joinable()) { mCVCameraThread.join(); }
        if (mCVImgProcThread.joinable()) { mCVImgProcThread.join(); }
    }

    void ImageneerController::ShowEffectsWindow()
    {
        ImGui::Begin("Effects Window");
        ImGui::SetWindowSize(ImVec2(mData.mMainWindowWidth / 2, mData.mMainWindowHeight / 2));

        if (ImGui::Button("Save Image", ImVec2(150, 30)))
        {
            std::cout << "Saving image and stuff\n";
        }
        ImGui::SameLine();
        if (ImGui::Button("Close Window", ImVec2(150, 30)))
        {
            mData.mShowEffectsWindow = false;
        }
        ImGui::End();
    }

    void ImageneerController::LoadTextureFromFile()
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(mData.mImageData.filePath, &image_width, &image_height, NULL, 4);
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

        mData.mImageData.texture = image_texture;
        mData.mImageData.width = image_width;
        mData.mImageData.height = image_height;

        mData.mImageData.loaded = true;
    }

    //COULD: add a thread for the dialogs
    bool ImageneerController::OpenFileExplorerDialog()
    {
        char* outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("png,jpg,bmp", NULL, &outPath);

        if (result == NFD_OKAY) {
            mData.mImageData.filePath = outPath;

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

