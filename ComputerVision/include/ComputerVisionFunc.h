#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
		void SaveImage(const char *path);
	private:
		struct ComputerVisionFuncData;
		std::unique_ptr<ComputerVisionFuncData> mData;
	};
}
