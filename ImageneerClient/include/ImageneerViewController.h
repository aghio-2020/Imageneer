#pragma once

#include "ImageneerData.h"
#include "ComputerVisionFunc.h"

#include <string>
#include <memory>
#include <thread>

namespace gui
{
const int kInitHeight = 700;
const int kInitWidth = 800;
const int kMinHeight = 300;
const int kMinWidth = 400;

class ImageneerViewController
{
public:
	~ImageneerViewController();
	ImageneerViewController();
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	void NewFrame();
	void Update();
	void Shutdown();
private:
	bool OpenFileExplorerDialog();
	bool OpenSaveFileDialog();
	bool LoadTextureFromFile();
	bool ReloadTextureFromTmpFile();
	void ShowEffectsWindow();
	void ShowImageView();

	ComputerVisionFunc mCVFunc;

	std::thread mCVCameraThread;
	std::thread mCVImgProcThread;

	ImageneerDataSingleton* mDataSingletonInstance;
	GLFWwindow* mWindow;
};

} // gui
