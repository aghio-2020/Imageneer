#include "ComputerVisionFunc.h"

#include <iostream>

namespace cvFunc
{
	const char* kCameraWindowName = "Camera Display";

	ComputerVisionFunc::~ComputerVisionFunc() = default;

	ComputerVisionFunc::ComputerVisionFunc()
	{
		mDataSingletonInstance = gui::ImageneerDataSingleton::Instance();
	}

	bool ComputerVisionFunc::IsCameraOpened()
	{
		return mDataSingletonInstance->GetShowCameraView();
	}

	void ComputerVisionFunc::StopShowingCamera()
	{
		mMutex.lock();
		mDataSingletonInstance->SetShowCameraView(false);
		mMutex.unlock();
	}

	void ComputerVisionFunc::StartShowingCamera()
	{
		mMutex.lock();
		mDataSingletonInstance->SetShowCameraView(true);
		mMutex.unlock();
	}

	void ComputerVisionFunc::SetTmpFile(const char* path)
	{
		mDataSingletonInstance->GetImageDataReference().mTmpImage = cv::imread(path);
	}

	//TODO: handle changes to notify imgui that it should reload the image into RAM
	void ComputerVisionFunc::SaveImage(const char* path)
	{
		cv::imwrite(path, mDataSingletonInstance->GetImageDataReference().mTmpImage);
	}

	void ComputerVisionFunc::HandleCamera(cv::VideoCapture& capture)
	{
		cv::Mat image;
		//add Effects Conditions Here
		while (mDataSingletonInstance->GetShowCameraView())
		{
			capture >> image;
			if (image.empty() || !capture.isOpened())
			{
				mDataSingletonInstance->SetShowCameraView(false);
				return;
			}
			cv::imshow(kCameraWindowName, image);
			cv::waitKey(10);
		}
	}

	void ComputerVisionFunc::OpenCamera()
	{
		cv::VideoCapture capture(0);

		if (!capture.isOpened()) 
		{
			std::cout << "cannot open camera\n";
			return;
		}

		cv::namedWindow(kCameraWindowName);

		mDataSingletonInstance->SetShowCameraView(true);

		//TODO: add another signaling/messaging system more adecuate for this app
		HandleCamera(capture);
		
		cv::destroyWindow(kCameraWindowName);
	}
}
