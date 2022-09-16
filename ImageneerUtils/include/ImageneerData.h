#pragma once

#include <GLFW/glfw3.h>

#include <mutex>
#include <memory>

namespace gui
{
	//the client shares the information that is being used to control the program
	class ImageneerDataSingleton
	{
	public:
		static ImageneerDataSingleton* Instance();

		const char* GetTmpFilePath();
		void UpdateTmpFileData();

		const int &GetMainWindowWidth();
		const int &GetMainWindowHeight();
		const bool &GetShowImageView();
		const bool &GetShowCameraView();
		const bool &GetShowEffectsWindow();
		const bool &GetUpdateImageTexture();

		void SetMainWindowWidth(const int& width);
		void SetMainWindowHeight(const int& height);
		void SetShowImageView(const bool& showImage);
		void SetShowCameraView(const bool& showCamera);
		void SetShowEffectsWindow(const bool& showEffectsWindow);
		void SetUpdateImageTexture(const bool& updateImageTexture);

		const int& GetImageDataWidth();
		const int& GetImageDataHeight();
		const GLuint& GetImageDataTexture();
		const char* GetImageDataFilePath();
		const char* GetImageDataFileType();
		const bool& GetImageDataLoaded();
		void ClearImageData();

		void SetImageDataWidth(const int &width);
		void SetImageDataHeight(const int &height);
		void SetImageDataTexture(const GLuint &texture);
		void SetImageDataFilePath(const char *filePath);
		void SetImageDataFileType(const char* type);
		void SetImageDataLoaded(const bool &loaded);

	private:
		ImageneerDataSingleton();
		
		std::mutex mMutex;
		struct ImageData;
		std::unique_ptr<ImageData> mImageData;
		struct EffectsData;
		std::unique_ptr<EffectsData> mEffectsData;
		int mMainWindowWidth;
		int mMainWindowHeight;
		bool mShowImageView;
		bool mShowCameraView;
		bool mShowEffectsWindow;
		bool mUpdateImageTexture;
	};
}