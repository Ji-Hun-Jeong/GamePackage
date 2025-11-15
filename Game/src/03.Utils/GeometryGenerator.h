#pragma once
#include <Renderer/Rendering/Mesh.h>

struct TVertex
{
	Vector3 Position;
	Vector2 UV;
};
struct TColorVertex
{
	Vector3 Position;
	Vector3 Color;
};
struct TGeometryData
{
	std::vector<TVertex> Vertices;
	std::vector<uint32_t> Indices;
};
struct TColorGeometryData
{
	std::vector<TColorVertex> Vertices;
	std::vector<uint32_t> Indices;
};
class CGeometryGenerator
{
public:
	static TGeometryData GenerateSquare();
	static TColorGeometryData GenerateRect();
};

