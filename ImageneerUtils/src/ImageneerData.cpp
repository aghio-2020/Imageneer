#include "ImageneerData.h"


namespace gui
{

    ImageneerDataSingleton::ImageneerDataSingleton()
    {
        mMainWindowWidth = 0;
        mMainWindowHeight = 0;
        mShowImageView = false;
        mShowCameraView = false;
        mShowEffectsWindow = false;
    }

    const int &ImageneerDataSingleton::GetMainWindowWidth()
    {
        return mMainWindowWidth;
    }
    const int &ImageneerDataSingleton::GetMainWindowHeight()
    {
        return mMainWindowHeight;
    }
    const bool &ImageneerDataSingleton::GetShowImageView()
    {
        return mShowImageView;
    }
    const bool &ImageneerDataSingleton::GetShowCameraView()
    {
        return mShowCameraView;
    }
    const bool &ImageneerDataSingleton::GetShowEffectsWindow()
    {
        return mShowEffectsWindow;
    }

    ImageData &ImageneerDataSingleton::GetImageDataReference()
    {
        return mImageData;
    }

    void ImageneerDataSingleton::SetMainWindowWidth(const int& width)
    {
        mMutex.lock();
        mMainWindowWidth = width;
        mMutex.unlock();
    }
    void ImageneerDataSingleton::SetMainWindowHeight(const int& height)
    {
        mMutex.lock();
        mMainWindowHeight = height;
        mMutex.unlock();
    }
    void ImageneerDataSingleton::SetShowImageView(const bool& showImage)
    {
        mMutex.lock();
        mShowImageView = showImage;
        mMutex.unlock();
    }
    void ImageneerDataSingleton::SetShowCameraView(const bool& showCamera)
    {
        mMutex.lock();
        mShowCameraView = showCamera;
        mMutex.unlock();
    }
    void ImageneerDataSingleton::SetShowEffectsWindow(const bool& showEffectsWindow)
    {
        mMutex.lock();
        mShowEffectsWindow = showEffectsWindow;
        mMutex.unlock();
    }
}