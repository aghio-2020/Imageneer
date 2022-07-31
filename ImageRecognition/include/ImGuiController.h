#pragma once

class ImGuiStyle;

class ImGuiController
{
public:
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	void NewFrame();
	void Update();
	void Shutdown();

	ImGuiStyle* style;
};
