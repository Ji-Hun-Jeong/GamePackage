#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IViewPort
	{
		friend class RenderContext;
	public:
		IViewPort() = default;
		virtual ~IViewPort() = default;

	private:
		virtual void RSSetViewPort() const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXViewPort : public IViewPort
		{
		public:
			CDXViewPort(ComPtr<ID3D11DeviceContext> InContext, std::vector<D3D11_VIEWPORT>&& InVP)
				: Context(InContext)
				, ViewPorts(InVP)
			{}
			~CDXViewPort()
			{}

		public:
			void RSSetViewPort() const override
			{
				Context->RSSetViewports(ViewPorts.size(), ViewPorts.data());
			}

		private:
			std::vector<D3D11_VIEWPORT> ViewPorts;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

