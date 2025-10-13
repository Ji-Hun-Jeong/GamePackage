#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include <Renderer/RHI/VertexShader.h>
#include <Renderer/RHI/InputLayout.h>

namespace Graphics
{
	class CRenderContext;
}

class CRenderStateObject
{
	friend class CRenderer;
public:
	CRenderStateObject(Graphics::CMesh* InMesh, Graphics::CMaterial* InMaterial)
		: Mesh(InMesh)
		, Material(InMaterial)
		, bDestroy(false)
	{}
	virtual ~CRenderStateObject() = default;

public:
	virtual void BindRenderState(Graphics::CRenderContext& InContext) const = 0;
	void Destroy() { bDestroy = true; }
	bool operator > (CRenderStateObject& InOther) const
	{
		return &Mesh > &InOther.Mesh;
	}

protected:
	Graphics::CMesh* Mesh;
	Graphics::CMaterial* Material;
	bool bDestroy;

};

class CBasicRenderStateObject : public CRenderStateObject
{
public:
	CBasicRenderStateObject(Graphics::CMesh* InMesh, Graphics::CMaterial* InMaterial
		, std::unique_ptr<Graphics::CVertexShader> InVertexShader, std::unique_ptr<Graphics::CInputLayout> InInputLayout)
		: CRenderStateObject(InMesh, InMaterial)
		, VertexShader(std::move(InVertexShader))
		, InputLayout(std::move(InInputLayout))
	{}
	~CBasicRenderStateObject() = default;

public:
	void BindRenderState(Graphics::CRenderContext& InContext) const override;

private:
	std::unique_ptr<Graphics::CVertexShader> VertexShader;
	std::unique_ptr<Graphics::CInputLayout> InputLayout;

};





