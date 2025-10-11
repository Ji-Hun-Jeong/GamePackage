#pragma once
#include <Renderer/Rendering/Mesh.h>

namespace Graphics
{
	class CRenderContext;
}

class CRenderStateObject
{
public:
	CRenderStateObject(Graphics::CMesh& InMesh)
		: Mesh(InMesh)
		, bDestroy(false)
	{}
	~CRenderStateObject() = default;

public:
	virtual void BindRenderState(Graphics::CRenderContext& InContext) const = 0;
	void Destroy() { bDestroy = true; }
	bool BeDestroy() const { return bDestroy; }

	bool operator > (CRenderStateObject& InOther) const
	{
		return &Mesh > &InOther.Mesh;
	}
private:
	Graphics::CMesh& Mesh;

	bool bDestroy;

};







