#pragma once

#include <memory>

#include "ImageneerData.h"

namespace cvFunc
{
	class ComputerVisionFunc
	{
	public:
		ComputerVisionFunc();
		~ComputerVisionFunc();
		void OpenCamera();
		bool IsCameraOpened();
		void StopShowingCamera();
		void StartShowingCamera();
		void SetTmpFile(const char* path);
		void SaveImage(const char* path);
	private:
		struct CVData;
		std::unique_ptr<CVData> mCVData;
		gui::ImageneerDataSingleton* mDataSingletonInstance;
		std::mutex mMutex;
	};
}
