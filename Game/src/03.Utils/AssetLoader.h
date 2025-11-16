#pragma once
#include <Renderer/Rendering/Mesh.h>

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
	const Graphics::TMeshData& GetMeshData(const std::string& InMeshName) const
	{
		auto Iter = MeshDatas.find(InMeshName);

		assert(Iter != MeshDatas.end());

		return Iter->second;
	}


private:
	std::map<std::string, Graphics::TMeshData> MeshDatas;

};

