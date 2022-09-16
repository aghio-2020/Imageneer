#pragma once

#include <memory>

#include "ImageneerData.h"


class ComputerVisionFunc
{
public:
	ComputerVisionFunc();
	~ComputerVisionFunc();
	void OpenCamera();
	bool IsCameraOpened();
	void StopShowingCamera();
	void SwapTmpFile();
	void SaveImage(const char* path);

	void Grayscale();
	void Blur();
private:
	struct CVData;
	std::unique_ptr<CVData> mCVData;
	gui::ImageneerDataSingleton* mDataSingletonInstance;
	std::mutex mMutex;
};

