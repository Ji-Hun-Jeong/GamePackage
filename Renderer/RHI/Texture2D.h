#pragma once
#include "RHI.h"

namespace Graphics
{
	struct TTexture2DDesc
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t MipLevels;
		uint32_t ArraySize;
		EGIFormat Format;
		GISampleDesc SampleDesc;
		EUsage Usage;
		uint32_t BindFlags;
		uint32_t CPUAccessFlags;
		uint32_t MiscFlags;
	};

	class CTexture2D : public CRHI
	{
	public:
		CTexture2D(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource);
		~CTexture2D();

	public:
		uint32_t GetRHIType() override
		{
			return StaticType();
		}
		static uint32_t StaticType()
		{
			static uint32_t RHIType = sRHIType++;
			return RHIType;
		}

	private:


	};
}


