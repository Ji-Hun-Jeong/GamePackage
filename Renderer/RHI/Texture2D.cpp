#include "pch.h"
#include "Texture2D.h"

namespace Graphics
{
    CTexture2D::CTexture2D(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
        : CRHI(InResourceHandle, InEventReleaseResource)
    {}
    CTexture2D::~CTexture2D()
    {
    }
}
