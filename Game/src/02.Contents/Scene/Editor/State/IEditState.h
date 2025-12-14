#pragma once
#include "01.Base/Actor/Actor.h"
#include "02.Contents/Actor/UI/UI.h"
#include "02.Contents/Actor/Tile/TileMap.h"
#include "02.Contents/Actor/Edit/EditHelper.h"
#include "04.Renderer/ImGuiManager.h"

struct TEditContext
{
	CUI* MainPanel = nullptr;
	CTileMap* TileMap = nullptr;
};

class IEditState : public CActor
{
public:
	IEditState() = default;
	virtual ~IEditState() = default;

public:
	virtual void EnterEditState(TEditContext& InEditContext) = 0;
	virtual void OnEditState(TEditContext& InEditContext) = 0;
	virtual void ExitEditState(TEditContext& InEditContext) = 0;
	virtual void ToImGUI() {}

	void SetMouseWorldPosition(const Vector2& InMouseWorldPosition) { MouseWorldPosition = InMouseWorldPosition; }
	const Vector2& GetMouseWorldPosition() const { return MouseWorldPosition; }

private:
	Vector2 MouseWorldPosition;
};

