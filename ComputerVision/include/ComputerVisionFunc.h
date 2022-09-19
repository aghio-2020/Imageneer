#pragma once

#include <memory>

#include "ImageneerData.h"


class ComputerVisionFunc
{
public:
	ComputerVisionFunc();
	~ComputerVisionFunc();
	void OpenCamera();
	void OpenCameraWithFaceDetection();
	bool IsCameraOpened();
	void StopShowingCamera();
	void UpdateTmpFile();
	void SaveImage(const char* path);

	void DetectFaces();
	void ToggleGrayscale();
	void Blur();
private:
	struct CVData;
	std::unique_ptr<CVData> mCVData;
	gui::ImageneerDataSingleton* mDataSingletonInstance;
	std::mutex mMutex;
};

