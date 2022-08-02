#pragma once

class ImGuiStyle;
class ImVec2;

namespace gui
{
const int W_HEIGHT = 800;
const int W_WIDTH = 700;

class ImGuiController
{
public:
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	void NewFrame();
	void Update();
	void Shutdown();

private:
	ImGuiStyle* style;
};

} // gui
