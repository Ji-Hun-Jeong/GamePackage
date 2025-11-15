#pragma once
#include <Renderer/Base/GraphicInfra.h>
#include "RenderResourceLoader.h"
#include "RenderStateObject.h"

namespace Core
{
	class CWindow;
}

class CSpriteRenderer
{
public:
	CSpriteRenderer(Graphics::IGraphicInfra& InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight);
	~CSpriteRenderer() = default;

public:
	void InitalizeFromWindow(Core::CWindow& InWindow);
	void SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight);

	void UpdateConstBuffer(CRenderStateObject& InRenderStateObject, EShaderType InShaderType, size_t InSlot
		, const void* InMappingPoint, size_t InByteWidth)
	{
		Graphics::CBuffer* Buffer = nullptr;
		if (InRenderStateObject.IsExistConstBufferInSlot(InShaderType, InSlot) == false)
		{
			InRenderStateObject.MountConstBuffer(InShaderType, InSlot
				, RenderResourceLoader.CreateConstBuffer(InByteWidth));
		}

		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			Buffer = InRenderStateObject.VertexConstBuffers[InSlot].get();
			break;
		case EShaderType::PixelShader:
			Buffer = InRenderStateObject.PixelConstBuffers[InSlot].get();
			break;
		default:
			break;
		}

		assert(Buffer);
		Context.UpLoadBuffer(*Buffer, InMappingPoint, InByteWidth, Graphics::EMapType::MAP_WRITE_DISCARD);
	}
	void SetMeshToRSO(CRenderStateObject& InRenderStateObject, const Graphics::TMeshData& InMeshData)
	{
		Graphics::CMesh* Mesh = RenderResourceLoader.LoadMesh(InMeshData);
		InRenderStateObject.MountMesh(Mesh);
	}
	CImage* SetShaderResourceToRSOByImagePath(CRenderStateObject& InRenderStateObject, const std::wstring& InImagePath)
	{
		CImage* Image = RenderResourceLoader.LoadImageFromFile(InImagePath);
		InRenderStateObject.MountShaderResource(EShaderType::PixelShader, 0, &Image->GetSRV());
		return Image;
	}
	void SetPSOToRSO(CRenderStateObject& InRenderStateObject, EPSOType InPSOType)
	{
		CPSO* PSO = PSOManager.GetPSO(InPSOType);
		InRenderStateObject.MountPSO(PSO);
	}
	void RenderObject(CRenderStateObject& InRenderStateObject)
	{
		RenderStateObjects.push_back(&InRenderStateObject);
	}
	void Draw()
	{
		std::sort(RenderStateObjects.begin(), RenderStateObjects.end(), [](CRenderStateObject* InA, CRenderStateObject* InB)->bool
			{
				return InA->GetRenderLayer() < InB->GetRenderLayer();
			});

		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (auto& RenderStateObject : RenderStateObjects)
			DDraw(*RenderStateObject);

		RenderStateObjects.clear();
	}
	void DDraw(const CRenderStateObject& InRenderStateObject)
	{
		for (size_t i = 0; i < InRenderStateObject.VertexConstBuffers.size(); ++i)
		{
			auto& VertexConstBuffer = InRenderStateObject.VertexConstBuffers[i];
			if (VertexConstBuffer == nullptr)
				continue;
			Context.VSSetConstantBuffer(uint32_t(i), VertexConstBuffer.get());
		}
		for (size_t i = 0; i < InRenderStateObject.PixelConstBuffers.size(); ++i)
		{
			auto& PixelConstBuffer = InRenderStateObject.PixelConstBuffers[i];
			Context.PSSetConstantBuffer(uint32_t(i), PixelConstBuffer.get());
		}
		for (size_t i = 0; i < InRenderStateObject.PixelShaderResources.size(); ++i)
		{
			auto& PixelShaderResource = InRenderStateObject.PixelShaderResources[i];
			Context.PSSetShaderResource(uint32_t(i), PixelShaderResource);
		}

		const Graphics::CMesh* Mesh = InRenderStateObject.Mesh;
		const CPSO* PSO = InRenderStateObject.PSO;
		if (Mesh && PSO)
		{
			const float Factor = 1.0f;
			const float BlendFactor[4] = { Factor, Factor, Factor, 0.0f };
			Context.OMSetBlendState(PSO->BlendState, BlendFactor, 0xffffffff);
			Context.IASetPrimitiveTopology(PSO->PrimitiveTopology);
			Context.IASetInputLayout(PSO->InputLayout);
			Context.VSSetShader(PSO->VertexShader);
			Context.RSSetState(PSO->RasterizerState);
			Context.PSSetShader(PSO->PixelShader);
			Context.PSSetSampler(0, 1, PSO->SamplerState);

			Context.IASetVertexBuffer(&Mesh->GetVertexBuffer(), &Mesh->GetStride(), &Mesh->GetOffset());
			Context.IASetIndexBuffer(&Mesh->GetIndexBuffer(), Mesh->GetIndexFormat(), 0);
			Context.DrawIndexed(Mesh->GetIndexCount());
		}
		else
		{
			Context.OMSetBlendState(nullptr, nullptr, 0xffffffff);
			Context.IASetInputLayout(nullptr);
			Context.VSSetShader(nullptr);
			Context.RSSetState(nullptr);
			Context.PSSetShader(nullptr);
			Context.PSSetSampler(0, 1, nullptr);

			uint32_t Stride = 0;
			uint32_t Offset = 0;
			Context.IASetVertexBuffer(nullptr, &Stride, &Offset);
			Context.IASetIndexBuffer(nullptr, Graphics::EGIFormat(0), 0);
		}
	}
	void Present()
	{
		SwapChain.Present();
	}

	const CPSOManager& GetPSOManager() const { return PSOManager; }
	CRenderResourceLoader& GetRenderResourceLoader() { return RenderResourceLoader; }

private:
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	CPSOManager PSOManager;
	CRenderResourceLoader RenderResourceLoader;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<CRenderStateObject*> RenderStateObjects;
};