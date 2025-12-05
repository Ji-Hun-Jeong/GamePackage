#pragma once
#include "RHI/PixelShader.h"
#include "RHI/ShaderResourceView.h"

namespace Graphics
{
	// 텍스쳐, PixelShaderConstantBuffer, SamplerState <- 이건 PSO..?
	using MaterialKey = uint32_t;

	struct TMaterialData
	{
		std::wstring PixelShaderPath;
		std::array<std::wstring, 6> ImagePaths;
		bool operator==(const TMaterialData& other) const
		{
			return PixelShaderPath == other.PixelShaderPath &&
				ImagePaths == other.ImagePaths;
		}
		bool operator < (const TMaterialData& InOther) const
		{
			if (PixelShaderPath < InOther.PixelShaderPath)
				return true;
			for (size_t i = 0; i < ImagePaths.size(); ++i)
			{
				if (ImagePaths[i] < InOther.ImagePaths[i])
					return true;
			}
			return false;
		}
	};

	class CMaterial
	{
	public:
		CMaterial(const CPixelShader& InPixelShader)
			: PixelShader(InPixelShader)
			, ShaderResourceViews{ nullptr }
			, Texture2Ds{ nullptr }
		{}
		~CMaterial() = default;

	public:
		const CPixelShader& GetPixelShader() const { return PixelShader; }
		const std::array<const CShaderResourceView*, 6>& GetShaderResourceViews() const { return ShaderResourceViews; }
		const std::array<const CTexture2D*, 6>& GetTexture2Ds() const { return Texture2Ds; }
		const CTexture2D& GetTexture2D(size_t InSlot) const { return *Texture2Ds[InSlot]; }
		void SetShaderResourceView(size_t InSlot, const CShaderResourceView* InShaderResourceView)
		{
			ShaderResourceViews[InSlot] = InShaderResourceView;
		}
		void SetImageResource(size_t InSlot, const CShaderResourceView* InShaderResourceView, const CTexture2D* InTexture2D)
		{
			ShaderResourceViews[InSlot] = InShaderResourceView;
			Texture2Ds[InSlot] = InTexture2D;
		}

	private:
		const CPixelShader& PixelShader;
		std::array<const CShaderResourceView*, 6> ShaderResourceViews;
		std::array<const CTexture2D*, 6> Texture2Ds;

	};
}

