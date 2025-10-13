#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>

class CModel
{
public:
	CModel(std::unique_ptr<Graphics::CMesh> InMesh, std::unique_ptr<Graphics::CMaterial> InMaterial) 
		: Mesh(std::move(InMesh))
		, Material(std::move(InMaterial))
	{}
	~CModel() {}

public:
	Graphics::CMesh& GetMesh() const { return *Mesh.get(); }
	Graphics::CMaterial& GetMaterial() const { return *Material.get(); }

private:
	std::unique_ptr<Graphics::CMesh> Mesh;
	std::unique_ptr<Graphics::CMaterial> Material;

};

