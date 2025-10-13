#pragma once
#include "RHI/VertexShader.h"
#include "RHI/InputLayout.h"
#include "RHI/RasterizerState.h"
#include "RHI/PixelShader.h"

namespace Graphics
{
	class CRenderDevice;
	class CRenderContext;
}

class CPSO
{
public:
	CPSO(std::unique_ptr<Graphics::CVertexShader> InVertexShader, std::unique_ptr<Graphics::CInputLayout> InInputLayout
		, std::unique_ptr<Graphics::CRasterizerState> InRasterizerState, std::unique_ptr<Graphics::CPixelShader> InPixelShader)
		: VertexShader(std::move(InVertexShader))
		, InputLayout(std::move(InInputLayout))
		, RasterizerState(std::move(InRasterizerState))
		, PixelShader(std::move(InPixelShader))
	{}
	void BindToPipeline(Graphics::CRenderContext& InContext);

private:
	std::unique_ptr<Graphics::CVertexShader> VertexShader;
	std::unique_ptr<Graphics::CInputLayout> InputLayout;
	std::unique_ptr<Graphics::CRasterizerState> RasterizerState;
	std::unique_ptr<Graphics::CPixelShader> PixelShader;

};

class CPSOManager
{
public:
	static CPSOManager& GetInst()
	{
		static CPSOManager Inst;
		return Inst;
	}
	CPSOManager(const CPSOManager&) = delete;
	CPSOManager(CPSOManager&&) = delete;
	CPSOManager& operator = (const CPSOManager&) = delete;
	CPSOManager& operator = (CPSOManager&&) = delete;

public:
	void InitalizePSO(Graphics::CRenderDevice& InDevice);
	void Finalize()
	{
		PSOs.clear();
	}
	CPSO* GetPSO(const std::string& InPSOName)
	{
		auto Iter = PSOs.find(InPSOName);
		if (Iter == PSOs.end())
			return nullptr;
		return Iter->second.get();
	}
	void EmplacePSO(const std::string& InPSOName, CPSO* InPSO)
	{
		PSOs.emplace(InPSOName, InPSO);
	}

private:
	CPSOManager() = default;

	std::map<std::string, std::unique_ptr<CPSO>> PSOs;
};


