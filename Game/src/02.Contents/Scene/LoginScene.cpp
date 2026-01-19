#include "pch.h"
#include "LoginScene.h"
#include "GameCore.h"

#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Scene/TestScene.h"

void CLoginScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(1.0f);

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");

	CBackGround* LoginBackGround = GetWorld()->SpawnActor<CBackGround>(this);
	LoginBackGround->InitalizeBackGround(L"resources/image/UI/Title/Login/Background.png");

	CBackGround* MapleIdBack = GetWorld()->SpawnActor<CBackGround>(LoginBackGround);
	MapleIdBack->GetTransform()->SetPosition(Vector3(-37.0f, 49.0f, 0.0f));
	MapleIdBack->InitalizeBackGround(L"resources/image/UI/Title/Login/MapleIDBack.png");

	/*COnOffUI* CheckUI = GetWorld()->SpawnActor<COnOffUI>(LoginBackGround);
	CheckUI->GetTransform()->SetPosition(Vector3(74.0f, 49.0f, 0.0f));
	CheckUI->InitalizeOnOffUI(L"resources/image/UI/Title/Login/Tab/Check/Enable/0.png", L"resources/image/UI/Title/Login/Tab/Check/Disable/0.png");*/

	/*CUI* SaveUI = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	SaveUI->GetTransform()->SetPosition(Vector3(112.0f, 49.0f, 0.0f));
	SaveUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/SaveEmailButton/Normal/0.png", L"resources/image/UI/Title/Login/SaveEmailButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/SaveEmailButton/Pressed/0.png"
		, [this, CheckUI]()->void
		{
			CheckUI->ChangeState();
		});*/

	/*CBackGround* PWBack = GetWorld()->SpawnActor<CBackGround>(LoginBackGround);
	PWBack->InitalizeBackGround(L"resources/image/UI/Title/Login/PWBack.png");

	CUI* LoginUI = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	LoginUI->GetTransform()->SetPosition(Vector3(0.0f, -60.0f, 0.0f));
	LoginUI->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/LoginButton/Normal/0.png", L"resources/image/UI/Title/Login/LoginButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/LoginButton/Pressed/0.png"
		, [this]()->void
		{
			LoadNewSceneWithFadeOut(CTestScene::GetStaticClass(), 1.0f);
		});

	CUI* CreateIDButton = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	CreateIDButton->GetTransform()->SetPosition(Vector3(-115.0f, -100.0f, 0.0f));
	CreateIDButton->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/CreateIDButton/Normal/0.png", L"resources/image/UI/Title/Login/CreateIDButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/CreateIDButton/Pressed/0.png", nullptr);

	CUI* FindIDButton = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	FindIDButton->GetTransform()->SetPosition(Vector3(-55.0f, -100.0f, 0.0f));
	FindIDButton->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/FindIDButton/Normal/0.png", L"resources/image/UI/Title/Login/FindIDButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/FindIDButton/Pressed/0.png", nullptr);

	CUI* FindPWButton = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	FindPWButton->GetTransform()->SetPosition(Vector3(5.0f, -100.0f, 0.0f));
	FindPWButton->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/FindPWButton/Normal/0.png", L"resources/image/UI/Title/Login/FindPWButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/FindPWButton/Pressed/0.png", nullptr);

	CUI* HomepageButton = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	HomepageButton->GetTransform()->SetPosition(Vector3(65.0f, -100.0f, 0.0f));
	HomepageButton->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/HomepageButton/Normal/0.png", L"resources/image/UI/Title/Login/HomepageButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/HomepageButton/Pressed/0.png", nullptr);

	CUI* QuitButton = GetWorld()->SpawnActor<CUI>(LoginBackGround);
	QuitButton->GetTransform()->SetPosition(Vector3(115.0f, -100.0f, 0.0f));
	QuitButton->InitalizeBasicButtonUI(L"resources/image/UI/Title/Login/QuitButton/Normal/0.png", L"resources/image/UI/Title/Login/QuitButton/MouseOn/0.png"
		, L"resources/image/UI/Title/Login/QuitButton/Pressed/0.png", nullptr);*/
}

void CLoginScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);
	/*const Vector2& MousePosition = CMouseManager::GetInst().GetMousePosition();
	printf("%f %f\n", MousePosition.x, MousePosition.y);*/
}
