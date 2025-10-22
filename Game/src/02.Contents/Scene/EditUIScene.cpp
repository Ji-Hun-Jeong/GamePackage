#include "pch.h"
#include "EditUIScene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"
#include "02.Contents/Actor/Base/MousePointer.h"

void CEditUIScene::Initalize()
{
	CScene::Initalize();

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");
	BackGround->SetInteractionComponent();

	CWindowIOManager* WindowIOManager = GetWorld()->SpawnActor<CWindowIOManager>(this);
	CMousePointer* MousePointer = GetWorld()->SpawnActor<CMousePointer>(GetMainCamera());

	CUI* LoadUI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	LoadUI->GetTransform()->SetPosition(Vector3(500.0f, 300.0f, 0.0f));
	LoadUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/LoadImageUI_Normal.png", L"resources/image/UI/Title/LoadImageUI_Hover.png"
		, L"resources/image/UI/Title/LoadImageUI_Clicked.png", L"resources/image/UI/Title/LoadImageUI_Normal.png"
		, [this, WindowIOManager, MousePointer]()->void
		{
			if (WindowIOManager->TryOpenFileDialog())
			{
				const std::wstring& ImagePath = WindowIOManager->GetOpenFilePath();
				MousePointer->GetRenderComponent()->SetDiffuseImage(ImagePath);
			}
			else
				MousePointer->GetRenderComponent()->ResetImage();
		});

	BackGround->GetInteractionComponent()->AddChildInteractionComponent(LoadUI->GetInteractionComponent());
	BackGround->GetInteractionComponent()->SetMouseReleaseEvent([this, MousePointer, BackGround]()->void
		{
			const std::wstring& CurrentImagePath = MousePointer->GetRenderComponent()->GetCurrentImagePath();
			if (CurrentImagePath.empty())
				return;

			const CMousePosition* MouseWorldPosition = MousePointer->GetMousePosition();
			Vector3 MousePosition = Vector3(MouseWorldPosition->GetMousePosition().x, MouseWorldPosition->GetMousePosition().y, 0.0f);
			CUI* NewUI = GetWorld()->SpawnActor<CUI>(BackGround);
			NewUI->GetTransform()->SetPosition(MousePosition);
			NewUI->GetRenderComponent()->SetDiffuseImage(CurrentImagePath);

		});
}
