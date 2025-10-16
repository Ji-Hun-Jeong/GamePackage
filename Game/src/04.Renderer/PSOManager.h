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
	CPSO(Graphics::ETopology InPrimitiveTopology, Graphics::CInputLayout* InInputLayout
		, Graphics::CVertexShader* InVertexShader, Graphics::CRasterizerState* InRasterizerState, Graphics::CPixelShader* InPixelShader)
		: PrimitiveTopology(InPrimitiveTopology)
		, InputLayout(InInputLayout)
		, VertexShader(InVertexShader)
		, RasterizerState(InRasterizerState)
		, PixelShader(InPixelShader)
	{}
	void BindToPipeline(Graphics::CRenderContext& InContext);

private:
	Graphics::ETopology PrimitiveTopology;
	Graphics::CInputLayout* InputLayout;
	Graphics::CVertexShader* VertexShader;
	Graphics::CRasterizerState* RasterizerState;
	Graphics::CPixelShader* PixelShader;

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

	std::array<std::unique_ptr<CPSO>, size_t(EPSOType::End)> PSOs;

};


