#pragma once
#include <array>
#include <Renderer/RHI/VertexShader.h>
#include <Renderer/RHI/InputLayout.h>
#include <Renderer/RHI/RasterizerState.h>
#include <Renderer/RHI/PixelShader.h>
#include <Renderer/RHI/Buffer.h>
#include <Renderer/RHI/SamplerState.h>

namespace Graphics
{
	class CRenderDevice;
	class CRenderContext;
}

class CPSO
{
public:
	CPSO(Graphics::ETopology InPrimitiveTopology, Graphics::CInputLayout* InInputLayout
		, Graphics::CVertexShader* InVertexShader, Graphics::CRasterizerState* InRasterizerState
		, Graphics::CPixelShader* InPixelShader, Graphics::CSamplerState* InSamplerState)
		: PrimitiveTopology(InPrimitiveTopology)
		, InputLayout(InInputLayout)
		, VertexShader(InVertexShader)
		, RasterizerState(InRasterizerState)
		, PixelShader(InPixelShader)
		, SamplerState(InSamplerState)
	{}
	void BindToPipeline(Graphics::CRenderContext& InContext);

private:
	Graphics::ETopology PrimitiveTopology;
	Graphics::CInputLayout* InputLayout;
	Graphics::CVertexShader* VertexShader;
	Graphics::CRasterizerState* RasterizerState;
	Graphics::CPixelShader* PixelShader;
	Graphics::CSamplerState* SamplerState;

};

enum class EPSOType
{
	Basic,
	End,
};

class CPSOManager
{
public:
	CPSOManager(Graphics::CRenderDevice& InDevice);
	~CPSOManager() = default;

public:
	CPSO* GetPSO(EPSOType InPSOType)
	{
		return PSOs[(size_t)InPSOType].get();
	}

private:
	std::unique_ptr<Graphics::CInputLayout> BasicInputLayout;
	std::unique_ptr<Graphics::CVertexShader> BasicVertexShader;
	std::unique_ptr<Graphics::CRasterizerState> BasicRasterizerState;
	std::unique_ptr<Graphics::CPixelShader> BasicPixelShader;
	std::unique_ptr<Graphics::CSamplerState> LinearSamplerState;

	std::array<std::unique_ptr<CPSO>, size_t(EPSOType::End)> PSOs;

};


