#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>


#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)
#pragma warning(disable : 6294)
#pragma warning(disable : 6201)
#pragma warning(disable : 6262)
#include <stb_image.h>
#include <portable-file-dialogs.h>
#pragma warning(default : 26812)
#pragma warning(default : 26495)
#pragma warning(default : 26451)
#pragma warning(default : 6294)
#pragma warning(default : 6201)
#pragma warning(default : 6262)
#include "ImageneerViewController.h"

#include <iostream>
#include <cstdlib>

//TODO: implement with MVC

namespace gui
{
    const char* kMainWindowName = "Imageneer";
    const char* kImageFilters = "png,jpg,bmp";
    const float kSpacingRatio = 1.1f;

    //TODO: make class to show logs in another window, initialize here passing glfw window

    ImageneerViewController::~ImageneerViewController() = default;

    ImageneerViewController::ImageneerViewController() :
        mWindow(nullptr)
    {
        mDataSingletonInstance = ImageneerDataSingleton::Instance();
    }

    void ImageneerViewController::Init(GLFWwindow* window, char const* glsl_version)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        mWindow = window;
    }

    void ImageneerViewController::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImageneerViewController::Update()
    {
        ImGui::Begin(kMainWindowName, nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::SetWindowPos(ImVec2(0, 0));

        ImGui::StyleColorsClassic();

        {
            int width, height;
            glfwGetWindowSize(mWindow, &width, &height);
            mDataSingletonInstance->SetMainWindowWidth(width);
            mDataSingletonInstance->SetMainWindowHeight(height);
            ImGui::SetWindowSize(ImVec2(static_cast<float>(mDataSingletonInstance->GetMainWindowWidth()), 
                static_cast<float>(mDataSingletonInstance->GetMainWindowHeight())));
        }

        if (ImGui::Button("Search Image", ImVec2(150, 30)))
        {
            if (OpenFileExplorerDialog() && LoadTextureFromFile())
            {
                mDataSingletonInstance->SetShowImageView(true);
                mDataSingletonInstance->UpdateTmpFileData();
                mCVFunc.UpdateTmpFile();
            } 
        }

        ImGui::SameLine();

        if (!mDataSingletonInstance->GetShowCameraView())
        {
            if (ImGui::Button("Open Camera", ImVec2(150, 30)))
            {
                if (mCVCameraThread.joinable())
                {
                    mCVCameraThread.detach();
                }
                mCVCameraThread = std::thread(&ComputerVisionFunc::OpenCamera, &mCVFunc);
            }
        }
        else if (ImGui::Button("Close Camera", ImVec2(150, 30)))
        {
            mCVFunc.StopShowingCamera();
            mDataSingletonInstance->SetShowCameraView(false);
            mCVCameraThread.detach();
        }

        if (mDataSingletonInstance->GetShowImageView())
        {
            ShowImageView();
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(10));

        ImGui::End();
    }

    void ImageneerViewController::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImageneerViewController::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        mCVFunc.StopShowingCamera();
        if (mCVCameraThread.joinable()) { mCVCameraThread.join(); }
        if (mCVImgProcThread.joinable()) { mCVImgProcThread.join(); }
    }

    void ImageneerViewController::ShowImageView()
    {
        if (mDataSingletonInstance->GetUpdateImageTexture())
        {
            ReloadTextureFromTmpFile();
            mDataSingletonInstance->SetUpdateImageTexture(false);
        }

        float ratioY = (ImGui::GetContentRegionAvail().y / 1.1f) / mDataSingletonInstance->GetImageDataHeight();
        float ratioX = ImGui::GetContentRegionAvail().x / (mDataSingletonInstance->GetImageDataWidth() * ratioY);
        float offSet = ImGui::GetContentRegionAvail().x - (ImGui::GetContentRegionAvail().x * ratioX);

        ImGui::BeginChild("Image Display", ImVec2(ImGui::GetContentRegionAvail().x / ratioX,
            ImGui::GetContentRegionAvail().y / kSpacingRatio), true, ImGuiWindowFlags_HorizontalScrollbar);

        if (mDataSingletonInstance->GetImageDataLoaded())
        {
            ImGui::Image((void*)(intptr_t)mDataSingletonInstance->GetImageDataTexture(),
                ImVec2(static_cast<float>(mDataSingletonInstance->GetImageDataWidth() * ratioY),
                    static_cast<float>(mDataSingletonInstance->GetImageDataHeight() * ratioY)));
        }

        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("Some Settings", ImVec2(ImGui::GetContentRegionAvail().x - offSet, 
            ImGui::GetContentRegionAvail().y / kSpacingRatio));
        ImGui::Text("Some Features");
        ImGui::EndChild();

        if (mDataSingletonInstance->GetShowEffectsWindow())
        {
            ShowEffectsWindow();
        }
        else if (ImGui::Button("Effects Window", ImVec2(150, 30)))
        {
            mDataSingletonInstance->SetShowEffectsWindow(true);
        }
        else
        {
            ImGui::SameLine();
        }

        if (ImGui::Button("Close Image", ImVec2(150, 30)))
        {
            mDataSingletonInstance->SetShowImageView(false);
            mDataSingletonInstance->SetShowEffectsWindow(false);
            mDataSingletonInstance->ClearImageData();
        }
        ImGui::SameLine();
    }

    //TODO: move somewhere else
    float blur;
    float grayscale;

    void ImageneerViewController::ShowEffectsWindow()
    {
        ImGui::Begin("Effects Window");
        ImGui::SetWindowSize(ImVec2(static_cast<float>(mDataSingletonInstance->GetMainWindowWidth()) / 2, 
            static_cast<float>(mDataSingletonInstance->GetMainWindowHeight()) / 2));

        ImGui::Text("Options");
        if (ImGui::Button("Save Image", ImVec2(150, 30)))
        {
            std::cout << "Saving image and stuff\n";
            OpenSaveFileDialog();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close Window", ImVec2(150, 30)))
        {
            mDataSingletonInstance->SetShowEffectsWindow(false);
        }

        //TODO: use imageProc thread here
        ImGui::NewLine();
        ImGui::Text("Effects");
        if (ImGui::Button("Detect Faces"))
        {
            std::cout << "Implement Face Detection\n";
        }
        if (ImGui::Button("Blur Image"))
        {
            //TODO: pass parameters indicated by the ui
            //COULD: make effects parameters class and store in singleton EffectsData atribute
            mCVFunc.Blur();
            std::cout << "Implement Blur Image\n";
        }
        ImGui::SameLine();
        ImGui::SliderFloat("blur", &blur, 0.0f, 1.0f);
        if (ImGui::Button("Grayscale"))
        {
            mCVFunc.Grayscale();
            std::cout << "Implement Grayscale\n";
        }

        ImGui::End();
    }

    //TODO: make a texture class/struct
    bool ImageneerViewController::LoadTextureFromFile()
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(mDataSingletonInstance->GetImageDataFilePath(), &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            std::cout << "NULL image data" << image_data << "\n";
            return false;
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

        mDataSingletonInstance->SetImageDataTexture(image_texture);
        mDataSingletonInstance->SetImageDataWidth(image_width);
        mDataSingletonInstance->SetImageDataHeight(image_height);

        mDataSingletonInstance->SetImageDataLoaded(true);

        return true;
    }

    //TODO: find how to unload the opengl texture to optimize memory
    bool ImageneerViewController::ReloadTextureFromTmpFile()
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(mDataSingletonInstance->GetTmpFilePath(), &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            std::cout << "NULL image data\n";
            return false;
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

        mDataSingletonInstance->SetImageDataTexture(image_texture);
        mDataSingletonInstance->SetImageDataWidth(image_width);
        mDataSingletonInstance->SetImageDataHeight(image_height);

        mDataSingletonInstance->SetImageDataLoaded(true);

        return true;
    }

    //COULD: add a thread for the dialogs
    bool ImageneerViewController::OpenFileExplorerDialog()
    {
        //TODO: save into disk the previous path
        auto file = pfd::open_file("Choose an image file", pfd::path::home(),
            { "Image Files (.jpg, .png, .bmp)", "*.jpg *.jpeg *.png *.bmp" },
            pfd::opt::force_overwrite).result();

        if (!file.empty())
        {
            std::string outPath = file.front();
            mDataSingletonInstance->SetImageDataFilePath(const_cast<char*>(outPath.c_str()));
            return true;
        }
        else
        {
            std::cout << "File open canceled or failed\n";
            return false;
        }

    }

    bool ImageneerViewController::OpenSaveFileDialog()
    {
        auto destination = pfd::save_file("Select a file", ".",
            { "Image Files", "*.png *.jpg *.jpeg *.bmp" },
            pfd::opt::force_overwrite).result();

        if (!destination.empty())
        {
            mCVFunc.SaveImage(destination.c_str());
            mDataSingletonInstance->SetImageDataFilePath(const_cast<char*>(destination.c_str()));
            mCVFunc.UpdateTmpFile();
            return true;
        }
        else
        {
            std::cout << "Canceled or Failed save\n";
            return false;
        }
    }

}

