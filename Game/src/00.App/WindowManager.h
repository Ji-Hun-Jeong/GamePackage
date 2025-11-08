#pragma once

class CWindowManager
{
public:
	static uint32_t GetScreenWidth() { return ScreenWidth; }
	static uint32_t GetScreenHeight() { return ScreenHeight; }
	static void SetScreenWidth(uint32_t InScreenWidth) { ScreenWidth = InScreenWidth; }
	static void SetScreenHeight(uint32_t InScreenHeight) { ScreenHeight = InScreenHeight; }

private:
	inline static uint32_t ScreenWidth = 0;
	inline static uint32_t ScreenHeight = 0;
};

