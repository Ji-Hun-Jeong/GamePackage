#pragma once
#include "Image.h"
#include "PSOManager.h"
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include <Renderer/RHI/ShaderResourceView.h>

enum class EShaderType
{
	VertexShader,
	PixelShader
};

enum class ERenderPass
{
	Opaque,
	UI,
};

struct TRenderState
{
	Graphics::CMesh* Mesh = nullptr;
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
	CPSO* PSO = nullptr;

	std::array<const Graphics::CShaderResourceView*, 6> VertexShaderResources = { nullptr };
	Graphics::CMaterial* Material = nullptr;

	std::array<Graphics::CBuffer*, 6> VertexConstBuffers = { nullptr };
	std::array<Graphics::CBuffer*, 6> PixelConstBuffers = { nullptr };

	ERenderPass RenderPass = ERenderPass::Opaque;
	uint32_t RenderLayer = 0;
};