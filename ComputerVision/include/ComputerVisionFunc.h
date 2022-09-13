#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)
#pragma warning(disable : 6294)
#pragma warning(disable : 6201)
#pragma warning(disable : 6262) // disable warning 4345
#endif
	#include <opencv2/imgcodecs.hpp>
	#include <opencv2/highgui.hpp>
	#include <opencv2/imgproc.hpp>
#ifdef _MSC_VER
#pragma warning(default : 26812)
#pragma warning(default : 26495)
#pragma warning(default : 26451)
#pragma warning(default : 6294)
#pragma warning(default : 6201)
#pragma warning(default : 6262) // enable warning 4345 back
#endif

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
