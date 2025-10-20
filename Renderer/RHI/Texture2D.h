#pragma once
#include "RHI.h"

namespace Graphics
{
	struct TTexture2DDesc
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t MipLevels = 0;
		uint32_t ArraySize = 0;
		EGIFormat Format = EGIFormat::GI_FORMAT_UNKNOWN;
		GISampleDesc SampleDesc;
		EUsage Usage = EUsage::UsageDefault;
		uint32_t BindFlags = 0;
		uint32_t CPUAccessFlags = 0;
		uint32_t MiscFlags = 0;
	};

	class CTexture2D : public CRHI
	{
	public:
		CTexture2D(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource, const TTexture2DDesc& InTexture2DDesc)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, Texture2DDesc(InTexture2DDesc)
			, TextureSize(0.0f)
		{
			TextureSize = Vector2(float(Texture2DDesc.Width), float(Texture2DDesc.Height));
		}
		~CTexture2D() = default;

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
		const TTexture2DDesc& GetTexture2DDesc() const { return Texture2DDesc; }
		const Vector2& GetTextureSize() const { return TextureSize; }
	private:
		TTexture2DDesc Texture2DDesc;
		Vector2 TextureSize;

	};
}


