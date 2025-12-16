#include "pch.h"
#include "LadderEditor.h"

#include "GameCore.h"
#include <random>

void CLadderEditor::StretchToUp(CLadderForm& InLadder)
{
	if (IsEditReady() == false)
		return;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, (int)BodyImagePaths.size() - 1);

	int r = dist(gen);

	CStaticActor* TopBody = InLadder.Bodys.front();

	const std::wstring& BodyImage = BodyImagePaths[r];
	CStaticActor* NewBody = GetWorld()->SpawnActor<CStaticActor>(&InLadder);
	NewBody->GetSpriteRenderComponent()->SetDiffuseImage(BodyImage);
	Vector2 NewBodyImageScale = NewBody->GetSpriteRenderComponent()->GetImageScale();
	InLadder.Bodys.push_front(NewBody);

	Vector3 NewBodyPosition = CTransformUtils::GetTopPosition(*TopBody, *NewBody);
	NewBody->GetTransform()->SetPosition(NewBodyPosition);

	Vector3 HeadPosition = CTransformUtils::GetTopPosition(*NewBody, *InLadder.Head);
	InLadder.Head->GetTransform()->SetPosition(HeadPosition);

	ReBuild(InLadder, *NewBody);
}

void CLadderEditor::StretchToDown(CLadderForm& InLadder)
{
	if (IsEditReady() == false)
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, (int)BodyImagePaths.size() - 1);

	int r = dist(gen);

	CStaticActor* BottomBody = InLadder.Bodys.back();

	const std::wstring& BodyImage = BodyImagePaths[r];
	CStaticActor* NewBody = GetWorld()->SpawnActor<CStaticActor>(&InLadder);
	NewBody->GetSpriteRenderComponent()->SetDiffuseImage(BodyImage);
	Vector2 NewBodyImageScale = NewBody->GetSpriteRenderComponent()->GetImageScale();
	InLadder.Bodys.push_back(NewBody);

	Vector3 NewBodyPosition = CTransformUtils::GetBottomPosition(*BottomBody, *NewBody);
	NewBody->GetTransform()->SetPosition(NewBodyPosition);

	Vector3 FootPosition = CTransformUtils::GetBottomPosition(*NewBody, *InLadder.Foot);
	InLadder.Foot->GetTransform()->SetPosition(FootPosition);

	ReBuild(InLadder, *NewBody);
}

CLadderForm* CLadderEditor::CreateLadder(const Vector3& InPosition)
{
	CLadderForm* Ladder = GetWorld()->SpawnActor<CLadderForm>(nullptr);
	Ladder->GetTransform()->SetPosition(InPosition);

	const std::wstring& FirstBodyImage = BodyImagePaths[0];
	CStaticActor* Body = GetWorld()->SpawnActor<CStaticActor>(Ladder);
	Body->GetSpriteRenderComponent()->SetDiffuseImage(FirstBodyImage);
	Ladder->AddBody(*Body);

	CStaticActor* Head = GetWorld()->SpawnActor<CStaticActor>(Ladder);
	Head->GetSpriteRenderComponent()->SetDiffuseImage(HeadImagePath);
	Vector3 HeadPosition = CTransformUtils::GetTopPosition(*Body, *Head);
	Head->GetTransform()->SetPosition(HeadPosition);
	Ladder->SetHead(*Head);

	CStaticActor* Foot = GetWorld()->SpawnActor<CStaticActor>(Ladder);
	Foot->GetSpriteRenderComponent()->SetDiffuseImage(FootImagePath);
	Vector3 FootPosition = CTransformUtils::GetBottomPosition(*Body, *Foot);
	Foot->GetTransform()->SetPosition(FootPosition);
	Ladder->SetFoot(*Foot);

	Vector2 HeadScale = Head->GetSpriteRenderComponent()->GetImageScale();
	Vector2 BodyScale = Body->GetSpriteRenderComponent()->GetImageScale();
	Vector2 FootScale = Foot->GetSpriteRenderComponent()->GetImageScale();
	Ladder->GetTransform()->SetScale(Vector3(BodyScale.x, HeadScale.y + BodyScale.y + FootScale.y, 1.0f));

	ManagingLadders.push_back(Ladder);
	return Ladder;
}