#pragma once
#include <Renderer/Rendering/Mesh.h>

struct TVertex
{
	Vector3 Position;
	Vector2 UV;
};
struct TGeometryData
{
	std::vector<TVertex> Vertices;
	std::vector<uint32_t> Indices;
};
class CGeometryGenerator
{
public:
	static TGeometryData GenerateSquare(const float InScreenWidth, const float InScreenHeight, const float InWidth, const float InHeight);
};

