#include "pch.h"
#include "LadderEditor.h"

#include "GameCore.h"
#include <random>

void CLadder::InitalizeLadder(const std::wstring& InHeadImagePath, const std::vector<std::wstring>& InBodyImagePath, const std::wstring& InFootImagePath)
{
	HeadImagePath = InHeadImagePath;
	BodyImagePaths = InBodyImagePath;
	FootImagePath = InFootImagePath;

	const std::wstring& FirstBodyImage = InBodyImagePath[0];
	CStaticActor* Body = GetWorld()->SpawnActor<CStaticActor>(this);
	Body->GetSpriteRenderComponent()->SetDiffuseImage(FirstBodyImage);
	Bodys.push_back(Body);

	Head = GetWorld()->SpawnActor<CStaticActor>(this);
	Head->GetSpriteRenderComponent()->SetDiffuseImage(InHeadImagePath);
	Vector3 HeadPosition;
	CTransformUtils::GetTopPosition(*Body, *Head, &HeadPosition);
	Head->GetTransform()->SetPosition(HeadPosition);

	Foot = GetWorld()->SpawnActor<CStaticActor>(this);
	Foot->GetSpriteRenderComponent()->SetDiffuseImage(InFootImagePath);
	Vector3 FootPosition;
	CTransformUtils::GetBottomPosition(*Body, *Foot, &FootPosition);
	Foot->GetTransform()->SetPosition(FootPosition);

	Vector2 HeadScale = Head->GetSpriteRenderComponent()->GetImageScale();
	Vector2 BodyScale = Body->GetSpriteRenderComponent()->GetImageScale();
	Vector2 FootScale = Foot->GetSpriteRenderComponent()->GetImageScale();
	Transform->SetScale(Vector3(BodyScale.x, HeadScale.y + BodyScale.y + FootScale.y, 1.0f));
}

void CLadder::StretchToUp()
{
	if (BodyImagePaths.empty()) return; 
	std::random_device rd;                          
	std::mt19937 gen(rd());                         
	std::uniform_int_distribution<int> dist(0, (int)BodyImagePaths.size() - 1);

	int r = dist(gen);
	
	CStaticActor* TopBody = Bodys.front();

	const std::wstring& BodyImage = BodyImagePaths[r];
	CStaticActor* NewBody = GetWorld()->SpawnActor<CStaticActor>(this);
	NewBody->GetSpriteRenderComponent()->SetDiffuseImage(BodyImage);
	Vector2 NewBodyImageScale = NewBody->GetSpriteRenderComponent()->GetImageScale();
	Bodys.push_front(NewBody);

	Vector3 NewBodyPosition;
	CTransformUtils::GetTopPosition(*TopBody, *NewBody, &NewBodyPosition);
	NewBody->GetTransform()->SetPosition(NewBodyPosition);
	
	Vector3 HeadPosition;
	CTransformUtils::GetTopPosition(*NewBody, *Head, &HeadPosition);
	Head->GetTransform()->SetPosition(HeadPosition);

	ReBuild(*NewBody);
}

void CLadder::StretchToDown()
{
	if (BodyImagePaths.empty()) return; // 예외처리
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, (int)BodyImagePaths.size() - 1);

	int r = dist(gen);

	CStaticActor* BottomBody = Bodys.back();

	const std::wstring& BodyImage = BodyImagePaths[r];
	CStaticActor* NewBody = GetWorld()->SpawnActor<CStaticActor>(this);
	NewBody->GetSpriteRenderComponent()->SetDiffuseImage(BodyImage);
	Vector2 NewBodyImageScale = NewBody->GetSpriteRenderComponent()->GetImageScale();
	Bodys.push_back(NewBody);

	Vector3 NewBodyPosition;
	CTransformUtils::GetBottomPosition(*BottomBody, *NewBody, &NewBodyPosition);
	NewBody->GetTransform()->SetPosition(NewBodyPosition);

	Vector3 FootPosition;
	CTransformUtils::GetBottomPosition(*NewBody, *Foot, &FootPosition);
	Foot->GetTransform()->SetPosition(FootPosition);

	ReBuild(*NewBody);
}
