#pragma once
#include "RHI.h"
#include "Texture2D.h"

namespace Graphics
{
	class CShaderResourceView : public CRHI
	{
	public:
		CShaderResourceView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
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

	private:


	};
}


