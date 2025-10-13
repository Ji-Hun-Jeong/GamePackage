#include "pch.h"
#include "PSOManager.h"
#include "Base/RenderDevice.h"
#include "Base/RenderContext.h"

void CPSO::BindToPipeline(Graphics::CRenderContext& InContext)
{
	InContext.RSSetState(*RasterizerState);
	InContext.VSSetShader(*VertexShader);
	InContext.IASetInputLayout(*InputLayout);
	InContext.PSSetShader(*PixelShader);
}

void CPSOManager::InitalizePSO(Graphics::CRenderDevice& InDevice)
{
	std::vector<Graphics::TInputElementDesc> InputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::Color, Graphics::EFormat::Vector3, 12, Graphics::EInputClass::VertexData}
	};
	auto VSIA = InDevice.CreateVertexShaderAndInputLayout(L"resources/shader/BasicVertexShader.hlsl", InputElementDescs);

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	auto RasterizerState = InDevice.CreateRasterizerState(RasterizerDesc);

	auto PS = InDevice.CreatePixelShader(L"resources/shader/BasicPixelShader.hlsl");

	CPSO* BasicPSO = new CPSO(std::move(VSIA.first), std::move(VSIA.second), std::move(RasterizerState), std::move(PS));
	EmplacePSO("BasicPSO", BasicPSO);
}
