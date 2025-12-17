#pragma once
#include "01.Base/Actor/Actor.h"
#include "02.Contents/Actor/UI/UI.h"
#include "02.Contents/Actor/Tile/TileMap.h"
#include "02.Contents/Actor/Tile/TileHandler.h"
#include "02.Contents/Actor/Edit/EditHelper.h"
#include "04.Renderer/ImGuiManager.h"

struct TTileMapEditContext
{
	CUI* MainPanel;
	CTileMap* TileMap;
	CTileMapper TileMapper;

	CTileHandler* TileHandler;
	CTileFocus* TileFocus;

	bool bOnMainPanel = false;

};

class CEditState : public CActor
{
public:
	CEditState() = default;
	virtual ~CEditState() = default;

public:
	virtual void InitalizeEditState(TTileMapEditContext& InEditContext)
	{
		TileMapEditContext = &InEditContext;
	}
	virtual void EnterEditState() = 0;
	virtual void OnEditState() = 0;
	virtual void ExitEditState() = 0;
	virtual void ToImGUI() {}

	void SetMouseWorldPosition(const Vector2& InMouseWorldPosition) { MouseWorldPosition = InMouseWorldPosition; }
	const Vector2& GetMouseWorldPosition() const { return MouseWorldPosition; }
	TTileMapEditContext& GetTileMapEditContext() const { return *TileMapEditContext; }

private:
	Vector2 MouseWorldPosition;
	TTileMapEditContext* TileMapEditContext = nullptr;

};

