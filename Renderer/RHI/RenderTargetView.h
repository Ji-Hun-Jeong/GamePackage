#pragma once
#include "RHI.h"
#include "Texture2D.h"

namespace Graphics
{
	class CRenderTargetView : public CRHI
	{
	public:
		CRenderTargetView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource, const CTexture2D& InTexture2D)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, Texture2D(InTexture2D)
		{}
		~CRenderTargetView() = default;

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
		const CTexture2D& GetTexture2D() const { return Texture2D; }
	private:
		const CTexture2D& Texture2D;

	};
}


