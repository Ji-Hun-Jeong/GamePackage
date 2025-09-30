#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IViewPort
	{
	public:
		IViewPort() = default;
		virtual ~IViewPort() = default;

	public:
		virtual void RSSetViewPort() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXViewPort : public IViewPort
		{
		public:
			CDXViewPort(ComPtr<ID3D11DeviceContext>& _Context, const std::vector<D3D11_VIEWPORT>& _VP)
				: Context(_Context)
				, ViewPorts{}
			{
				for (auto& VP : _VP)
					ViewPorts.push_back(VP);
			}
			~CDXViewPort()
			{}

		public:
			void RSSetViewPort() const override
			{
				Context->RSSetViewports(ViewPorts.size(), ViewPorts.data());
			}

		private:
			std::vector<D3D11_VIEWPORT> ViewPorts;
			ComPtr<ID3D11DeviceContext>& Context;
		};
	}

}

