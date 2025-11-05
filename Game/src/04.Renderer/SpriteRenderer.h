#pragma once
#include <Renderer/Base/GraphicInfra.h>
#include "RenderResourceLoader.h"
#include "RenderStateObject.h"

namespace Core
{
	class CWindow;
}

class CRenderCommand
{
	friend class CSpriteRenderer;
public:
	CRenderCommand()
		: RenderLayer(0)
	{}
	~CRenderCommand() = default;
public:
	void ClearCommand()
	{
		Mesh = nullptr;
		PSO = nullptr;
		for (auto& VertexShaderResource : VertexShaderResources)
			VertexShaderResource = nullptr;
		for (auto& PixelShaderResource : PixelShaderResources)
			PixelShaderResource = nullptr;
		for (auto& VertexConstBuffer : VertexConstBuffers)
			VertexConstBuffer = nullptr;
		for (auto& PixelConstBuffer : PixelConstBuffers)
			PixelConstBuffer = nullptr;
	}
private:
	const Graphics::CMesh* Mesh;
	const CPSO* PSO;

	std::array<CImage*, 6> VertexShaderResources;
	std::array<CImage*, 6> PixelShaderResources;

	std::array<Graphics::CBuffer*, 6> VertexConstBuffers;
	std::array<Graphics::CBuffer*, 6> PixelConstBuffers;

public:
	uint32_t RenderLayer;

};
enum class EShaderType
{
	VertexShader,
	PixelShader,
};
class CSpriteRenderer
{
public:
	CSpriteRenderer(std::unique_ptr<Graphics::IGraphicInfra> InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight);
	~CSpriteRenderer() = default;

public:
	void InitalizeFromWindow(Core::CWindow& InWindow);
	void SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight);
	uint32_t GetScreenWidth() const { return ScreenWidth; }
	uint32_t GetScreenHeight() const { return ScreenHeight; }

	void UpdateConstBuffer(Graphics::CBuffer& InConstBuffer, const void* InMappingPoint, size_t InByteWidth)
	{
		Context.CopyBuffer(&InConstBuffer, InMappingPoint, InByteWidth);
	}
	void StartRender()
	{
		CurrentLoadingCommand.ClearCommand();
	}
	void MountMesh(const Graphics::TMeshData& InMeshData, EPSOType InPSOType)
	{
		Graphics::CMesh* Mesh = RenderResourceLoader.LoadMesh(InMeshData);
		CPSO* PSO = PSOManager.GetPSO(InPSOType);

		CurrentLoadingCommand.Mesh = Mesh;
		CurrentLoadingCommand.PSO = PSO;
	}
	CImage* MountImage(size_t InSlot, const std::wstring& InImagePath)
	{
		CImage* Image = RenderResourceLoader.LoadImageFromFile(InImagePath);
		CurrentLoadingCommand.PixelShaderResources[InSlot] = Image;
		return Image;
	}
	void MountConstBuffer(EShaderType InShaderType, size_t InSlot, Graphics::CBuffer& InConstBuffer)
	{
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			CurrentLoadingCommand.VertexConstBuffers[InSlot] = &InConstBuffer;
			break;
		case EShaderType::PixelShader:
			CurrentLoadingCommand.PixelConstBuffers[InSlot] = &InConstBuffer;
			break;
		default:
			assert(0);
		}
	}
	void MountShaderResource(EShaderType InShaderType, size_t InSlot, CImage& InResource)
	{
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			CurrentLoadingCommand.VertexShaderResources[InSlot] = &InResource;
			break;
		case EShaderType::PixelShader:
			CurrentLoadingCommand.PixelShaderResources[InSlot] = &InResource;
			break;
		default:
			assert(0);
		}
	}
	void MountRenderLayer(uint32_t InRenderLayer)
	{
		CurrentLoadingCommand.RenderLayer = InRenderLayer;
	}
	void FinishRender()
	{
		RenderCommands.push_back(CurrentLoadingCommand);
	}
	void Draw()
	{
		std::sort(RenderCommands.begin(), RenderCommands.end(), [](CRenderCommand& InA, CRenderCommand& InB)->bool
			{
				return InA.RenderLayer < InB.RenderLayer;
			});

		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (auto& RenderCommand : RenderCommands)
			DDraw(RenderCommand);

		SwapChain.Present();
	}
	void DDraw(const CRenderCommand& InRenderCommand)
	{
		for (size_t i = 0; i < InRenderCommand.VertexConstBuffers.size(); ++i)
		{
			auto& VertexConstBuffer = InRenderCommand.VertexConstBuffers[i];
			if (VertexConstBuffer == nullptr)
				continue;
			Context.VSSetConstantBuffer(uint32_t(i), VertexConstBuffer);
		}
		for (size_t i = 0; i < InRenderCommand.PixelConstBuffers.size(); ++i)
		{
			auto& PixelConstBuffer = InRenderCommand.PixelConstBuffers[i];
			if (PixelConstBuffer == nullptr)
				continue;
			Context.PSSetConstantBuffer(uint32_t(i), PixelConstBuffer);
		}
		for (size_t i = 0; i < InRenderCommand.PixelShaderResources.size(); ++i)
		{
			auto& PixelShaderResource = InRenderCommand.PixelShaderResources[i];
			if (PixelShaderResource == nullptr)
				continue;
			Context.PSSetShaderResource(uint32_t(i), &PixelShaderResource->GetSRV());
		}

		const Graphics::CMesh* Mesh = InRenderCommand.Mesh;
		const CPSO* PSO = InRenderCommand.PSO;
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
			Context.PSSetSamplers(0, 1, PSO->SamplerState);

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
			Context.PSSetSamplers(0, 1, nullptr);

			uint32_t Stride = 0;
			uint32_t Offset = 0;
			Context.IASetVertexBuffer(nullptr, &Stride, &Offset);
			Context.IASetIndexBuffer(nullptr, Graphics::EGIFormat(0), 0);
			Context.DrawIndexed(0);
		}
	}

	void UpdateRSOs(const std::vector<CRenderStateObject*>& InRenderStateObjects)
	{
		for (auto& RenderStateObject : InRenderStateObjects)
			RenderStateObject->MapBuffersOnUpdated(Context);
	}
	void RenderRSOs(const std::vector<CRenderStateObject*>& InRenderStateObjects)
	{
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (auto& RenderStateObject : InRenderStateObjects)
			RenderStateObject->BindRenderState(Context);

		SwapChain.Present();
	}

	const CPSOManager& GetPSOManager() const { return PSOManager; }
	CRenderResourceLoader& GetRenderResourceLoader() { return RenderResourceLoader; }

private:
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

	std::unique_ptr<Graphics::IGraphicInfra> GraphicInfra;
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	CPSOManager PSOManager;
	CRenderResourceLoader RenderResourceLoader;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<CRenderCommand> RenderCommands;
	CRenderCommand CurrentLoadingCommand;
};