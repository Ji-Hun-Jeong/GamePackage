#pragma once
#include "Actor.h"
#include "01.Base/Actor/Camera.h"
#include "02.Contents/Actor/Base/Fader.h"

class CScene : public CActor
{
public:
	CScene();
	virtual ~CScene() = default;

public:
	CCamera* GetMainCamera() const { return MainCamera; }
	CFader* GetFader() const { return Fader; }

	void LoadNewSceneWithFadeOut(CClass* InSceneClass, float InHoldingTime);

private:
	CCamera* MainCamera;
	CFader* Fader;

};

