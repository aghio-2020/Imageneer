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

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <utility>

namespace gui
{
    const unsigned int kMaxTmpFileNameSize = 30;
    const char* kTmpFileName = "tmp";

    //tmp image data is shared through out the program and used to decide what to show
    struct ImageneerDataSingleton::ImageData
    {
        //TODO: make thread safe getters and setters
        int mWidth;
        int mHeight;
        GLuint mTexture;
        char* mFilePath;
        char* mFileType;
        char* mTmpFileName;
        bool mLoaded = false;

        void Clear() { mWidth, mHeight, mTexture = 0; mLoaded = false; mFilePath = nullptr; }
    };

    struct ImageneerDataSingleton::EffectsData
    {
        int mBlurRatio;
    };

    ImageneerDataSingleton::ImageneerDataSingleton()
    {
        mImageData = std::make_unique<ImageneerDataSingleton::ImageData>();
        mEffectsData = std::make_unique<ImageneerDataSingleton::EffectsData>();
        mMainWindowWidth = 0;
        mMainWindowHeight = 0;
        mShowImageView = false;
        mShowCameraView = false;
        mShowEffectsWindow = false;
        mUpdateImageTexture = false;
    }

    ImageneerDataSingleton* ImageneerDataSingleton::Instance()
    {
        static ImageneerDataSingleton* mInstance = new ImageneerDataSingleton;
        return mInstance;
    }


    //the program works over the tmp file that contains all the changes
    const char* ImageneerDataSingleton::GetTmpFilePath()
    {
        return mImageData->mTmpFileName;
    }

    //TODO: should work with strings instead of char*
    void ImageneerDataSingleton::UpdateTmpFileData()
    {
        if (!mImageData->mTmpFileName)
        {
            mImageData->mTmpFileName = new char[kMaxTmpFileNameSize];
        }
        std::string tmpfile = mImageData->mFilePath;
        size_t pos;
        pos = tmpfile.rfind(".");
        std::string typestr = tmpfile.substr(pos);
        char* filetype = new char[typestr.length() + 1];
        std::strcpy(filetype, typestr.c_str());
        mImageData->mFileType = filetype;
        std::strcpy(mImageData->mTmpFileName, kTmpFileName);
        std::strcat(mImageData->mTmpFileName, filetype);
        delete [] filetype;
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
    const bool& ImageneerDataSingleton::GetUpdateImageTexture()
    {
        return mUpdateImageTexture;
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
    void ImageneerDataSingleton::SetUpdateImageTexture(const bool& updateImageTexture)
    {
        mMutex.lock();
        mUpdateImageTexture = updateImageTexture;
        mMutex.unlock();
    }

    //ImageData

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
    const char* ImageneerDataSingleton::GetImageDataFileType()
    {
        return mImageData->mFileType;
    }
    const bool& ImageneerDataSingleton::GetImageDataLoaded()
    {
        return mImageData->mLoaded;
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
    void ImageneerDataSingleton::SetImageDataFilePath(const char *filePath)
    {
        char* tmp = new char[strlen(filePath) + 1];
        std::strcpy(tmp, filePath);
        mImageData->mFilePath = tmp;
        delete[] tmp;
    }
    void ImageneerDataSingleton::SetImageDataFileType(const char *type)
    {
        char* tmp = new char[strlen(type) + 1];
        std::strcpy(tmp, type);
        mImageData->mFilePath = tmp;
        delete[] tmp;
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