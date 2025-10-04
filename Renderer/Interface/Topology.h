#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	enum class eTopology
	{
		Triangle, End,
	};
	class ITopology
	{
		friend class RenderContext;
	public:
		ITopology() = default;
		virtual ~ITopology() = default;

	private:
		virtual void IASetPrimitiveTopology() const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXTopology : public ITopology
		{
		public:
			CDXTopology(ComPtr<ID3D11DeviceContext> InContext, D3D11_PRIMITIVE_TOPOLOGY InTL)
				: Context(InContext)
				, Topology(InTL)
			{}
			~CDXTopology()
			{}

		public:
			void IASetPrimitiveTopology() const override
			{
				Context->IASetPrimitiveTopology(Topology);
			}

		private:
			ComPtr<ID3D11DeviceContext> Context;
			D3D11_PRIMITIVE_TOPOLOGY Topology;
		};
	}

}

