#pragma once
#include "RHI.h"

namespace Graphics
{
	struct TBufferUAV
	{
		UINT FirstElement;
		UINT NumElements;
		UINT Flags;
	};

	struct TUnorderedAccessViewDesc
	{
		DXGI_FORMAT Format;
		D3D11_UAV_DIMENSION ViewDimension;
		TBufferUAV Buffer;
	};

	class CUnorderedAccessView : public CRHI
	{
	public:
		CUnorderedAccessView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
		{}
		~CUnorderedAccessView() = default;

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
	};
}
