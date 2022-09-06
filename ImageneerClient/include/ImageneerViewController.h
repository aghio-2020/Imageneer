#pragma once

#include "ImageneerData.h"
#include "computerVisionFunc.h"

#include <mutex>
#include <memory>

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
	void LoadTextureFromFile();
	void ShowEffectsWindow();

	std::unique_ptr<ImageneerData> mData;
	cvFunc::ComputerVisionFunc mCVFunc;

	std::thread mCVCameraThread;
	std::thread mCVImgProcThread;

	GLFWwindow* mWindow;
};

} // gui
