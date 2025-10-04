#pragma once
#include "DX.h"
#include "Base/NumberGenerator.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXResourceStorage
	{
	public:
		CDXResourceStorage(size_t InResourceCapacity)
		{
			Resources.resize(InResourceCapacity, nullptr);
		}
		~CDXResourceStorage() = default;

	public:
		[[nodiscard]] size_t InsertResource(ComPtr<ID3D11DeviceChild> InResource)
		{
			size_t ResourceHandle = HandleGenerator.GenerateNumber();
			Resources[ResourceHandle] = InResource;
			return ResourceHandle;
		}
		void EraseResource(size_t InResourceHandle)
		{
			Resources[InResourceHandle] = nullptr;
			HandleGenerator.ReleaseNumber(InResourceHandle);
		}

		template <typename T>
		T* GetResource(size_t InResourceId)
		{
			assert(Resources.size() > InResourceId);
			return static_cast<T*>(Resources[InResourceId].Get());
		}

	private:
		std::vector<ComPtr<ID3D11DeviceChild>> Resources;
		CNumberGenerator HandleGenerator;

	};
}

