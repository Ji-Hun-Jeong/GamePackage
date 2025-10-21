#include "pch.h"
#include "LobbyScene.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/Character.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Scene/EditUIScene.h"

void CLobbyScene::Initalize()
{
	CScene::Initalize();
	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	CUI* UI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	UI->InitalizeBasicButtonUI(L"resources/image/UI/Title/SceneToolUI_Normal.png", L"resources/image/UI/Title/SceneToolUI_Hover.png"
		, L"resources/image/UI/Title/SceneToolUI_Clicked.png", L"resources/image/UI/Title/SceneToolUI_Normal.png"
		, [this]()->void
		{
			GetWorld()->LoadScene<CEditUIScene>();
		});
	
	CMousePointer* MousePointer = GetWorld()->SpawnActor<CMousePointer>(GetMainCamera());
}