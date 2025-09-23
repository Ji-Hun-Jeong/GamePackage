#pragma once
#include <Core/public/InputManager.h>
#include <03.Utils/NumberGenerator/NumberGenerator.h>

#include "01.Base/Scene/Scene.h"

namespace Core
{
	class CWindow;
};

class CWorld
{
public:
	CWorld(Core::CWindow& InWindow);
	~CWorld();

public:
	void Update()
	{
		CurrentScene->Update(0.1f);
	}

public:
	template <typename T>
	T* NewObject(CObject* InOwner)
	{
		T* Object = new T();
		Object->SetWorld(this);
		Object->SetInstanceId(NumberGenerator);
		Object->SetOwner(InOwner);

		return Object;
	}

	template <typename T>
	T* SpawnActor(CScene* InRequestScene = nullptr)
	{
		T* Actor = NewObject<T>(nullptr);
		if (InRequestScene)
			InRequestScene->AddActor(Actor);
		else
			CurrentScene->AddActor(Actor);
		return Actor;
	}

	void AddScene(CScene* InScene)
	{
		Scenes.insert(std::make_pair(InScene->GetSceneName(), std::unique_ptr<CScene>(InScene)));
	}
	void SetCurrentScene(const std::string& InSceneName)
	{
		CScene* FindScene = Scenes.find(InSceneName)->second.get();
		if (FindScene)
			CurrentScene = FindScene;
	}

private:
	Core::CWindow& Window;
	Core::CInputManager InputManager;

	CNumberGenerator NumberGenerator;

	std::map<std::string, std::unique_ptr<CScene>> Scenes;
	CScene* CurrentScene;
};

