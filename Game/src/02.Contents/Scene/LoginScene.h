#pragma once
#include "01.Base/Actor/Scene.h"

class CLoginScene : public CScene
{
public:
	CLoginScene() = default;
	~CLoginScene() = default;

public:
	void BeginPlay() override;
	void Update(float InDeltaTime) override;

private:

};

