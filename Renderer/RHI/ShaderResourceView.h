#pragma once
#include "RHI.h"
#include "Texture2D.h"

namespace Graphics
{
	class CShaderResourceView : public CRHI
	{
	public:
		CShaderResourceView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource
			, std::unique_ptr<CTexture2D> InTexture2D)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, Texture2D(std::move(InTexture2D))
		{}
		~CShaderResourceView() {}

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
		const CTexture2D& GetTexture2D() const { return *Texture2D.get(); }

	private:
		std::unique_ptr<CTexture2D> Texture2D;

	};
}


