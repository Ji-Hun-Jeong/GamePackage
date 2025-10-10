#pragma once
#include <functional>
#include "RenderCommon.h"

namespace Graphics
{
	// Rendering Hardware Interface
	class CRHI
	{
	public:
		CRHI(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: ResourceHandle(InResourceHandle)
			, EventReleaseResource(InEventReleaseResource)
		{}
		virtual ~CRHI() = 0
		{
			EventReleaseResource(ResourceHandle);
		}

	public:
		size_t GetResourceHandle() const { return ResourceHandle; }
		virtual uint32_t GetRHIType() = 0;

	private:
		size_t ResourceHandle;
		std::function<void(size_t)> EventReleaseResource;

	protected:
		static uint32_t sRHIType;
	};
}