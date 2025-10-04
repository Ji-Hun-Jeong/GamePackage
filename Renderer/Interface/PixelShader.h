#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IPixelShader
	{
		friend class RenderContext;
	public:
		IPixelShader() = default;
		virtual ~IPixelShader() = default;

	private:
		virtual void PSSetShader() const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXPixelShader : public IPixelShader
		{
		public:
			CDXPixelShader(ComPtr<ID3D11DeviceContext> InContext, ComPtr<ID3D11PixelShader> InPS)
				: Context(InContext)
				, PixelShader(InPS)
			{
			}
			~CDXPixelShader()
			{
			}

		public:
			void PSSetShader() const override
			{
				Context->PSSetShader(PixelShader.Get(), nullptr, 0);
			}

		private:
			ComPtr<ID3D11PixelShader> PixelShader;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

