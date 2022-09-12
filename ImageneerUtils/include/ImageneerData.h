#pragma once

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <opencv2/imgproc.hpp>

#include <mutex>

namespace gui
{
	//tmp image data is shared through out the program and used to decide what to show
	struct ImageData
	{
		//TODO: make thread safe getters and setters
		cv::Mat mTmpImage;
		int mWidth;
		int mHeight;
		GLuint mTexture;
		char* mFilePath;
		bool mLoaded = false;

		void Clear() { mWidth, mHeight, mTexture = 0; mLoaded = false; mFilePath = nullptr; mTmpImage.release(); }
	};

	//the client shares the information that is being used to control the program
	class ImageneerDataSingleton
	{
	public:
		static ImageneerDataSingleton *Instance()
		{
			static ImageneerDataSingleton *mInstance = new ImageneerDataSingleton;
			return mInstance;
		}

		const int &GetMainWindowWidth();
		const int &GetMainWindowHeight();
		const bool &GetShowImageView();
		const bool &GetShowCameraView();
		const bool &GetShowEffectsWindow();
		ImageData& GetImageDataReference();

		void SetMainWindowWidth(const int& width);
		void SetMainWindowHeight(const int& height);
		void SetShowImageView(const bool& showImage);
		void SetShowCameraView(const bool& showCamera);
		void SetShowEffectsWindow(const bool& showEffectsWindow);

	private:
		ImageneerDataSingleton();

		ImageData mImageData;
		std::mutex mMutex;
		int mMainWindowWidth;
		int mMainWindowHeight;
		bool mShowImageView;
		bool mShowCameraView;
		bool mShowEffectsWindow;
	};
}