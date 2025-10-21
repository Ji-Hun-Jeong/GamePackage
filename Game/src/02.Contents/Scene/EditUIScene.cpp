#include "pch.h"
#include "EditUIScene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"

void CEditUIScene::Initalize()
{
	CScene::Initalize();
	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	CWindowIOManager* WindowIOManager = GetWorld()->SpawnActor<CWindowIOManager>(this);

	CUI* LoadUI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	LoadUI->GetTransform()->SetPosition(Vector3(500.0f, 300.0f, 0.0f));
	LoadUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/LoadImageUI_Normal.png", L"resources/image/UI/Title/LoadImageUI_Hover.png"
		, L"resources/image/UI/Title/LoadImageUI_Clicked.png", L"resources/image/UI/Title/LoadImageUI_Normal.png", [this, WindowIOManager, LoadUI]()->void
		{
			WindowIOManager->OpenFileDialog();
			CActor* Test = GetWorld()->SpawnActor<CActor>(LoadUI);
			Test->SetRenderComponent();
			Test->GetRenderComponent()->SetDiffuseImage(WindowIOManager->GetOpenFilePath());
		});
	
}
