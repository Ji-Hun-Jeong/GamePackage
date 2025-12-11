#pragma once
#include "Actor.h"
#include "01.Base/Actor/Camera.h"
#include "01.Base/Actor/UIManager.h"
#include "02.Contents/Actor/Base/Fader.h"

class CScene : public CActor
{
public:
	CScene();
	virtual ~CScene() = default;

public:
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		UIManager.SetMouseWorldPosition(CMouseManager::GetInst(), *MainCamera);
	}
	void FinalUpdate() override
	{
		CActor::FinalUpdate();
		UIManager.FindFocusUI();
	}
	CCamera* GetMainCamera() const { return MainCamera; }
	CFader* GetFader() const { return Fader; }

	void LoadNewSceneWithFadeOut(CClass* InSceneClass, float InHoldingTime);

private:
	CCamera* MainCamera = nullptr;
	CFader* Fader = nullptr;

protected:
	CUIManager UIManager;

};

