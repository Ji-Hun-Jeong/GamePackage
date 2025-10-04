#pragma once
#include "Base/RenderContext.h"
#include "DX.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXContext : public RenderContext
	{
	public:
		CDXContext()
			: Context(nullptr)
		{}
		~CDXContext() = default;

	public:
		void Initalize(ComPtr<ID3D11DeviceContext> InContext)
		{
			Context = InContext;
		}
		void IASetInputLayout(class CInputLayout& InInputLayout)
		{

		}

	private:
		ComPtr<ID3D11DeviceContext> Context;
		
	};
}


