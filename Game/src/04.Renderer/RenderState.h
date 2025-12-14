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
	Graphics::CMesh* Mesh;
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
	CPSO* PSO;

	std::array<const Graphics::CShaderResourceView*, 6> VertexShaderResources;
	Graphics::CMaterial* Material;

	std::array<Graphics::CBuffer*, 6> VertexConstBuffers;
	std::array<Graphics::CBuffer*, 6> PixelConstBuffers;

	ERenderPass RenderPass = ERenderPass::Opaque;
	uint32_t RenderLayer;
};