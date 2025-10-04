#pragma once
#include "Resource/Buffer.h"
#include "Resource/Shader.h"
#include "Resource/InputLayout.h"

namespace Graphics
{
	class RenderDevice
	{
	public:
		RenderDevice() = default;
		virtual ~RenderDevice() = 0 {}


	};
}