#include "pch.h"
#include "PSOManager.h"
#include "Base/RenderDevice.h"
#include "Base/RenderContext.h"

void CPSO::BindToPipeline(Graphics::CRenderContext& InContext)
{
	InContext.IASetPrimitiveTopology(PrimitiveTopology);
	InContext.IASetInputLayout(*InputLayout);
	InContext.VSSetShader(*VertexShader);
	InContext.RSSetState(*RasterizerState);
	InContext.PSSetShader(*PixelShader);
}

CPSOManager::CPSOManager(Graphics::CRenderDevice& InDevice)
{
	std::vector<Graphics::TInputElementDesc> InputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::UV, Graphics::EFormat::Vector2, 12, Graphics::EInputClass::VertexData}
	};
	auto VSIA = InDevice.CreateVertexShaderAndInputLayout(L"resources/shader/ImageVertexShader.hlsl", InputElementDescs);
	BasicVertexShader = std::move(VSIA.first);
	BasicInputLayout = std::move(VSIA.second);

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	BasicRasterizerState = InDevice.CreateRasterizerState(RasterizerDesc);

	BasicPixelShader = InDevice.CreatePixelShader(L"resources/shader/ImagePixelShader.hlsl");

	CPSO* ImagePSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), BasicPixelShader.get());

	PSOs[size_t(EPSOType::Basic)] = std::unique_ptr<CPSO>(ImagePSO);
}
