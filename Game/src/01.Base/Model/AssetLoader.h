#pragma once
#include "Model.h"
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/MeshManager.h>

class CAssetLoader
{
	SINGLE(CAssetLoader)
public:
	void Initalize(Graphics::CRenderDevice* InDevice, Graphics::CMeshManager* InMeshManager)
	{
		Device = InDevice;
	}
	void Finalize()
	{
		Models.clear();
	}
	CModel& MakeModel(const std::string& InModelName, const Graphics::TMeshData& InMeshData, const Graphics::TMaterialData& InMaterialData)
	{
		auto Iter = Models.find(InModelName);
		if (Iter != Models.end())
			return *Iter->second.get();

		auto VertexBuffer = Device->CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		auto IndexBuffer = Device->CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		auto PixelShader = Device->CreatePixelShader(InMaterialData.PixelShaderPath);

		auto Mesh = std::make_unique<Graphics::CMesh>(std::move(VertexBuffer), std::move(IndexBuffer), InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride
			, InMeshData.Offset);
		auto Material = std::make_unique<Graphics::CMaterial>(std::move(PixelShader));

		CModel* Model = new CModel(std::move(Mesh), std::move(Material));
		Models.emplace(InModelName, Model);
		return *Model;
	}

	std::pair<std::unique_ptr<Graphics::CVertexShader>, std::unique_ptr<Graphics::CInputLayout>> MakeVSAndInputLayout(const std::wstring& InVertexShaderPath
		, const std::vector<Graphics::TInputElementDesc>& InInputElementDescs)
	{
		return Device->CreateVertexShaderAndInputLayout(InVertexShaderPath, InInputElementDescs);
	}

private:
	Graphics::CRenderDevice* Device;

	std::map<std::string, std::unique_ptr<CModel>> Models;

};

