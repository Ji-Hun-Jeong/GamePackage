#pragma once
#include "Actor.h"

class UIManager : public CActor
{
	GENERATE_OBJECT(UIManager)
public:
	UIManager() = default;
	~UIManager() = default;

public:
	void SetMouseWorldPosition(const class CMouseManager& InMouseManager, const class CCamera& InCamera);

private:
	Vector2 MouseWorldPosition;
};

