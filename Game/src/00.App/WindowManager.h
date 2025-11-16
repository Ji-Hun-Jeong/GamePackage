#pragma once

class CWindowManager
{
	SINGLE(CWindowManager)
public:
	uint32_t GetScreenWidth() { return ScreenWidth; }
	uint32_t GetScreenHeight() { return ScreenHeight; }
	void SetScreenWidth(uint32_t InScreenWidth) { ScreenWidth = InScreenWidth; }
	void SetScreenHeight(uint32_t InScreenHeight) { ScreenHeight = InScreenHeight; }

	bool TryGetFilePathByDialog(std::wstring* OutFilePath);

private:
	uint32_t ScreenWidth = 0;
	uint32_t ScreenHeight = 0;

};

