#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IShaderResource
	{
	public:
		IShaderResource() = default;
		virtual ~IShaderResource() = default;

	public:
		virtual void VSSetShaderResources(UINT _StartSlot) const = 0;
		virtual void PSSetShaderResources(UINT _StartSlot) const = 0;
		virtual void CSSetShaderResources(UINT _StartSlot) const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class DXShaderResource : public IShaderResource
		{
		public:
			DXShaderResource(ComPtr<ID3D11DeviceContext>& _Context, std::vector<ComPtr<ID3D11ShaderResourceView>>& _ShaderResourceViews)
				: Context(_Context)
				, ShaderResourceViews{}
			{
				ShaderResourceViews.reserve(_ShaderResourceViews.size());
				for (auto& SRV : _ShaderResourceViews)
				{
					SRV->AddRef();
					ShaderResourceViews.push_back(SRV.Get());
				}
			}
			~DXShaderResource()
			{
				for (auto SRV : ShaderResourceViews)
					SRV->Release();
			}

		public:
			void VSSetShaderResources(UINT _StartSlot) const override
			{
				Context->VSSetShaderResources(_StartSlot, ShaderResourceViews.size(), ShaderResourceViews.data());
			}
			void PSSetShaderResources(UINT _StartSlot) const override
			{
				Context->PSSetShaderResources(_StartSlot, ShaderResourceViews.size(), ShaderResourceViews.data());
			}
			void CSSetShaderResources(UINT _StartSlot) const override
			{
				Context->CSSetShaderResources(_StartSlot, ShaderResourceViews.size(), ShaderResourceViews.data());
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
			std::vector<ID3D11ShaderResourceView*> ShaderResourceViews;
			ComPtr<ID3D11DeviceContext>& Context;
		};
	}
}

