#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ComputerVisionFunc.h"

#include <iostream>
#include <csignal>

namespace cvFunc
{
	const char* kCameraWindowName = "Camera Display";
	bool showCamera;

	//COULD: make handleImg() to handle all the image processing in one thread

	void showImage(const char *filepath)
	{
		cv::Mat img = cv::imread(filepath);
		cv::imshow("Image", img);
		cv::waitKey(0);
	}

	void closeCameraSignalHandler(int sigint)
	{
		showCamera = false;
	}

	//COULD: make handleCamera() to deal with all the camera posibilities in the thread

	void openCamera()
	{
		showCamera = true;

		cv::Mat image;

		cv::VideoCapture capture(0);

		cv::namedWindow(kCameraWindowName);

		if (!capture.isOpened()) 
		{
			//TODO: logging system
			std::cout << "cannot open camera";
		}

		//TODO: add another signaling/messaging system more adecuate for this app
		std::signal(SIGINT, closeCameraSignalHandler);

		while (showCamera) 
		{
			capture >> image;
			cv::imshow(kCameraWindowName, image);
			cv::waitKey(25);
		}
		
		cv::destroyWindow(kCameraWindowName);
	}
}
