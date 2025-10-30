#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/RHI/Buffer.h>
#include "00.App/CoreSystem.h"
#include "Image.h"
#include "PSOManager.h"

class CRenderStateObject
{
public:
	CRenderStateObject()
		: Mesh(nullptr)
		, PSO(nullptr)
		, VertexShaderResources{}
		, PixelShaderResources{}
		, PixelShaderResourceStartSlot(0)
		, VertexConstBufferStartSlot(0)
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
	void SetPixelShaderResourceStartSlot(uint32_t InPixelShaderResourceStartSlot) { PixelShaderResourceStartSlot = InPixelShaderResourceStartSlot; }
	void SetVertexConstBufferStartSlot(uint32_t InVertexConstBufferStartSlot)
	{
		VertexConstBufferStartSlot = InVertexConstBufferStartSlot;
	}

	void SetVertexConstBuffer(size_t InSlot, std::unique_ptr<Graphics::CBuffer> InVertexConstBuffer)
	{
		if (VertexConstBuffer[InSlot] != nullptr)
		{
			VertexConstBuffer[InSlot].reset();
			std::cout << "VertexConstBuffer Is Overlapped\n";
		}
		VertexConstBuffer[InSlot] = std::move(InVertexConstBuffer);
	}

	void UpdateVertexConstBuffer(Graphics::CRenderContext& InContext, size_t InSlot, const void* InMappingPoint, size_t InByteWidth);
	void BindRenderState(Graphics::CRenderContext& InContext);

	void SetRender(bool bInRender) { bRender = bInRender; }

	bool IsExistBufferInSlot(size_t InSlot)
	{
		assert(InSlot < VertexConstBuffer.size());
		if (VertexConstBuffer[InSlot])
			return true;
		return false;
	}
protected:
	Graphics::CMesh* Mesh;
	CPSO* PSO;

	std::array<CImage*, 6> VertexShaderResources;
	std::array<CImage*, 6> PixelShaderResources;

	std::array<std::unique_ptr<Graphics::CBuffer>, 6> VertexConstBuffer;
	std::queue<size_t> UpdateList;

	uint32_t PixelShaderResourceStartSlot;
	uint32_t VertexConstBufferStartSlot;

	bool bRender;
};

struct TBufferMappingInstance
{
	CRenderStateObject& RenderStateObject;
	size_t UpdateSlot;
	const std::vector<uint8_t>& BufferData;
};