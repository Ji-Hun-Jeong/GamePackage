#include "pch.h"
#include "LobbyScene.h"

#include "01.Base/Actor/Character.h"
#include "01.Base/Actor/UI.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Scene/EditUIScene.h"

void CLobbyScene::Initalize()
{
	CScene::Initalize();
	//CBackGround* BackGround = SpawnActor<CBackGround>(this);
	//BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	//CUI* UI = SpawnActor<CUI>(GetMainCamera());
	//UI->InitalizeBasicButtonUI(L"resources/image/UI/Title/SceneToolUI_Normal.png", L"resources/image/UI/Title/SceneToolUI_Hover.png"
	//	, L"resources/image/UI/Title/SceneToolUI_Clicked.png", L"resources/image/UI/Title/SceneToolUI_Normal.png"
	//	, [this](EKeyType InKeyType, const Vector2& InMousePosition)->void
	//	{
	//		if (InKeyType == EKeyType::LButton)
	//			LoadScene<CEditUIScene>();
	//	});
	//
	//CMousePointer* MousePointer = SpawnActor<CMousePointer>(GetMainCamera());
}