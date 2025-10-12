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
	Graphics::CMesh& MakeMesh(const std::string& InMeshName, const Graphics::TMeshData& InMeshData)
	{
		VB = Device->CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		IB = Device->CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(*VB.get(), *IB.get(), InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride
			, InMeshData.Offset);

		return *Mesh;
	}
	std::pair<Graphics::CVertexShader*, Graphics::CInputLayout*> MakeVSAndInputLayout(const std::wstring& InVertexShaderPath
		, const std::vector<Graphics::TInputElementDesc>& InInputElementDescs)
	{
		auto VSAndInputLayout = Device->CreateVertexShaderAndInputLayout(InVertexShaderPath, InInputElementDescs);
		VS = std::move(VSAndInputLayout.first);
		IA = std::move(VSAndInputLayout.second);
		return { VS.get(), IA.get() };
	}
	Graphics::CPixelShader& MakePixelShader(const std::wstring& InPixelShaderPath)
	{
		PS = Device->CreatePixelShader(InPixelShaderPath);
		return *PS.get();
	}

private:
	Graphics::CRenderDevice* Device;

	// 얘네가 안사라졌는데 Storage가 먼저 사라짐 싱글턴이라
	std::unique_ptr<Graphics::CBuffer> VB;
	std::unique_ptr<Graphics::CBuffer> IB;
	std::unique_ptr<Graphics::CVertexShader> VS;
	std::unique_ptr<Graphics::CInputLayout> IA;
	std::unique_ptr<Graphics::CPixelShader> PS;

};

