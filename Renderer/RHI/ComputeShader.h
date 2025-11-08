#pragma once
#include "RHI.h"

namespace Graphics
{
	class CComputeShader : public CRHI
	{
	public:
		CComputeShader(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
		{}
		~CComputeShader() = default;

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


