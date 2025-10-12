#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/RHI/VertexShader.h>
#include <Renderer/RHI/InputLayout.h>
#include <Renderer/RHI/PixelShader.h>

namespace Graphics
{
	class CRenderContext;
}

class CRenderStateObject
{
	friend class CRenderer;
public:
	CRenderStateObject(Graphics::CMesh& InMesh)
		: Mesh(InMesh)
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
	Graphics::CMesh& Mesh;
	bool bDestroy;

};

class CBasicRenderStateObject : public CRenderStateObject
{
public:
	CBasicRenderStateObject(Graphics::CMesh& InMesh, Graphics::CVertexShader& InVertexShader
		, Graphics::CInputLayout& InInputLayout, Graphics::CPixelShader& InPixelShader)
		: CRenderStateObject(InMesh)
		, VertexShader(InVertexShader)
		, InputLayout(InInputLayout)
		, PixelShader(InPixelShader)
	{}
	~CBasicRenderStateObject() = default;

public:
	void BindRenderState(Graphics::CRenderContext& InContext) const override;

private:
	Graphics::CVertexShader& VertexShader;
	Graphics::CInputLayout& InputLayout;
	Graphics::CPixelShader& PixelShader;

};





