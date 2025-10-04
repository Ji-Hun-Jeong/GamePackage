#pragma once
#include <functional>

namespace Graphics
{
	class IResource
	{
		friend class DX::CDXDevice;
		friend class DX::CDXContext;
	public:
		IResource()
			: EventReleaseResource(nullptr)
			, ResourceHandle(0)
		{}
		virtual ~IResource() = 0
		{
			EventReleaseResource(ResourceHandle);
		}

	private:
		void SetResourceHandle(size_t InResourceHandle) { ResourceHandle = InResourceHandle; }
		void SetReleaseResourceEvent(std::function<void(size_t)> InEventReleaseResource)
		{
			EventReleaseResource = InEventReleaseResource;
		}
		size_t GetResourceHandle() const { return ResourceHandle; }

	private:
		std::function<void(size_t)> EventReleaseResource;
		size_t ResourceHandle;

	};
}