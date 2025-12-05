#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
class CAssetLoader
{
	SINGLE(CAssetLoader)
public:
	void LoadMeshData(const std::string& InMeshName, const Graphics::TMeshData& InMeshData)
	{
		if (MeshDatas.contains(InMeshName))
			return;
		MeshDatas.emplace(InMeshName, InMeshData);
	}
	void LoadMaterialData(const std::string& InMaterialName, const Graphics::TMaterialData& InMaterialData)
	{
		if (MaterialDatas.contains(InMaterialName))
			return;
		MaterialDatas.emplace(InMaterialName, InMaterialData);
	}
	const Graphics::TMeshData& GetMeshData(const std::string& InMeshName) const
	{
		auto Iter = MeshDatas.find(InMeshName);

		assert(Iter != MeshDatas.end());

		return Iter->second;
	}
	const Graphics::TMaterialData& GetMaterialData(const std::string& InMaterialName) const
	{
		auto Iter = MaterialDatas.find(InMaterialName);

		assert(Iter != MaterialDatas.end());

		return Iter->second;
	}


private:
	std::map<std::string, Graphics::TMeshData> MeshDatas;
	std::map<std::string, Graphics::TMaterialData> MaterialDatas;
};

