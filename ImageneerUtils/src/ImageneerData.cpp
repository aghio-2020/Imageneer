#include "ImageneerData.h"

#ifdef _MSC_VER
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)
#pragma warning(disable : 6294)
#pragma warning(disable : 6201)
#pragma warning(disable : 6262) // disable warning 4345
#endif
#include <opencv2/imgproc.hpp>
#ifdef _MSC_VER
#pragma warning(default : 26812)
#pragma warning(default : 26495)
#pragma warning(default : 26451)
#pragma warning(default : 6294)
#pragma warning(default : 6201)
#pragma warning(default : 6262) // enable warning 4345 back
#endif


namespace gui
{
    //tmp image data is shared through out the program and used to decide what to show
    struct ImageneerDataSingleton::ImageData
    {
        //TODO: make thread safe getters and setters
        int mWidth;
        int mHeight;
        GLuint mTexture;
        char* mFilePath;
        char* mTmpFilePath;
        bool mLoaded = false;

        void Clear() { mWidth, mHeight, mTexture = 0; mLoaded = false; mFilePath = nullptr; }
    };

    ImageneerDataSingleton::ImageneerDataSingleton()
    {
        mMainWindowWidth = 0;
        mMainWindowHeight = 0;
        mShowImageView = false;
        mShowCameraView = false;
        mShowEffectsWindow = false;
        mImageData = std::make_unique<ImageneerDataSingleton::ImageData>();
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

    const int& ImageneerDataSingleton::GetImageDataWidth()
    {
        return mImageData->mWidth;
    }
    const int& ImageneerDataSingleton::GetImageDataHeight()
    {
        return mImageData->mHeight;
    }
    const GLuint& ImageneerDataSingleton::GetImageDataTexture()
    {
        return mImageData->mTexture;
    }
    const char* ImageneerDataSingleton::GetImageDataFilePath()
    {
        return mImageData->mFilePath;
    }
    const bool& ImageneerDataSingleton::GetImageDataLoaded()
    {
        return mImageData->mLoaded;
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

    void ImageneerDataSingleton::SetImageDataWidth(const int &width)
    {
        mImageData->mWidth = width;
    }
    void ImageneerDataSingleton::SetImageDataHeight(const int &height)
    {
        mImageData->mHeight = height;
    }
    void ImageneerDataSingleton::SetImageDataTexture(const GLuint &texture)
    {
        mImageData->mTexture = texture;
    }
    void ImageneerDataSingleton::SetImageDataFilePath(char *filePath)
    {
        mImageData->mFilePath = filePath;
    }
    void ImageneerDataSingleton::SetImageDataLoaded(const bool &loaded)
    {
        mImageData->mLoaded = loaded;
    }
    void ImageneerDataSingleton::ClearImageData()
    {
        mImageData->Clear();
    }
}