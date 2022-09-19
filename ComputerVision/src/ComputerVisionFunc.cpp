#include "ComputerVisionFunc.h"

#include <iostream>
#include <filesystem>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)
#pragma warning(disable : 6294)
#pragma warning(disable : 6201)
#pragma warning(disable : 6262) // disable warning 4345
#endif
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#ifdef _MSC_VER
#pragma warning(default : 26812)
#pragma warning(default : 26495)
#pragma warning(default : 26451)
#pragma warning(default : 6294)
#pragma warning(default : 6201)
#pragma warning(default : 6262) // enable warning 4345 back
#endif


const char* kCameraWindowName = "Camera Display";

struct ComputerVisionFunc::CVData
{
	cv::Mat mTmpImage;
	cv::CascadeClassifier mFaceCascade;
	std::vector<cv::Rect> mFaces;

	CVData()
	{
		mFaceCascade.load("../../Resources/haarcascade_frontalface_default.xml");
		if (mFaceCascade.empty())
		{
			std::cout << "XML not loaded\n";
		}
	}
};

ComputerVisionFunc::~ComputerVisionFunc() = default;

ComputerVisionFunc::ComputerVisionFunc()
{
	mDataSingletonInstance = gui::ImageneerDataSingleton::Instance();
	mCVData = std::make_unique<ComputerVisionFunc::CVData>();
}

bool ComputerVisionFunc::IsCameraOpened()
{
	return mDataSingletonInstance->GetShowCameraView();
}

void ComputerVisionFunc::StopShowingCamera()
{
	mDataSingletonInstance->SetShowCameraView(false);
}

void ComputerVisionFunc::UpdateTmpFile()
{
	mCVData->mTmpImage = cv::imread(mDataSingletonInstance->GetImageDataFilePath(), cv::IMREAD_ANYCOLOR);
	cv::imwrite(mDataSingletonInstance->GetTmpFilePath(), mCVData->mTmpImage);
	std::cout << mDataSingletonInstance->GetImageDataFilePath() << std::endl;
}

//TODO: handle changes to notify imgui that it should reload the image into RAM
//TODO: save file in new filepath and update all the data for the file
void ComputerVisionFunc::SaveImage(const char* path)
{
	cv::imwrite(path, mCVData->mTmpImage);
	mDataSingletonInstance->SetImageDataFilePath(path);
	mDataSingletonInstance->UpdateTmpFileData();
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
		
	capture.release();
	cv::destroyWindow(kCameraWindowName);
}

void ComputerVisionFunc::Grayscale()
{
	cv::Mat tmp = std::move(mCVData->mTmpImage);
	cv::cvtColor(tmp, mCVData->mTmpImage, cv::COLOR_BGR2GRAY);
	cv::imwrite(mDataSingletonInstance->GetTmpFilePath(), mCVData->mTmpImage);
	mDataSingletonInstance->SetUpdateImageTexture(true);
}

void ComputerVisionFunc::DetectFaces()
{
	mCVData->mFaceCascade.detectMultiScale(mCVData->mTmpImage, mCVData->mFaces, 1.1, 10);
	for (int i = 0; i < mCVData->mFaces.size(); i++)
	{
		cv::rectangle(mCVData->mTmpImage, mCVData->mFaces[i].tl(), mCVData->mFaces[i].br(), cv::Scalar(10, 255, 10), 10);
	}
	cv::imwrite(mDataSingletonInstance->GetTmpFilePath(), mCVData->mTmpImage);
	mDataSingletonInstance->SetUpdateImageTexture(true);
}

void ComputerVisionFunc::Blur()
{
	int sigma = 3;
	int ksize = (sigma * 5) | 1;
	cv::Mat tmp = std::move(mCVData->mTmpImage);
	cv::GaussianBlur(tmp, mCVData->mTmpImage, cv::Size(ksize, ksize), sigma, sigma);
	cv::imwrite(mDataSingletonInstance->GetTmpFilePath(), mCVData->mTmpImage);
	mDataSingletonInstance->SetUpdateImageTexture(true);
}


