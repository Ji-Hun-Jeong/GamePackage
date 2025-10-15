#pragma once
#include <functional>
#include "RenderCommon.h"

namespace Graphics
{
	// Rendering Hardware Interface
	class CRHI
	{
	public:
		CRHI(size_t InRHIHandle, std::function<void(size_t)> InEventReleaseResource)
			: RHIHandle(InRHIHandle)
			, EventReleaseResource(InEventReleaseResource)
		{}
		virtual ~CRHI() = 0
		{
			EventReleaseResource(RHIHandle);
		}

	public:
		size_t GetResourceHandle() const { return RHIHandle; }
		virtual uint32_t GetRHIType() = 0;

	private:
		size_t RHIHandle;
		std::function<void(size_t)> EventReleaseResource;

	protected:
		static uint32_t sRHIType;
	};
}