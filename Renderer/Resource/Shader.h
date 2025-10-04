#pragma once
#include "IResource.h"

namespace Graphics
{
	class CShader : public IResource
	{
	public:
		CShader(const std::wstring& InShaderPath)
			: ShaderPath(InShaderPath)
		{}
		~CShader() = default;

	public:
		const std::wstring& GetShaderPath() const { return ShaderPath; }

	private:
		std::wstring ShaderPath;
	};
}


