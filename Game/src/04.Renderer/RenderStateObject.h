#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/RHI/Buffer.h>
#include "00.App/CoreSystem.h"
#include "Image.h"
#include "PSOManager.h"

class CBufferMappingInstance
{
	friend class CRenderStateObject;
public:
	CBufferMappingInstance(size_t InByteWidth, std::unique_ptr<Graphics::CBuffer> InGpuBuffer)
		: GpuBuffer(std::move(InGpuBuffer))
		, bUpdated(false)
	{
		CpuBuffer.resize(InByteWidth, 0);
	}
	~CBufferMappingInstance() = default;

private:
	std::vector<uint8_t> CpuBuffer;
	std::unique_ptr<Graphics::CBuffer> GpuBuffer;
	bool bUpdated;

};

class CRenderStateObject
{
public:
	CRenderStateObject()
		: Mesh(nullptr)
		, PSO(nullptr)
		, VertexShaderResources{}
		, PixelShaderResources{}
		, RenderLayer(0)
		, bRender(true)
	{
	}
	~CRenderStateObject() = default;

public:
	void SetMesh(Graphics::CMesh* InMeshOrNull)
	{
		Mesh = InMeshOrNull;
	}
	void SetPixelShaderResource(uint8_t InPixelShaderResourceSlot, CImage* InImage)
	{
		assert(InPixelShaderResourceSlot < 12);
		PixelShaderResources[InPixelShaderResourceSlot] = InImage;
	}
	void SetVertexShaderResource(uint8_t InVertexShaderResourceSlot, CImage* InImage)
	{
		// 구현해야함
		assert(0);
		assert(InImage);
		VertexShaderResources[InVertexShaderResourceSlot] = InImage;
	}
	void SetPSO(CPSO* InPSO)
	{
		assert(InPSO);
		PSO = InPSO;
	}

	void SetVertexConstBuffer(size_t InSlot, std::unique_ptr<CBufferMappingInstance> InVertexConstBufferMappingInstance)
	{
		if (VertexConstBufferMappingInstance[InSlot] != nullptr)
		{
			VertexConstBufferMappingInstance[InSlot].reset();
			std::cout << "VertexConstBuffer Is Overlapped\n";
		}
		VertexConstBufferMappingInstance[InSlot] = std::move(InVertexConstBufferMappingInstance);
	}

	void SetPixelConstBuffer(size_t InSlot, std::unique_ptr<CBufferMappingInstance> InPixelConstBufferMappingInstance)
	{
		if (PixelConstBufferMappingInstance[InSlot] != nullptr)
		{
			PixelConstBufferMappingInstance[InSlot].reset();
			std::cout << "PixelConstBufferMappingInstance Is Overlapped\n";
		}
		PixelConstBufferMappingInstance[InSlot] = std::move(InPixelConstBufferMappingInstance);
	}

	void UpdateVertexConstMappingInstance(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		auto& MappingInstance = VertexConstBufferMappingInstance[InSlot];
		assert(MappingInstance);

		memcpy(MappingInstance->CpuBuffer.data(), InMappingPoint, InByteWidth);
		MappingInstance->bUpdated = true;
	}

	void UpdatePixelConstMappingInstance(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		auto& MappingInstance = PixelConstBufferMappingInstance[InSlot];
		assert(MappingInstance);

		memcpy(MappingInstance->CpuBuffer.data(), InMappingPoint, InByteWidth);
		MappingInstance->bUpdated = true;
	}

	void MapBuffersOnUpdated(Graphics::CRenderContext& InContext);
	void BindRenderState(Graphics::CRenderContext& InContext);

	void SetRenderLayer(uint32_t InLayer) { RenderLayer = InLayer; }
	void SetRender(bool bInRender) { bRender = bInRender; }
	uint32_t GetRenderLayer() const { return RenderLayer; }

	bool IsExistVertexConstBufferInSlot(size_t InSlot)
	{
		assert(InSlot < VertexConstBufferMappingInstance.size());
		if (VertexConstBufferMappingInstance[InSlot])
			return true;
		return false;
	}
	bool IsExistPixelConstBufferInSlot(size_t InSlot)
	{
		assert(InSlot < PixelConstBufferMappingInstance.size());
		if (PixelConstBufferMappingInstance[InSlot])
			return true;
		return false;
	}

protected:
	Graphics::CMesh* Mesh;
	CPSO* PSO;

	std::array<CImage*, 6> VertexShaderResources;
	std::array<CImage*, 6> PixelShaderResources;

	std::array<std::unique_ptr<CBufferMappingInstance>, 6> VertexConstBufferMappingInstance;
	std::array<std::unique_ptr<CBufferMappingInstance>, 6> PixelConstBufferMappingInstance;

	uint32_t RenderLayer;

	bool bRender;
};