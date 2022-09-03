#pragma once

#include <mutex>
#include "ImageneerData.h"
#include "computerVisionFunc.h"

namespace gui
{
const int kInitHeight = 700;
const int kInitWidth = 800;
const int kMinHeight = 300;
const int kMinWidth = 400;

class ImageneerController
{
public:
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	void NewFrame();
	void Update();
	void Shutdown();
private:
	bool OpenFileExplorerDialog();
	void LoadTextureFromFile();
	void ShowEffectsWindow();
	
	//View component is managed by ImGui
	ImageneerData mData;

	cvFunc::ComputerVisionFunc mCVFunc;

	std::thread mCVCameraThread;
	std::thread mCVImgProcThread;

	GLFWwindow* mWindow;
};

} // gui
