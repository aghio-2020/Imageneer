#pragma once

#include <string>

struct ImGuiStyle;
struct ImVec2;

namespace gui
{
const int kInitHeight = 700;
const int kInitWidth = 800;
const int kMinHeight = 300;
const int kMinWidth = 400;

struct ImageData
{
	 char *filePath;
	 bool loaded = false;
	 int width;
	 int height;
	 GLuint texture;
};

class UIController
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

	ImGuiStyle* mStyle;
	GLFWwindow* mWindow;
	ImageData mImageData;
};

} // gui
