#pragma once
#include <array>
#include "RHI/VertexShader.h"
#include "RHI/InputLayout.h"
#include "RHI/RasterizerState.h"
#include "RHI/PixelShader.h"
#include "RHI/Buffer.h"

namespace Graphics
{
	class CRenderDevice;
	class CRenderContext;
}

class CPSO
{
public:
	CPSO(std::unique_ptr<Graphics::CVertexShader> InVertexShader, std::unique_ptr<Graphics::CInputLayout> InInputLayout
		, Graphics::ETopology InPrimitiveTopology
		, std::unique_ptr<Graphics::CRasterizerState> InRasterizerState, std::unique_ptr<Graphics::CPixelShader> InPixelShader)
		: VertexShader(std::move(InVertexShader))
		, InputLayout(std::move(InInputLayout))
		, PrimitiveTopology(InPrimitiveTopology)
		, RasterizerState(std::move(InRasterizerState))
		, PixelShader(std::move(InPixelShader))
	{}
	void BindToPipeline(Graphics::CRenderContext& InContext);

private:
	std::unique_ptr<Graphics::CVertexShader> VertexShader;
	std::unique_ptr<Graphics::CInputLayout> InputLayout;
	Graphics::ETopology PrimitiveTopology;
	std::unique_ptr<Graphics::CRasterizerState> RasterizerState;
	std::unique_ptr<Graphics::CPixelShader> PixelShader;

};

enum class EPSOType
{
	ColorBasic,
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
	std::array<std::unique_ptr<CPSO>, size_t(EPSOType::End)> PSOs;

	
};


