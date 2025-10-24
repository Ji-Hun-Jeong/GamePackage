#include "pch.h"
#include "EditUIScene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Actor/UI/UITool/UIToolPanelManager.h"

void CEditUIScene::Initalize()
{
	CScene::Initalize();

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	CWindowIOManager* WindowIOManager = GetWorld()->SpawnActor<CWindowIOManager>(this);
	CMousePointer* MousePointer = GetWorld()->SpawnActor<CMousePointer>(GetMainCamera());

	CUI* LoadUI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	LoadUI->GetTransform()->SetPosition(Vector3(500.0f, 300.0f, 0.0f));
	LoadUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/LoadImageUI_Normal.png", L"resources/image/UI/Title/LoadImageUI_Hover.png"
		, L"resources/image/UI/Title/LoadImageUI_Clicked.png", L"resources/image/UI/Title/LoadImageUI_Normal.png"
		, [MousePointer, WindowIOManager](EKeyType InKeyType, const Vector2& InMousePosition)->void
		{
			if (InKeyType == EKeyType::LButton)
				MousePointer->SetMouseImageFromDialog(*WindowIOManager);
		});

	CUIToolPanel* UIToolPanel = GetWorld()->SpawnActor<CUIToolPanel>(this);
	UIToolPanel->GetTransform()->SetScale(Vector3(900, 600, 0));

	CUIToolPanelManager* UIToolPanelManager = GetWorld()->SpawnActor<CUIToolPanelManager>(this);
	UIToolPanelManager->InitUIToolPanelManager(*UIToolPanel);

	CUIToolInputState* InputState = new CUIToolInputState(*LoadUI, *WindowIOManager, *MousePointer);
	UIToolPanelManager->AddUIToolState(std::unique_ptr<CUIToolInputState>(InputState));
	CUIToolMoveState* MoveState = new CUIToolMoveState;
	UIToolPanelManager->AddUIToolState(std::unique_ptr<CUIToolMoveState>(MoveState));

	UIToolPanelManager->SetCurrentUIToolState(InputState);

	CUI* ChangeStateUI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	ChangeStateUI->GetTransform()->SetPosition(Vector3(500.0f, 100.0f, 0.0f));
	ChangeStateUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/PanelMove_Normal.png", L"resources/image/UI/Title/PanelMove_Hover.png"
		, L"resources/image/UI/Title/PanelMove_Clicked.png", L"resources/image/UI/Title/PanelMove_Normal.png"
		, [UIToolPanelManager, MoveState](EKeyType InKeyType, const Vector2& InMousePosition)->void
		{
			UIToolPanelManager->SetCurrentUIToolState(MoveState);
		});

	// Todo: Move포커싱을 위에 있는 Interaction이 가져가서 Move가 뚝뚝 끊기는 현상 수정
}