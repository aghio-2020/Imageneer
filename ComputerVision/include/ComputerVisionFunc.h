#pragma once

namespace img
{
	class ComputerVisionFunc
	{
	public:
		ComputerVisionFunc() = default;
		void openCamera();
		void showImage(const char* filepath);
	};
}
