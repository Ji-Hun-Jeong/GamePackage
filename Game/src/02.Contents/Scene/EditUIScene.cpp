#include "pch.h"
#include "EditUIScene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Actor/UI/UITool/UIToolPanel.h"

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
		, [this, WindowIOManager, MousePointer](EKeyType InKeyType, const Vector2& InMousePosition)->void
		{
			LoadImageFromDialog(InKeyType, *WindowIOManager, *MousePointer);
		});

	CUIToolPanel* UIToolPanel = GetWorld()->SpawnActor<CUIToolPanel>(this);
	UIToolPanel->GetTransform()->SetScale(Vector3(900, 600, 0));

	CUIToolInputState* UIToolInputState = GetWorld()->SpawnActor<CUIToolInputState>(this);
	UIToolInputState->MouseReleaseEvent = [this, MousePointer, UIToolInputState, UIToolPanel](EKeyType InKeyType, const Vector2& InMousePosition)->void
		{
			UIToolInputState->PlaceUIOnToolPanel(InKeyType, MousePointer->GetRenderComponent()->GetCurrentImagePath()
				, InMousePosition, *UIToolPanel);
		};

	UIToolPanel->AddUIToolState(UIToolInputState);
	UIToolPanel->SetCurrentState(UIToolInputState);
}

void CEditUIScene::LoadImageFromDialog(EKeyType InKeyType, CWindowIOManager& InWindowIOManager, CMousePointer& InMousePointer)
{
	if (InKeyType != EKeyType::LButton)
		return;
	if (InWindowIOManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowIOManager.GetOpenFilePath();
		InMousePointer.GetRenderComponent()->SetDiffuseImage(ImagePath);
	}
	else
		InMousePointer.GetRenderComponent()->ResetImage();
}

