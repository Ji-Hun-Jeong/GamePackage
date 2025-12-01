#pragma once
#include <Renderer/Base/GraphicInfra.h>
#include "PSOManager.h"
#include "RenderState.h"
#include "RenderResourceLoader.h"

namespace Core
{
	class CWindow;
}

class CSpriteRenderer
{
public:
	CSpriteRenderer(Graphics::IGraphicInfra& InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight);
	~CSpriteRenderer();

public:
	void InitalizeFromWindow(Core::CWindow& InWindow);
	void SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetCameraOffset(const Vector2& InCameraOffset) { CameraOffset = InCameraOffset; }
	bool IsInsideNDC(const Vector2& InPosition, const Vector2& InScale);

	void StartState()
	{
		bDefineState = true;
		if (RenderStates.capacity() <= CurrentRenderState)
			RenderStates.resize(CurrentRenderState * 2);
	}
	void EndState()
	{
		bDefineState = false;
		CurrentRenderState += 1;
	}
	void SetMesh(Graphics::CMesh& InMesh)
	{
		if (bDefineState)
			RenderStates[CurrentRenderState].Mesh = &InMesh;
	}
	void SetLayer(uint32_t InRenderLayer)
	{
		if (bDefineState)
			RenderStates[CurrentRenderState].RenderLayer = InRenderLayer;
	}
	void SetPipeline(CPSO& InPSO)
	{
		if (bDefineState)
			RenderStates[CurrentRenderState].PSO = &InPSO;
	}
	void SetShaderResourceView(EShaderType InShaderType, size_t InSlot, const Graphics::CShaderResourceView& InShaderResourceView)
	{
		if (bDefineState == false)
			return;
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			RenderStates[CurrentRenderState].VertexShaderResources[InSlot] = &InShaderResourceView;
			break;
		case EShaderType::PixelShader:
			RenderStates[CurrentRenderState].PixelShaderResources[InSlot] = &InShaderResourceView;
			break;
		default:
			break;
		}
	}
	void SetConstBuffer(EShaderType InShaderType, size_t InSlot, Graphics::CBuffer& InBuffer)
	{
		if (bDefineState)
		{
			switch (InShaderType)
			{
			case EShaderType::VertexShader:
				RenderStates[CurrentRenderState].VertexConstBuffers[InSlot] = &InBuffer;
				break;
			case EShaderType::PixelShader:
				RenderStates[CurrentRenderState].PixelConstBuffers[InSlot] = &InBuffer;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (InShaderType)
			{
			case EShaderType::VertexShader:
				GlobalVertexConstBuffers[InSlot] = &InBuffer;
				break;
			case EShaderType::PixelShader:
				GlobalPixelConstBuffers[InSlot] = &InBuffer;
				break;
			default:
				break;
			}
		}
	}
	void UpdateBuffer(Graphics::CBuffer& InBuffer, const void* InMappingPoint, size_t InByteWidth)
	{
		Context.UpLoadBuffer(InBuffer, InMappingPoint, InByteWidth, Graphics::EMapType::MAP_WRITE_DISCARD);
	}
	void Draw()
	{
		std::stable_sort(RenderStates.begin(), RenderStates.begin() + CurrentRenderState, [](const TRenderState& InA, const TRenderState& InB)->bool
			{
				return InA.RenderLayer < InB.RenderLayer;
			});
		static float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (size_t i = 0; i < GlobalVertexConstBuffers.size(); ++i)
		{
			auto& GlobalVertexConstBuffer = GlobalVertexConstBuffers[i];
			Context.VSSetConstantBuffer(uint32_t(i), GlobalVertexConstBuffer);
		}
		for (size_t i = 0; i < GlobalPixelConstBuffers.size(); ++i)
		{
			auto& GlobalPixelConstBuffer = GlobalPixelConstBuffers[i];
			Context.PSSetConstantBuffer(uint32_t(i), GlobalPixelConstBuffer);
		}

		for (size_t i = 0; i < CurrentRenderState; ++i)
		{
			const TRenderState& RenderState = RenderStates[i];
			DDraw(RenderState);
		}
	}
	void DDraw(const TRenderState& InRenderState)
	{
		for (size_t i = 0; i < InRenderState.VertexConstBuffers.size(); ++i)
		{
			auto& VertexConstBuffer = InRenderState.VertexConstBuffers[i];
			if (VertexConstBuffer)
				Context.VSSetConstantBuffer(uint32_t(i), VertexConstBuffer);
		}
		for (size_t i = 0; i < InRenderState.PixelConstBuffers.size(); ++i)
		{
			auto& PixelConstBuffer = InRenderState.PixelConstBuffers[i];
			if (PixelConstBuffer)
				Context.PSSetConstantBuffer(uint32_t(i), PixelConstBuffer);
		}
		
		for (size_t i = 0; i < InRenderState.PixelShaderResources.size(); ++i)
		{
			auto& PixelShaderResource = InRenderState.PixelShaderResources[i];
			Context.PSSetShaderResource(uint32_t(i), PixelShaderResource);
		}

		const Graphics::CMesh* Mesh = InRenderState.Mesh;
		const CPSO* PSO = InRenderState.PSO;

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
	void Present()
	{
		SwapChain.Present();

		CurrentRenderState = 0;
		memset(RenderStates.data(), 0, sizeof(TRenderState) * RenderStates.size());
	}

private:
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<TRenderState> RenderStates;
	size_t CurrentRenderState = 0;
	bool bDefineState = false;

	std::array<Graphics::CBuffer*, 6> GlobalVertexConstBuffers;
	std::array<Graphics::CBuffer*, 6> GlobalPixelConstBuffers;

	uint32_t ScreenWidth = 0;
	uint32_t ScreenHeight = 0;

	Vector2 CameraOffset;
};