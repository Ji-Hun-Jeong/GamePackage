#include "pch.h"
#include "PSOManager.h"
#include "Base/RenderDevice.h"
#include "Base/RenderContext.h"

CPSOManager::CPSOManager()
{
	
}

void CPSOManager::Initalize(Graphics::CRenderDevice& InDevice)
{
	std::vector<Graphics::TInputElementDesc> InputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::UV, Graphics::EFormat::Vector2, 12, Graphics::EInputClass::VertexData}
	};
	auto VSIA = InDevice.CreateVertexShaderAndInputLayout(L"resources/shader/BasicVertexShader.hlsl", InputElementDescs);
	BasicVertexShader = std::move(VSIA.first);
	BasicInputLayout = std::move(VSIA.second);

	std::vector<Graphics::TInputElementDesc> RectInputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::Color, Graphics::EFormat::Vector3, 12, Graphics::EInputClass::VertexData}
	};
	VSIA = InDevice.CreateVertexShaderAndInputLayout(L"resources/shader/RectVertexShader.hlsl", RectInputElementDescs);
	RectVertexShader = std::move(VSIA.first);
	RectInputLayout = std::move(VSIA.second);

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	BasicRasterizerState = InDevice.CreateRasterizerState(RasterizerDesc);

	BasicPixelShader = InDevice.CreatePixelShader(L"resources/shader/BasicPixelShader.hlsl");
	EdgePixelShader = InDevice.CreatePixelShader(L"resources/shader/EdgePixelShader.hlsl");
	ColorPixelShader = InDevice.CreatePixelShader(L"resources/shader/ColorPixelShader.hlsl");
	RectPixelShader = InDevice.CreatePixelShader(L"resources/shader/RectPixelShader.hlsl");

	Graphics::TSamplerDesc SamplerDesc;
	SamplerDesc.Filter = Graphics::EFilter::FILTER_MIN_MAG_MIP_POINT;
	SamplerDesc.AddressU = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.ComparisonFunc = Graphics::EComparisonFunc::COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	LinearSamplerState = InDevice.CreateSamplerState(SamplerDesc);

	Graphics::TBlendDesc BasicBlendStateDesc;
	BasicBlendStateDesc.RenderTarget[0].BlendEnable = true;
	BasicBlendStateDesc.RenderTarget[0].SrcBlend = Graphics::EBlend::BLEND_SRC_ALPHA;
	BasicBlendStateDesc.RenderTarget[0].DestBlend = Graphics::EBlend::BLEND_INV_SRC_ALPHA;
	BasicBlendStateDesc.RenderTarget[0].BlendOp = Graphics::EBlendOP::BLEND_OP_ADD;

	BasicBlendStateDesc.RenderTarget[0].SrcBlendAlpha = Graphics::EBlend::BLEND_SRC_ALPHA;
	BasicBlendStateDesc.RenderTarget[0].DestBlendAlpha = Graphics::EBlend::BLEND_INV_SRC_ALPHA;
	BasicBlendStateDesc.RenderTarget[0].BlendOpAlpha = Graphics::EBlendOP::BLEND_OP_ADD;

	// 필요하면 RGBA 각각에 대해서도 조절 가능
	BasicBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = Graphics::EColorWhiteEnable::COLOR_WRITE_ENABLE_ALL;
	BasicBlendState = InDevice.CreateBlendState(BasicBlendStateDesc);
	std::cout << BasicBlendState << std::endl;

	Graphics::TBlendDesc TransparentBlendStateDesc;
	TransparentBlendStateDesc.RenderTarget[0].BlendEnable = true;
	TransparentBlendStateDesc.RenderTarget[0].SrcBlend = Graphics::EBlend::BLEND_SRC_ALPHA;
	TransparentBlendStateDesc.RenderTarget[0].DestBlend = Graphics::EBlend::BLEND_INV_SRC_ALPHA;
	TransparentBlendStateDesc.RenderTarget[0].BlendOp = Graphics::EBlendOP::BLEND_OP_ADD;

	TransparentBlendStateDesc.RenderTarget[0].SrcBlendAlpha = Graphics::EBlend::BLEND_ONE;
	TransparentBlendStateDesc.RenderTarget[0].DestBlendAlpha = Graphics::EBlend::BLEND_ZERO;
	TransparentBlendStateDesc.RenderTarget[0].BlendOpAlpha = Graphics::EBlendOP::BLEND_OP_ADD;

	TransparentBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = Graphics::EColorWhiteEnable::COLOR_WRITE_ENABLE_ALL;

	TransparentBlendState = InDevice.CreateBlendState(TransparentBlendStateDesc);
	std::cout << TransparentBlendState << std::endl;

	CPSO* ImagePSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), BasicPixelShader.get(), LinearSamplerState.get(), BasicBlendState.get());
	PSOs[size_t(EPSOType::Basic)] = std::unique_ptr<CPSO>(ImagePSO);


	CPSO* EdgePSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), EdgePixelShader.get(), LinearSamplerState.get(), BasicBlendState.get());
	PSOs[size_t(EPSOType::Mark)] = std::unique_ptr<CPSO>(EdgePSO);


	CPSO* TransparentPSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), BasicPixelShader.get(), LinearSamplerState.get(), TransparentBlendState.get());
	PSOs[size_t(EPSOType::Transparent)] = std::unique_ptr<CPSO>(TransparentPSO);

	CPSO* ColorPSO = new CPSO(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST, BasicInputLayout.get(), BasicVertexShader.get()
		, BasicRasterizerState.get(), ColorPixelShader.get(), LinearSamplerState.get(), BasicBlendState.get());
	PSOs[size_t(EPSOType::Color)] = std::unique_ptr<CPSO>(ColorPSO);

	CPSO* RectPSO = new CPSO(Graphics::ETopology::PrimitiveTopologyLINELIST, RectInputLayout.get(), RectVertexShader.get()
		, BasicRasterizerState.get(), RectPixelShader.get(), LinearSamplerState.get(), BasicBlendState.get());
	PSOs[size_t(EPSOType::Rect)] = std::unique_ptr<CPSO>(RectPSO);
}
