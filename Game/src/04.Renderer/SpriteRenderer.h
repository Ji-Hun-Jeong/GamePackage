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
	void SetViewInfo(const Vector3& InWorldPosition, const Vector3& InRotation, const Vector3& InScale, bool bInUseScreenSize = true);
	uint32_t GetScreenWidth() const { return ViewData.ScreenWidth; }
	uint32_t GetScreenHeight() const { return ViewData.ScreenHeight; }
	const Vector3& GetRendererWorldPosition() const { return RendererPosition; }

	void DrawMesh(Graphics::CMesh& InMesh, const Vector3& InWorldPosition, const Vector3& InRotation, const Vector3& InScale
		, CPSO* InPSO, Graphics::CMaterial& InMaterial, uint32_t InRenderLayer = 0)
	{
		RenderStates[CurrentRenderState].Mesh = &InMesh;
		RenderStates[CurrentRenderState].Position = InWorldPosition;
		RenderStates[CurrentRenderState].Rotation = InRotation;
		RenderStates[CurrentRenderState].Scale = InScale;

		RenderStates[CurrentRenderState].PSO = InPSO;
		RenderStates[CurrentRenderState].Material = &InMaterial;
		RenderStates[CurrentRenderState].RenderLayer = InRenderLayer;

	}
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
	void SetInstanceData(EShaderType InShaderType, size_t InSlot, Graphics::CBuffer& InBuffer)
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
	}
	void SetPass(ERenderPass InRenderPass)
	{
		if (bDefineState)
			RenderStates[CurrentRenderState].RenderPass = InRenderPass;
	}
	void UpdateBuffer(Graphics::CBuffer& InBuffer, const void* InMappingPoint, size_t InByteWidth)
	{
		Context.UpLoadBuffer(InBuffer, InMappingPoint, InByteWidth, Graphics::EMapType::MAP_WRITE_DISCARD);
	}
	Graphics::CBuffer& EntrustBuffer(const void* InMappingPoint, size_t InByteWidth)
	{
		std::unique_ptr<Graphics::CBuffer> Buffer = CRenderResourceLoader::GetInst().CreateConstBuffer(InByteWidth);
		Graphics::CBuffer& RawBuffer = *Buffer.get();
		UpdateBuffer(*Buffer.get(), InMappingPoint, InByteWidth);
		DisposableBuffers.push_back(std::move(Buffer));

		return RawBuffer;
	}
	void Draw()
	{
		std::vector<TRenderState> CulledRenderStates;
		CulledRenderStates.reserve(CurrentRenderState);
		for (size_t i = 0; i < CurrentRenderState; ++i)
		{
			TRenderState& RenderState = RenderStates[i];
			if (IsInsideRenderer(RenderState.Position, RenderState.Scale) == false)
				continue;

			CulledRenderStates.push_back(RenderState);
		}

		std::stable_sort(CulledRenderStates.begin(), CulledRenderStates.begin() + CulledRenderStates.size(), [](const TRenderState& InA, const TRenderState& InB)->bool
			{
				if (InA.RenderPass != InB.RenderPass)
					return InA.RenderPass < InB.RenderPass;
				return InA.RenderLayer < InB.RenderLayer;
			});
		static float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		//// copy buffer
		/*std::vector<Matrix> CpuModelBuffer;
		CpuModelBuffer.reserve(CulledRenderStates.size());
		for (auto& RenderState : CulledRenderStates)
			CpuModelBuffer.push_back(RenderState.Model);

		std::unique_ptr<Graphics::CBuffer> GpuModelBuffer = CRenderResourceLoader::GetInst().CreateStructuredBuffer(sizeof(Matrix), CpuModelBuffer.size());
		std::unique_ptr<Graphics::CShaderResourceView> ModelBufferSRV = CRenderResourceLoader::GetInst().CreateStructuredBufferSRV(*GpuModelBuffer.get(), CpuModelBuffer.size());
		UpdateBuffer(*GpuModelBuffer.get(), CpuModelBuffer.data(), sizeof(Matrix) * CpuModelBuffer.size());

		Context.VSSetShaderResource(0, ModelBufferSRV.get());*/

		ViewData.View = GetNDCMatrix(RendererPosition, RendererRotation, RendererScale).Invert().Transpose();
		UpdateBuffer(*ViewBuffer.get(), &ViewData, sizeof(ViewData));
		Context.VSSetConstantBuffer(1, ViewBuffer.get());
		
		for (size_t i = 0; i < CulledRenderStates.size(); ++i)
		{
			const TRenderState& RenderState = CulledRenderStates[i];

			Matrix Model = GetNDCMatrix(RenderState.Position, RenderState.Rotation, RenderState.Scale).Transpose();
			UpdateBuffer(*ModelBuffer.get(), &Model, sizeof(Model));
			Context.VSSetConstantBuffer(0, ModelBuffer.get());

			DDraw(RenderState);
		}
	}
	void DDraw(const TRenderState& InRenderState)
	{
		for (size_t i = 0; i < InRenderState.VertexConstBuffers.size(); ++i)
		{
			auto& VertexConstBuffer = InRenderState.VertexConstBuffers[i];
			if (VertexConstBuffer)
				Context.VSSetConstantBuffer(uint32_t(i + 2), VertexConstBuffer);
		}
		for (size_t i = 0; i < InRenderState.PixelConstBuffers.size(); ++i)
		{
			auto& PixelConstBuffer = InRenderState.PixelConstBuffers[i];
			if (PixelConstBuffer)
				Context.PSSetConstantBuffer(uint32_t(i), PixelConstBuffer);
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
		Context.PSSetSampler(0, 1, PSO->SamplerState);

		Context.IASetVertexBuffer(&Mesh->GetVertexBuffer(), &Mesh->GetStride(), &Mesh->GetOffset());
		Context.IASetIndexBuffer(&Mesh->GetIndexBuffer(), Mesh->GetIndexFormat(), 0);

		Context.PSSetShader(&InRenderState.Material->GetPixelShader());
		const std::array<const Graphics::CShaderResourceView*, 6>& PixelShaderResources = InRenderState.Material->GetShaderResourceViews();
		for (size_t i = 0; i < PixelShaderResources.size(); ++i)
		{
			auto& PixelShaderResource = PixelShaderResources[i];
			Context.PSSetShaderResource(uint32_t(i), PixelShaderResource);
		}

		Context.DrawIndexed(Mesh->GetIndexCount());

	}
	void Present()
	{
		SwapChain.Present();

		CurrentRenderState = 0;
		memset(RenderStates.data(), 0, sizeof(TRenderState) * RenderStates.size());

		DisposableBuffers.clear();
	}

private:
	bool IsInsideRenderer(const Vector3& InWorldPosition, const Vector3& InScale) const;
	void TransformNDC(TRenderState* InOutRenderState) const;
	Matrix GetNDCMatrix(const Vector3& InWorldPosition, const Vector3& InRotation, const Vector3& InScale)
	{
		const Vector3 NormalizedScale = Vector3(InScale.x / ViewData.ScreenWidth, InScale.y / ViewData.ScreenHeight, InScale.z);
		const Vector3 NormalizedPosition = Vector3(InWorldPosition.x / (ViewData.ScreenWidth * 0.5f), InWorldPosition.y / (ViewData.ScreenHeight * 0.5f), InWorldPosition.z);
		return Matrix::CreateScale(NormalizedScale)
			* Matrix::CreateRotationX(InRotation.x)
			* Matrix::CreateRotationY(InRotation.y)
			* Matrix::CreateRotationZ(InRotation.z)
			* Matrix::CreateTranslation(NormalizedPosition);
	}

private:
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<TRenderState> RenderStates;
	size_t CurrentRenderState = 0;
	bool bDefineState = false;


	std::unique_ptr<Graphics::CBuffer> ModelBuffer;
	std::unique_ptr<Graphics::CBuffer> ViewBuffer;
	Vector3 RendererPosition;
	Vector3 RendererRotation;
	Vector3 RendererScale;
	struct TViewData
	{
		Matrix View;
		uint32_t ScreenWidth = 0;
		uint32_t ScreenHeight = 0;
		uint32_t Dummy[2];
	} ViewData;
	static_assert(sizeof(ViewData) % 16 == 0);

	std::vector<std::unique_ptr<Graphics::CBuffer>> DisposableBuffers;

};