#pragma once
#include "IResource.h"

namespace Graphics
{
	enum class ESementicName
	{
		Position, Color, UV,
	};
	enum class EFormat
	{
		Vector3, Vector2,
	};
	enum class EInputClass
	{
		VertexData,
	};

	struct TInputElementDesc
	{
		ESementicName SementicName;
		EFormat Format;
		UINT Offset;
		EInputClass InputClass;
	};

	class CInputLayout : public IResource
	{
	public:
		CInputLayout(std::vector<TInputElementDesc>&& InInputElementDescs)
			: InputElementDescs(std::move(InInputElementDescs))
		{}
		~CInputLayout() = default;

		const std::vector<TInputElementDesc>& GetInputElementDescs() const { return InputElementDescs; }

	private:
		std::vector<TInputElementDesc> InputElementDescs;

	};
}


