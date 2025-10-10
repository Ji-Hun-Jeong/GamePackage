#pragma once
#include "Mesh.h"

namespace Graphics
{
	class CMeshManager
	{
	public:
		CMeshManager() = default;
		~CMeshManager() = default;

	public:
		void AddMesh(const std::string& InMeshName, std::unique_ptr<CMesh> InMesh)
		{
			if (Meshes.contains(InMeshName))
				printf("CMeshManager: %s Is Already Contained", InMeshName.c_str());

			Meshes.emplace(InMeshName, std::move(InMesh));
		}
		void RemoveMesh(const std::string& InMeshName)
		{
			Meshes.erase(InMeshName);
		}
		CMesh* GetMesh(const std::string& InMeshName)
		{
			auto Iter = Meshes.find(InMeshName);
			if (Iter == Meshes.end())
				return nullptr;

			return Iter->second.get();
		}

	private:
		std::map<std::string, std::unique_ptr<CMesh>> Meshes;
	};
}


