#pragma once
#include "Actor.h"
#include "01.Base/Actor/Camera.h"

class CScene : public CActor
{
public:
	CScene();
	virtual ~CScene() = default;

public:
	CCamera* GetMainCamera() const { return MainCamera; }

private:
	CCamera* MainCamera;

};

