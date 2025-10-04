#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{


	class IVertexShader
	{
		friend class RenderContext;
	public:
		IVertexShader() = default;
		virtual ~IVertexShader() = default;

	private:
		virtual void VSSetShader() const = 0;
		virtual void IASetInputLayout() const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXVertexShader : public IVertexShader
		{
		public:
			CDXVertexShader(ComPtr<ID3D11DeviceContext> InContext, ComPtr<ID3D11VertexShader> InVS, ComPtr<ID3D11InputLayout> InIL)
				: Context(InContext)
				, VertexShader(InVS)
				, InputLayout(InIL)
			{}
			~CDXVertexShader()
			{}

		public:
			void VSSetShader() const override
			{
				Context->VSSetShader(VertexShader.Get(), nullptr, 0);
			}
			void IASetInputLayout() const override
			{
				Context->IASetInputLayout(InputLayout.Get());
			}

		private:
			ComPtr<ID3D11VertexShader> VertexShader;
			ComPtr<ID3D11InputLayout> InputLayout;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

