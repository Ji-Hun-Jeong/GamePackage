#pragma once
#include "01.Base/Actor/Actor.h"

class CLadderEditor
{
public:
	CLadderEditor() = default;
	~CLadderEditor() = default;

public: 
	void SetHeadImagePath(const std::wstring& InHeadImagePath)
	{
		HeadImagePath = InHeadImagePath;
	}
	void AddBodyImagePath(const std::wstring& InBodyImagePath)
	{
		BodyImagePaths.push_back(InBodyImagePath);
	}
	void SetFootImagePath(const std::wstring& InFootImagePath)
	{
		FootImagePath = InFootImagePath;
	}

private:
	std::wstring HeadImagePath;
	std::vector<std::wstring> BodyImagePaths;
	std::wstring FootImagePath;

	CActor* CurrentFocusLadderActor = nullptr;
};

