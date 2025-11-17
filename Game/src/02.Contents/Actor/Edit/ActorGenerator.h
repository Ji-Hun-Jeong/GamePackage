#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/StaticActor.h"

class CActorGenerator : public CActor
{
	GENERATE_OBJECT(CActorGenerator)
public:
	CActorGenerator() = default;
	~CActorGenerator() = default;

public:
	CStaticActor* GenerateStaticActor(const std::wstring& InImagePath, const Vector2& InPosition);
	CStaticActor* GenerateStaticActor(const Vector2& InPosition);
	void EraseActor(const Vector2& InWorldPosition);
	void SetGeneratedActorImagePathByWindowManager(class CWindowManager& InWindowManager);
	const std::wstring& GetGeneratedActorImagePath() const { return GeneratedActorImagePath; }

private:
	// Todo: 공간분할
	std::vector<CActor*> GeneratedActors;

	std::wstring GeneratedActorImagePath;
	std::map<std::string, std::wstring> LoadedImagePaths;

};

