#pragma once

namespace Graphics
{
	struct TColorVertex
	{
		Vector3 Pos;
		Vector3 Color;
	};
	struct TColorMeshData
	{
		std::vector<TColorVertex> Vertices;
		std::vector<uint8_t> Indices;
	};
	class CGeometryGenerator
	{
		
	};
}


