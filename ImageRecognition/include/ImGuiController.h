#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

class ImGuiController
{
public:
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	virtual void Update();
	void Shutdown();
};
