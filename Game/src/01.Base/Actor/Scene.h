#pragma once
#include "Actor.h"
#include "01.Base/Actor/Camera.h"

class CScene : public CActor
{
public:
	CScene() = default;
	virtual ~CScene() = default;

public:
	void Initalize() override;

	CCamera* GetMainCamera() const { return MainCamera; }
private:
	CCamera* MainCamera;

};

