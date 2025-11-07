#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/RHI/Buffer.h>
#include "00.App/CoreSystem.h"
#include "Image.h"
#include "PSOManager.h"

enum class EShaderType
{
	VertexShader,
	PixelShader,
};

class CRenderStateObject
{
	friend class CSpriteRenderer;
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
	void MountMesh(Graphics::CMesh* InMeshOrNull)
	{
		Mesh = InMeshOrNull;
	}
	void MountShaderResource(EShaderType InShaderType, uint8_t InSlot, const Graphics::CShaderResourceView* InSRV)
	{
		assert(InSlot < VertexShaderResources.size());
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			if (VertexShaderResources[InSlot] != nullptr)
				std::cout << "VertexShaderResources Is Overlapped\n";
			
			VertexShaderResources[InSlot] = InSRV;
			break;
		case EShaderType::PixelShader:
			if (PixelShaderResources[InSlot] != nullptr)
				std::cout << "PixelShaderResources Is Overlapped\n";

			PixelShaderResources[InSlot] = InSRV;
			break;
		default:
			assert(0);
			break;
		}
	}
	void MountPSO(CPSO* InPSO)
	{
		assert(InPSO);
		PSO = InPSO;
	}

	void MountConstBuffer(EShaderType InShaderType, size_t InSlot, std::unique_ptr<Graphics::CBuffer> InConstBuffer)
	{
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			if (VertexConstBuffers[InSlot] != nullptr)
			{
				VertexConstBuffers[InSlot].reset();
				std::cout << "VertexConstBuffer Is Overlapped\n";
			}
			VertexConstBuffers[InSlot] = std::move(InConstBuffer);
			break;
		case EShaderType::PixelShader:
			if (PixelConstBuffers[InSlot] != nullptr)
			{
				PixelConstBuffers[InSlot].reset();
				std::cout << "PixelConstBuffer Is Overlapped\n";
			}
			PixelConstBuffers[InSlot] = std::move(InConstBuffer);
			break;
		default:
			assert(0);
			break;
		}
	}

	void SetRenderLayer(uint32_t InLayer) { RenderLayer = InLayer; }
	void SetRender(bool bInRender) { bRender = bInRender; }
	uint32_t GetRenderLayer() const { return RenderLayer; }

	bool IsExistConstBufferInSlot(EShaderType InShaderType, size_t InSlot)
	{
		assert(InSlot < VertexConstBuffers.size());
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			if (VertexConstBuffers[InSlot])
				return true;
			return false;
			break;
		case EShaderType::PixelShader:
			if (PixelConstBuffers[InSlot])
				return true;
			return false;
			break;
		default:
			assert(0);
			break;
		}
		return false;
	}

protected:
	Graphics::CMesh* Mesh;
	CPSO* PSO;

	std::array<const Graphics::CShaderResourceView*, 6> VertexShaderResources;
	std::array<const Graphics::CShaderResourceView*, 6> PixelShaderResources;

	std::array<std::unique_ptr<Graphics::CBuffer>, 6> VertexConstBuffers;
	std::array<std::unique_ptr<Graphics::CBuffer>, 6> PixelConstBuffers;

	uint32_t RenderLayer;

	bool bRender;
};