#pragma once

#include <GLFW/glfw3.h>

#include <mutex>
#include <memory>

namespace gui
{
	typedef void* HandlerImage;

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

		void SetMainWindowWidth(const int& width);
		void SetMainWindowHeight(const int& height);
		void SetShowImageView(const bool& showImage);
		void SetShowCameraView(const bool& showCamera);
		void SetShowEffectsWindow(const bool& showEffectsWindow);

		const int& GetImageDataWidth();
		const int& GetImageDataHeight();
		const GLuint& GetImageDataTexture();
		const char* GetImageDataFilePath();
		const bool& GetImageDataLoaded();
		void ClearImageData();

		void SetImageDataWidth(const int &width);
		void SetImageDataHeight(const int &height);
		void SetImageDataTexture(const GLuint &texture);
		void SetImageDataFilePath(char *filePath);
		void SetImageDataLoaded(const bool &loaded);

	private:
		ImageneerDataSingleton();

		struct ImageData;
		std::unique_ptr<ImageData> mImageData;
		std::mutex mMutex;
		int mMainWindowWidth;
		int mMainWindowHeight;
		bool mShowImageView;
		bool mShowCameraView;
		bool mShowEffectsWindow;
	};
}