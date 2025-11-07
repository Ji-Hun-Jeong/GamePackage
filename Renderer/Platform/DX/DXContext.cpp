#include "pch.h"
#include "DXContext.h"

namespace Graphics::DX
{
	void CDXContext::OMSetRenderTargets(uint32_t InNumViews, const CRenderTargetView* InRenderTargetView, const CDepthStencilView* InDepthStencilView)
	{
		std::vector<ID3D11RenderTargetView*> RawRenderTargetViews(InNumViews, nullptr);
		for (size_t i = 0; i < RawRenderTargetViews.size(); ++i)
			RawRenderTargetViews[i] = DXResourceStorage.GetResource<ID3D11RenderTargetView>(InRenderTargetView->GetResourceHandle());

		ID3D11DepthStencilView* RawDepthStencilView = nullptr;
		if (InDepthStencilView)
			RawDepthStencilView = DXResourceStorage.GetResource<ID3D11DepthStencilView>(InDepthStencilView->GetResourceHandle());
		Context->OMSetRenderTargets(InNumViews, RawRenderTargetViews.data(), RawDepthStencilView);
	}
	void CDXContext::ClearRenderTarget(const CRenderTargetView* InRenderTargetView, const float* InClearColor)
	{
		ID3D11RenderTargetView* RawRenderTargetView = nullptr;
		if(InRenderTargetView)
			RawRenderTargetView = DXResourceStorage.GetResource<ID3D11RenderTargetView>(InRenderTargetView->GetResourceHandle());
		Context->ClearRenderTargetView(RawRenderTargetView, InClearColor);
	}
	void CDXContext::IASetInputLayout(const CInputLayout* InInputLayout)
	{
		ID3D11InputLayout* RawInputLayout = nullptr;
		if(InInputLayout)
			RawInputLayout = DXResourceStorage.GetResource<ID3D11InputLayout>(InInputLayout->GetResourceHandle());
		Context->IASetInputLayout(RawInputLayout);
	}
	void CDXContext::IASetPrimitiveTopology(ETopology InTopology)
	{
		D3D11_PRIMITIVE_TOPOLOGY RawTopology = (D3D11_PRIMITIVE_TOPOLOGY)InTopology;
		Context->IASetPrimitiveTopology(RawTopology);
	}
	void CDXContext::IASetVertexBuffer(const CBuffer* InVertexBuffer, const uint32_t* InStride, const uint32_t* InOffset)
	{
		ID3D11Buffer* RawVertexBuffer = nullptr;
		if(InVertexBuffer)
			RawVertexBuffer = DXResourceStorage.GetResource<ID3D11Buffer>(InVertexBuffer->GetResourceHandle());
		
		Context->IASetVertexBuffers(0, 1, &RawVertexBuffer, InStride, InOffset);
	}
	void CDXContext::IASetIndexBuffer(const CBuffer* InIndexBuffer, EGIFormat InFormat, uint32_t InOffset)
	{
		DXGI_FORMAT Format = DXGI_FORMAT(InFormat);
		ID3D11Buffer* RawIndexBuffer = nullptr;
		if(InIndexBuffer)
			RawIndexBuffer = DXResourceStorage.GetResource<ID3D11Buffer>(InIndexBuffer->GetResourceHandle());
		Context->IASetIndexBuffer(RawIndexBuffer, Format, InOffset);
	}
	void CDXContext::VSSetShader(const CVertexShader* InVertexShader)
	{
		ID3D11VertexShader* RawVertexShader = nullptr;
		if(InVertexShader)
			RawVertexShader = DXResourceStorage.GetResource<ID3D11VertexShader>(InVertexShader->GetResourceHandle());
		Context->VSSetShader(RawVertexShader, nullptr, 0);
	}
	void CDXContext::VSSetConstantBuffers(uint32_t InStartSlot, uint32_t InNumBuffers, const std::vector<std::unique_ptr<CBuffer>>& InBuffers)
	{
		std::vector<ID3D11Buffer*> Buffers(InNumBuffers, nullptr);
		for (size_t i = 0; i < InNumBuffers; ++i)
		{
			Buffers[i] = DXResourceStorage.GetResource<ID3D11Buffer>(InBuffers[i]->GetResourceHandle());
		}
		Context->VSSetConstantBuffers(InStartSlot, InNumBuffers, Buffers.data());
	}

