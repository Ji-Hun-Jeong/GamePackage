#pragma once
#include "RHI.h"

namespace Graphics
{

	class CVertexShader : public CRHI
	{
	public:
		CVertexShader(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource
			, const std::wstring& InShaderPath)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, ShaderPath(InShaderPath)
		{}
		~CVertexShader() = default;

	public:
		const std::wstring& GetShaderPath() const { return ShaderPath; }
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
		std::wstring ShaderPath;

	};
}


