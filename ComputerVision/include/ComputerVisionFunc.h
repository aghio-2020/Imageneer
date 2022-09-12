#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
		void HandleCamera(cv::VideoCapture& capture);
		bool IsCameraOpened();
		void StopShowingCamera();
		void StartShowingCamera();
		void SetTmpFile(const char* path);
		void SaveImage(const char* path);
	private:
		gui::ImageneerDataSingleton* mDataSingletonInstance;
		std::mutex mMutex;
	};
}
