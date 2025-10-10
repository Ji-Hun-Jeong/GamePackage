#pragma once
#include "RHI.h"

namespace Graphics
{
	class CDepthStencilView : public CRHI
	{
	public:
		CDepthStencilView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
		{}
		~CDepthStencilView() = default;

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


