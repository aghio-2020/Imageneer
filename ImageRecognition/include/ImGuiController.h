#pragma once

class ImGuiController
{
public:
	void Init(GLFWwindow* window, char const* glsl_version);
	void Render();
	virtual void Update();
	void Shutdown();
};
