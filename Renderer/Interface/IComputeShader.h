#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IComputeShader
	{
	public:
		IComputeShader() = default;
		virtual ~IComputeShader() = default;

	public:
		virtual void CSSetShader() = 0;
		virtual void ComputeShaderBarrier() = 0;
		virtual void Dispatch(UINT InThreadGroupCountX, UINT InThreadGroupCountY, UINT InThreadGroupCountZ) = 0;

	protected:

	};

	using Microsoft::WRL::ComPtr;
	class CDXComputeShader : public IComputeShader
	{
	public:
		CDXComputeShader(ComPtr<ID3D11DeviceContext>& InContext, ComPtr<ID3D11ComputeShader>&& InComputeShader)
			: Context(InContext)
			, ComputeShader(InComputeShader)
		{

		}
		~CDXComputeShader()
		{

		}

	public:
		void CSSetShader() override
		{
			Context->CSSetShader(ComputeShader.Get(), nullptr, 0);
		}

		void ComputeShaderBarrier() override
		{
			/*ID3D11ShaderResourceView* NullSRV[6] = { nullptr };
			Context->CSSetShaderResources(0, 6, NullSRV);*/

			Context->Flush();
		}

		void Dispatch(UINT InThreadGroupCountX, UINT InThreadGroupCountY, UINT InThreadGroupCountZ)
		{
			Context->Dispatch(InThreadGroupCountX, InThreadGroupCountY, InThreadGroupCountZ);
		}

	private:
		ComPtr<ID3D11ComputeShader> ComputeShader;

		ComPtr<ID3D11DeviceContext>& Context;
	};

}

