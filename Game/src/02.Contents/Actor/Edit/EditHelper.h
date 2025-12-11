#pragma once
#include "01.Base/Actor/StaticActor.h"

class CImageImporter
{
public:
	CImageImporter() = default;
	~CImageImporter() = default;

public:
	void AddImagePathByWindowManager(class CWindowManager& InWindowManager);
	void AddImagePath(const std::string& InImageName, const std::wstring& InImagePath)
	{
		LoadedImagePaths.emplace(InImageName, InImagePath);
		CurrentImagePath = InImagePath;
	}
	void SetCurrentImagePath(const std::string& InImageName)
	{
		auto Iter = LoadedImagePaths.find(InImageName);
		if (Iter == LoadedImagePaths.end())
			return;
		CurrentImagePath = Iter->second;
	}
	bool IsExistCurrentImagePath() const { return CurrentImagePath.empty() == false; }
	const std::wstring& GetCurrentImagePath() const { return CurrentImagePath; }

	const std::map<std::string, std::wstring>& GetLoadedImagePaths() const { return LoadedImagePaths; }

private:
	std::wstring CurrentImagePath;
	std::map<std::string, std::wstring> LoadedImagePaths;

};

class CActorGenerator : public CActor
{
	GENERATE_OBJECT(CActorGenerator)
public:
	CActorGenerator() = default;
	~CActorGenerator() = default;

public:
	CStaticActor* GenerateStaticActor(const CImageImporter& InImageImporter, const Vector2& InPosition)
	{
		const std::wstring& ActorImagePath = InImageImporter.GetCurrentImagePath();
		return GenerateStaticActor(ActorImagePath, InPosition);
	}
	CStaticActor* GenerateStaticActor(const std::wstring& InImagePath, const Vector2& InPosition)
	{
		if (InImagePath.empty())
			return nullptr;

		CStaticActor* StaticActor = GenerateStaticActor(InPosition);
		StaticActor->GetSpriteRenderComponent()->SetDiffuseImage(InImagePath);

		return StaticActor;
	}
	CStaticActor* GenerateStaticActor(const Vector2& InPosition);

private:

};

class CActorTranslator
{
public:
	CActorTranslator() = default;
	~CActorTranslator() = default;

public:
	void SetFirstDiff(class CMouseManager& InMouseManager, CActor& InTranslatedActor);
	void TranslateActor(class CMouseManager& InMouseManager, CActor& InTranslatedActor);

private:
	Vector3 FirstDiff;
};