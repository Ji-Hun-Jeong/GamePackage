#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include "PSOManager.h"

class CRenderStateObject
{
	friend class CRenderer;
public:
	CRenderStateObject(Graphics::CMesh* InMesh, Graphics::CMaterial* InMaterial, CPSO* InPSO)
		: Mesh(InMesh)
		, Material(InMaterial)
		, PSO(InPSO)
		, bDestroy(false)
	{}
	~CRenderStateObject() = default;

public:
	void BindRenderState(Graphics::CRenderContext& InContext) const
	{
		PSO->BindToPipeline(InContext);
		Material->BindToPipeline(InContext);
		Mesh->BindToPipeline(InContext);
	}
	void Destroy() { bDestroy = true; }
	bool operator > (CRenderStateObject& InOther) const
	{
		return &Mesh > &InOther.Mesh;
	}

protected:
	Graphics::CMesh* Mesh;
	Graphics::CMaterial* Material;
	CPSO* PSO;
	bool bDestroy;

};