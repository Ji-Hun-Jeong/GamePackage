#include "pch.h"
#include "PSOManager.h"
#include "Base/RenderDevice.h"
#include "Base/RenderContext.h"

void CPSO::BindToPipeline(Graphics::CRenderContext& InContext)
{
	const float Factor = 1.0f;
	const float BlendFactor[4] = { Factor, Factor, Factor, 1.0f };
	InContext.OMSetBlendState(*BlendState, BlendFactor, 0xffffffff);
	InContext.IASetPrimitiveTopology(PrimitiveTopology);
	InContext.IASetInputLayout(*InputLayout);
	InContext.VSSetShader(*VertexShader);
	InContext.RSSetState(*RasterizerState);
	InContext.PSSetShader(*PixelShader);
	InContext.PSSetSamplers(0, 1, SamplerState);
}

CPSOManager::CPSOManager(Graphics::CRenderDevice& InDevice)
{
	std::vector<Graphics::TInputElementDesc> InputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::UV, Graphics::EFormat::Vector2, 12, Graphics::EInputClass::VertexData}
	};
	auto VSIA = InDevice.CreateVertexShaderAndInputLayout(L"resources/shader/BasicVertexShader.hlsl", InputElementDescs);
	BasicVertexShader = std::move(VSIA.first);
	BasicInputLayout = std::move(VSIA.second);

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	BasicRasterizerState = InDevice.CreateRasterizerState(RasterizerDesc);

	BasicPixelShader = InDevice.CreatePixelShader(L"resources/shader/BasicPixelShader.hlsl");

	Graphics::TSamplerDesc SamplerDesc;
	SamplerDesc.Filter = Graphics::EFilter::FILTER_MIN_MAG_MIP_POINT;
	SamplerDesc.AddressU = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.ComparisonFunc = Graphics::EComparisonFunc::COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	LinearSamplerState = InDevice.CreateSamplerState(SamplerDesc);

	Graphics::TBlendDesc BlendStateDesc;
	BlendStateDesc.RenderTarget[0].BlendEnable = true;
	BlendStateDesc.RenderTarget[0].SrcBlend = Graphics::EBlend::BLEND_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].DestBlend = Graphics::EBlend::BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOp = Graphics::EBlendOP::BLEND_OP_ADD;

	BlendStateDesc.RenderTarget[0].SrcBlendAlpha = Graphics::EBlend::BLEND_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].DestBlendAlpha = Graphics::EBlend::BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOpAlpha = Graphics::EBlendOP::BLEND_OP_ADD;

	// 필요하면 RGBA 각각에 대해서도 조절 가능
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = Graphics::EColorWhiteEnable::COLOR_WRITE_ENABLE_ALL;
	BasicBlendState = InDevice.CreateBlendState(BlendStateDesc);

	CPSO* ImagePSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), BasicPixelShader.get(), LinearSamplerState.get(), BasicBlendState.get());

	PSOs[size_t(EPSOType::Basic)] = std::unique_ptr<CPSO>(ImagePSO);
}
