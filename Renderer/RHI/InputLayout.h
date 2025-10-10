#pragma once
#include "RHI.h"

namespace Graphics
{
	enum class ESementicName
	{
		Position, Color, UV,
	};
	enum class EFormat
	{
		Vector2, Vector3,
	};
	enum class EInputClass
	{
		VertexData,
	};

	struct TInputElementDesc
	{
		ESementicName SementicName;
		EFormat Format;
		uint32_t Offset;
		EInputClass InputClass;
	};

	class CInputLayout : public CRHI
	{
	public:
		CInputLayout(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource
			, const std::vector<TInputElementDesc>& InInputElementDescs)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, InputElementDescs(InInputElementDescs)
		{}
		~CInputLayout() = default;

	public:
		uint32_t GetRHIType() override
		{
			return StaticType();
		}
		static uint32_t StaticType()
		{
			static uint32_t RHIType = sRHIType++;
			return RHIType;
		}

	private:
		std::vector<TInputElementDesc> InputElementDescs;

	};
}


