#include "pch.h"
#include "LobbyScene.h"
#include "GameCore.h"

#include "01.Base/Actor/UI.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Scene/EditUIScene.h"

CLobbyScene::CLobbyScene()
{
	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	CUI* UI = GetWorld()->SpawnActor<CUI>(GetMainCamera());
	UI->InitalizeBasicButtonUI(L"resources/image/UI/Title/SceneToolUI_Normal.png", L"resources/image/UI/Title/SceneToolUI_Hover.png"
		, L"resources/image/UI/Title/SceneToolUI_Clicked.png"
		, [this]()->void
		{
			GetWorld()->LoadScene<CEditUIScene>();
		});
}
