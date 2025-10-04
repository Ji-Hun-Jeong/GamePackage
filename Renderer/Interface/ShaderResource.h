#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IShaderResource
	{
		friend class RenderContext;
	public:
		IShaderResource() = default;
		virtual ~IShaderResource() = default;

	private:
		virtual void VSSetShaderResources(UINT InStartSlot) const = 0;
		virtual void PSSetShaderResources(UINT InStartSlot) const = 0;
		virtual void CSSetShaderResources(UINT InStartSlot) const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXShaderResource : public IShaderResource
		{
		public:
			CDXShaderResource(ComPtr<ID3D11DeviceContext>& InContext, std::vector<ComPtr<ID3D11ShaderResourceView>>&& InShaderResourceViews)
				: Context(InContext)
				, ShaderResourceViews(InShaderResourceViews)
			{}
			~CDXShaderResource()
			{}

		public:
			void VSSetShaderResources(UINT InStartSlot) const override
			{
				Context->VSSetShaderResources(InStartSlot, ShaderResourceViews.size(), ShaderResourceViews.data()->GetAddressOf());
			}
			void PSSetShaderResources(UINT InStartSlot) const override
			{
				Context->PSSetShaderResources(InStartSlot, ShaderResourceViews.size(), ShaderResourceViews.data()->GetAddressOf());
			}
			void CSSetShaderResources(UINT InStartSlot) const override
			{
				Context->CSSetShaderResources(InStartSlot, ShaderResourceViews.size(), ShaderResourceViews.data()->GetAddressOf());
			}
			//Vector2 GetImageSize(size_t _Index) const override
			//{
			//	if (ShaderResourceViews.size() <= _Index)
			//		return Vector2(0.0f, 0.0f);

			//	ID3D11Resource* Resource = nullptr;
			//	ShaderResourceViews[_Index]->GetResource(&Resource);

			//	ID3D11Texture2D* Texture = nullptr;
			//	HRESULT Hr = Resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&Texture);
			//	Resource->Release();

			//	// 텍스처 정보 가져오기
			//	D3D11_TEXTURE2D_DESC Desc;
			//	Texture->GetDesc(&Desc);
			//	Texture->Release();
			//	
			//	return Vector2(Desc.Width, Desc.Height);
			//}

		private:
			std::vector<ComPtr<ID3D11ShaderResourceView>> ShaderResourceViews;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}
}

