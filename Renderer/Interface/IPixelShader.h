#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IPixelShader
	{
	public:
		IPixelShader() = default;
		virtual ~IPixelShader() = default;

	public:
		virtual void PSSetShader() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXPixelShader : public IPixelShader
		{
		public:
			CDXPixelShader(ComPtr<ID3D11DeviceContext>& _Context, ComPtr<ID3D11PixelShader>& _PS)
				: Context(_Context)
				, PixelShader(_PS.Get())
			{
				PixelShader->AddRef();
			}
			~CDXPixelShader()
			{
				PixelShader->Release();
			}

		public:
			void PSSetShader() const override
			{
				Context->PSSetShader(PixelShader, nullptr, 0);
			}

		private:
			ID3D11PixelShader* PixelShader;
			ComPtr<ID3D11DeviceContext>& Context;
		};
	}

}

