#include "pch.h"
#include "Utils.h"

bool StrToVec2(const std::string& InStr, Vector2* OutVec2)
{
	size_t Index = InStr.find(' ');
	if (Index == std::string::npos)
		return false;
	std::string XStr = InStr.substr(0, Index);
	std::string YStr = InStr.substr(Index + 1);

	float X = std::stof(XStr);
	float Y = std::stof(YStr);

	OutVec2->x = X;
	OutVec2->y = Y;

	return true;
}