	void CDXContext::VSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer)
	{
		ID3D11Buffer* Buffer = nullptr;
		if(InBuffer)
			Buffer = DXResourceStorage.GetResource<ID3D11Buffer>(InBuffer->GetResourceHandle());
		Context->VSSetConstantBuffers(InStartSlot, 1, &Buffer);
	}

	void CDXContext::RSSetViewPort(const TViewPort* InViewPort)
	{
		D3D11_VIEWPORT RawViewPort;
		memcpy(&RawViewPort, InViewPort, sizeof(TViewPort));
		Context->RSSetViewports(1, &RawViewPort);
	}
	void CDXContext::RSSetState(const CRasterizerState* InRasterizerState)
	{
		ID3D11RasterizerState* RawRasterizerState = nullptr;
		if(InRasterizerState)
			RawRasterizerState = DXResourceStorage.GetResource<ID3D11RasterizerState>(InRasterizerState->GetResourceHandle());
		Context->RSSetState(RawRasterizerState);
	}
	void CDXContext::PSSetShader(const CPixelShader* InPixelShader)
	{
		ID3D11PixelShader* RawPixelShader = nullptr;
		if(InPixelShader)
			RawPixelShader = DXResourceStorage.GetResource<ID3D11PixelShader>(InPixelShader->GetResourceHandle());
		Context->PSSetShader(RawPixelShader, nullptr, 0);
	}
	void CDXContext::PSSetShaderResources(uint32_t InStartSlot, uint32_t InNumViews, const std::vector<std::unique_ptr<CShaderResourceView>>& InShaderResourceViews)
	{
		std::vector<ID3D11ShaderResourceView*> ShaderResourceViews(InNumViews, nullptr);
		for (size_t i = 0; i < InNumViews; ++i)
		{
			ShaderResourceViews[i] = DXResourceStorage.GetResource<ID3D11ShaderResourceView>(InShaderResourceViews[i]->GetResourceHandle());
		}

		Context->PSSetShaderResources(InStartSlot, InNumViews, ShaderResourceViews.data());
	}
	void CDXContext::PSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView)
	{
		ID3D11ShaderResourceView* ShaderResourceView = nullptr;
		if (InShaderResourceView)
			ShaderResourceView = DXResourceStorage.GetResource<ID3D11ShaderResourceView>(InShaderResourceView->GetResourceHandle());
		Context->PSSetShaderResources(InStartSlot, 1, &ShaderResourceView);
	}
	void CDXContext::PSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer)
	{
		ID3D11Buffer* Buffer = nullptr;
		if (InBuffer)
			Buffer = DXResourceStorage.GetResource<ID3D11Buffer>(InBuffer->GetResourceHandle());
		Context->PSSetConstantBuffers(InStartSlot, 1, &Buffer);
	}
	void CDXContext::PSSetSampler(uint32_t InStartSlot, uint32_t InNumSamplers, const CSamplerState* InSamplers)
	{
		ID3D11SamplerState* SamplerStates = nullptr;
		if (InSamplers)
			SamplerStates = DXResourceStorage.GetResource<ID3D11SamplerState>(InSamplers->GetResourceHandle());
		Context->PSSetSamplers(InStartSlot, 1, &SamplerStates);
	}
	void CDXContext::OMSetBlendState(const CBlendState* InBlendState, const float* InBlendFactor, uint32_t InSampleMask)
	{
		ID3D11BlendState* BlendState = nullptr;
		if(InBlendState)
			BlendState = DXResourceStorage.GetResource<ID3D11BlendState>(InBlendState->GetResourceHandle());
		Context->OMSetBlendState(BlendState, InBlendFactor, InSampleMask);
	}
}