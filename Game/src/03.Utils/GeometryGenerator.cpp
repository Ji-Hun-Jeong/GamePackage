#include "pch.h"
#include "GeometryGenerator.h"

TGeometryData CGeometryGenerator::GenerateSquare()
{
	std::vector<TVertex> Vertices =
	{
		{Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)},
		{Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)},
		{Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)},
		{Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)}
	};

	std::vector<uint32_t> Indices =
	{
		0, 1, 2, 0, 2, 3
	};

	return TGeometryData{ Vertices, Indices };
}

TColorGeometryData CGeometryGenerator::GenerateRect()
{
	std::vector<TColorVertex> Vertices =
	{
		{Vector3(-1.0f, -1.0f, 0.0f), Vector3(1.0f,1.0f,1.0f)},
		{Vector3(-1.0f, 1.0f, 0.0f), Vector3(1.0f,1.0f,1.0f)},
		{Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f,1.0f,1.0f)},
		{Vector3(1.0f, -1.0f, 0.0f), Vector3(1.0f,1.0f,1.0f)}
	};

	std::vector<uint32_t> Indices =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	return TColorGeometryData{ Vertices, Indices };
}
