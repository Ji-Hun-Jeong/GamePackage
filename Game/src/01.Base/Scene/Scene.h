#pragma once
#include "01.Base/Object/Actor/Actor.h"

class CScene
{
	DONTCOPY(CScene)
public:
	CScene(const std::string& InSceneName)
		: SceneName(InSceneName)
	{}
	virtual ~CScene() = default;

public:
	virtual void EnterScene() {}
	virtual void ExitScene() {}
	virtual void Update(float InDeltaTime)
	{
		for (auto& Actor : Actors)
			Actor->Update(InDeltaTime);
	}

public:
	void AddActor(CActor* InActor)
	{
		Actors.push_back(std::unique_ptr<CActor>(InActor));
	}
	const std::string& GetSceneName() const { return SceneName; }

protected:
	std::list<std::unique_ptr<CActor>> Actors;

	std::string SceneName;

};

