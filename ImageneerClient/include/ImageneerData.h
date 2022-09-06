#pragma once

#include "imgui.h"
#include <cinttypes>

namespace gui
{
	struct ImageData
	{
		char* filePath;
		bool loaded = false;
		uint8_t width;
		uint8_t height;
		GLuint texture;

		void Clear() { width, height, texture = 0; loaded = false; filePath = nullptr; }
	};

	struct ImageneerData
	{
		ImGuiStyle* mStyle;
		ImageData mImageData;

		bool mShowImage = false;
		bool mCameraOpened = false;
		bool mShowEffectsWindow = false;
		int mMainWindowWidth;
		int mMainWindowHeight;
	};
}