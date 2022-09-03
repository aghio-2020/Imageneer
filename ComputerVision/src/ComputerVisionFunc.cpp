#include "ComputerVisionFunc.h"

#include <iostream>
#include <csignal>
#include <memory>

namespace cvFunc
{
	const char* kCameraWindowName = "Camera Display";

	struct ComputerVisionFunc::ComputerVisionFuncData
	{
		bool mShowCamera;
		std::mutex mMutex;
		cv::Mat mTmpImage;
	};

	ComputerVisionFunc::~ComputerVisionFunc() = default;

	ComputerVisionFunc::ComputerVisionFunc() : mData(std::make_unique<ComputerVisionFuncData>()) {}

	bool ComputerVisionFunc::IsCameraOpened()
	{
		return mData->mShowCamera;
	}

	void ComputerVisionFunc::StopShowingCamera()
	{
		mData->mMutex.lock();
		mData->mShowCamera = false;
		mData->mMutex.unlock();
	}

	void ComputerVisionFunc::SaveImage(const char *path)
	{
		cv::imwrite(path, mData->mTmpImage);
	}

	void ComputerVisionFunc::HandleCamera(cv::VideoCapture& capture, cv::Mat& image)
	{
		//add Effects Conditions Here
		capture >> image;
		cv::imshow(kCameraWindowName, image);
		cv::waitKey(25);
	}

	void ComputerVisionFunc::OpenCamera()
	{
		cv::Mat image;
		cv::VideoCapture capture(0);

		if (!capture.isOpened()) 
		{
			std::cout << "cannot open camera\n";
			return;
		}

		cv::namedWindow(kCameraWindowName);

		mData->mShowCamera = true;

		//TODO: add another signaling/messaging system more adecuate for this app

		while (mData->mShowCamera)
		{
			HandleCamera(capture, image);
		}
		
		cv::destroyWindow(kCameraWindowName);
	}
}
