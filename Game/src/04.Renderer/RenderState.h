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

	uint32_t RenderLayer;
};