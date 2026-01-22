#include "pch.h"
#include "Utils.h"
#include <charconv> // std::from_chars를 위해 필요

bool StrToVec2(const std::string_view InStr, Vector2* OutVec2)
{
    size_t Index = InStr.find(','); 
    if (Index == std::string_view::npos)
        return false;

    const std::string_view XStr = InStr.substr(0, Index);
    const std::string_view YStr = InStr.substr(Index + 1);

    auto [ptrX, ecX] = std::from_chars(XStr.data(), XStr.data() + XStr.size(), OutVec2->x);

    const char* yStart = YStr.data();
    while (yStart < YStr.data() + YStr.size() && (*yStart == ' ' || *yStart == ',')) 
        ++yStart;

    auto [ptrY, ecY] = std::from_chars(yStart, YStr.data() + YStr.size(), OutVec2->y);

    return (ecX == std::errc() && ecY == std::errc());
}