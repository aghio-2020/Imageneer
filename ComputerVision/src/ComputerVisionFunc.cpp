#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ComputerVisionFunc.h"

#include <iostream>
#include <csignal>

namespace cvFunc
{
	const char* kCameraWindowName = "Camera Display";

	void showImage(const char *filepath)
	{
		cv::Mat img = cv::imread(filepath);
		cv::imshow("Image", img);
		cv::waitKey(0);
	}

	void closeWindowHandler(int signal)
	{
		cv::destroyWindow(kCameraWindowName);
	}

	void openCamera()
	{
		cv::Mat image;

		cv::namedWindow(kCameraWindowName);

		cv::VideoCapture capture(0);

		if (!capture.isOpened()) 
		{
			//TODO: logging system
			std::cout << "cannot open camera";
		}

		while (true) 
		{
			capture >> image;
			cv::imshow(kCameraWindowName, image);
			cv::waitKey(25);
		}
	}
}
