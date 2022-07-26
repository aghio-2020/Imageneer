#include "ComputerVisionFunc.h"

#include <iostream>
#include <filesystem>
#include <cstdio>

#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)
#pragma warning(disable : 6294)
#pragma warning(disable : 6201)
#pragma warning(disable : 6262)
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#pragma warning(default : 26812)
#pragma warning(default : 26495)
#pragma warning(default : 26451)
#pragma warning(default : 6294)
#pragma warning(default : 6201)
#pragma warning(default : 6262)


const char* kCameraWindowName = "Camera Display";
const char* kHaarResourcePath = "Resources/haarcascade_frontalface_default.xml";
const char* kHaarDebugResourcePath = "../../Resources/haarcascade_frontalface_default.xml";

struct ComputerVisionFunc::CVData
{
	cv::Mat mTmpImage;
	cv::CascadeClassifier mFaceCascade;

	CVData()
	{
		//TODO: find a better way to load the correct path
		if (!mFaceCascade.load(kHaarResourcePath) && !mFaceCascade.load(kHaarDebugResourcePath))
		{
			std::cout << "XML cannot be readed" << std::endl;
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

void ComputerVisionFunc::SaveImage(const char* path)
{
	cv::imwrite(path, mCVData->mTmpImage);
	mDataSingletonInstance->SetImageDataFilePath(path);
	mDataSingletonInstance->UpdateTmpFileData();
}

//SHOULD: make a class for each of the things displayed that derive from a base class Display
//TODO: allow user to take pictures with camera and save them
void ComputerVisionFunc::OpenCameraWithFaceDetection()
{
	cv::VideoCapture capture(0);

	if (!capture.isOpened()) 
	{
		std::cout << "cannot open camera" << std::endl;
		return;
	}

	cv::namedWindow(kCameraWindowName);
	mDataSingletonInstance->SetShowCameraView(true);
	cv::Mat image;
	std::vector<cv::Rect> faces;
	cv::CascadeClassifier faceCascade;
	
	if (!faceCascade.load(kHaarResourcePath) && !faceCascade.load(kHaarDebugResourcePath))
	{
		std::cout << "XML cannot be readed" << std::endl;
	}

	while (mDataSingletonInstance->GetShowCameraView())
	{
		capture >> image;
		if (image.empty() || !capture.isOpened())
		{
			mDataSingletonInstance->SetShowCameraView(false);
			return;
		}
		faceCascade.detectMultiScale(image, faces, 1.1, 10);
		for (int i = 0; i < faces.size(); i++)
		{
			cv::rectangle(image, faces[i].tl(), faces[i].br(), cv::Scalar(10, 255, 10), 10);
		}
		cv::imshow(kCameraWindowName, image);
		image.release();
		cv::waitKey(100);
	}
		
	capture.release();
	faceCascade.~CascadeClassifier();
	cv::destroyWindow(kCameraWindowName);
}

void ComputerVisionFunc::OpenCamera()
{
	cv::VideoCapture capture(0);

	if (!capture.isOpened())
	{
		std::cout << "cannot open camera" << std::endl;
		return;
	}

	cv::namedWindow(kCameraWindowName);
	mDataSingletonInstance->SetShowCameraView(true);
	cv::Mat image;

	while (mDataSingletonInstance->GetShowCameraView())
	{
		capture >> image;
		if (image.empty() || !capture.isOpened())
		{
			mDataSingletonInstance->SetShowCameraView(false);
			return;
		}
		cv::imshow(kCameraWindowName, image);
		image.release();
		cv::waitKey(10);
	}

	image.release();
	capture.release();
	cv::destroyWindow(kCameraWindowName);
}

void ComputerVisionFunc::ToggleGrayscale()
{
	cv::Mat tmp = std::move(mCVData->mTmpImage);
	if (!mDataSingletonInstance->GetEffectsDataGrayscaleApplied())
	{
		cv::cvtColor(tmp, mCVData->mTmpImage, cv::COLOR_BGR2GRAY);
		mDataSingletonInstance->SetEffectsDataGrayscaleApplied(true);
	}
	else
	{
		cv::cvtColor(tmp, mCVData->mTmpImage, cv::COLOR_GRAY2BGR);
		mDataSingletonInstance->SetEffectsDataGrayscaleApplied(false);
	}
	cv::imwrite(mDataSingletonInstance->GetTmpFilePath(), mCVData->mTmpImage);
	mDataSingletonInstance->SetUpdateImageTexture(true);
}

void ComputerVisionFunc::DetectFaces()
{
	std::vector<cv::Rect> faces;
	mCVData->mFaceCascade.detectMultiScale(mCVData->mTmpImage, faces, 1.1, 10);
	for (int i = 0; i < faces.size(); i++)
	{
		cv::rectangle(mCVData->mTmpImage, faces[i].tl(), faces[i].br(), cv::Scalar(10, 255, 10), 10);
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


