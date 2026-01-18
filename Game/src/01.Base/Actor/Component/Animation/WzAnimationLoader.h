#pragma once
#include "Animation.h"
#include "Common/Json.h"

class CWzAnimationLoader
{
public:
	CWzAnimationLoader() = default;
	~CWzAnimationLoader() = default;

public:
	CAnimation LoadWzAnimation(const rapidjson::Document& InLoadData, const std::string& InAnimName);
private:

};
