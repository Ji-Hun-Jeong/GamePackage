#include "pch.h"
#include "Player.h"
#include "GameCore.h"
#include "02.Contents/Manager/CharacterLoader.h"

CPlayer::CPlayer()
{
	Transform->SetScale(Vector3(37.0f, 37.0f, 1.0f));

	AddComponent<CSpriteRenderComponent>();
	SetLineActor();
	SpriteRenderComponent->SetColor(Vector3(0.0f, 1.0f, 0.0f), 1.0f);

	AddComponent<CRigidBody>();

	AnimatorController = AddComponent<CAnimatorController>();
	Character = AddComponent<CCharacter>();
}

CPlayer::~CPlayer()
{
	
}


void CPlayer::BeginPlay()
{
	// BodyImage Center = 14 16, 13 16
	// Body.Navel = 13 12, 10 12
	// Arm.Navel = -6 6
	//Vector3 MyArmPosition = Vector3(-1, 4, 0) + Vector3(12, 2, 0);
	//Vector3 MyArmPosition1 = Vector3(-3, 4, 0) + Vector3(6, -1, 0);
	//{
	//	Body = GetWorld()->SpawnActor<CStaticActor>(this);
	//	Body->AddComponent<CAnimator>();

	//	auto Animation = std::make_unique<CAnimation>(true);
	//	TFrame Frame;
	//	Frame.ImagePath = L"resources/image/Character/walk/01/Body0.png";
	//	Animation->AddFrame(Frame);
	//	Frame.ImagePath = L"resources/image/Character/walk/01/Body1.png";
	//	Animation->AddFrame(Frame);
	//	Frame.ImagePath = L"resources/image/Character/walk/01/Body2.png";
	//	Animation->AddFrame(Frame);
	//	Frame.ImagePath = L"resources/image/Character/walk/01/Body3.png";
	//	Animation->AddFrame(Frame);
	//	Animation->UnifyFrameDuration(1.0f);

	//	Body->GetAnimator()->AddAnimation("Walk", std::move(Animation));
	//	AnimatorController->AddAnimator(*Body->GetAnimator());
	//}

	//
	//{
	//	Arm = GetWorld()->SpawnActor<CStaticActor>(this);
	//	Arm->AddComponent<CAnimator>();
	//	//Head = GetWorld()->SpawnActor<CStaticActor>(this);
	//	//Head->AddComponent<CAnimator>();
	//	CAnimation* BodyWalkAnimation = Body->GetAnimator()->GetAnimation("Walk");
	//	struct TAnimData
	//	{
	//		Vector3 BodyOrigin;
	//		Vector3 BodyNeck;
	//		Vector3 BodyNavel;
	//		Vector2 BodyImageCenter;
	//		Vector3 ArmNavel;
	//	};
	//	TAnimData AnimDatas[4];
	//	AnimDatas[0].BodyOrigin = Vector3(19.0f, 32.0f, 0.0f);
	//	AnimDatas[0].BodyNeck = Vector3(-4.0f, -32.0f, 0.0f);
	//	AnimDatas[0].BodyNavel = Vector3(-6.0f, -20.0f, 0.0f);
	//	AnimDatas[0].ArmNavel = Vector3(-12.0f, 2.0f, 0.0f);
	//	AnimDatas[0].BodyImageCenter = CRenderResourceLoader::GetInst().GetImage(BodyWalkAnimation->GetFrame(0).ImagePath)->GetTexture2D().GetTextureSize() / 2.0f;

	//	AnimDatas[1].BodyOrigin = Vector3(16.0f, 32.0f, 0.0f);
	//	AnimDatas[1].BodyNeck = Vector3(-4.0f, -31.0f, 0.0f);
	//	AnimDatas[1].BodyNavel = Vector3(-6.0f, -20.0f, 0.0f);
	//	AnimDatas[1].ArmNavel = Vector3(-6, -1.0f, 0.0f);
	//	AnimDatas[1].BodyImageCenter = CRenderResourceLoader::GetInst().GetImage(BodyWalkAnimation->GetFrame(1).ImagePath)->GetTexture2D().GetTextureSize() / 2.0f;

	//	AnimDatas[2].BodyOrigin = Vector3(19.0f, 32.0f, 0.0f);
	//	AnimDatas[2].BodyNeck = Vector3(-4.0f, -32.0f, 0.0f);
	//	AnimDatas[2].BodyNavel = Vector3(-6.0f, -20.0f, 0.0f);
	//	AnimDatas[2].ArmNavel = Vector3(-12.0f, 2.0f, 0.0f);
	//	AnimDatas[2].BodyImageCenter = CRenderResourceLoader::GetInst().GetImage(BodyWalkAnimation->GetFrame(2).ImagePath)->GetTexture2D().GetTextureSize() / 2.0f;

	//	AnimDatas[3].BodyOrigin = Vector3(21.0f, 31.0f, 0.0f);
	//	AnimDatas[3].BodyNeck = Vector3(-4.0f, -31.0f, 0.0f);
	//	AnimDatas[3].BodyNavel = Vector3(-6.0f, -18.0f, 0.0f);
	//	AnimDatas[3].ArmNavel = Vector3(-13.0f, 4.0f, 0.0f);
	//	AnimDatas[3].BodyImageCenter = CRenderResourceLoader::GetInst().GetImage(BodyWalkAnimation->GetFrame(3).ImagePath)->GetTexture2D().GetTextureSize() / 2.0f;

	//	auto Animation = std::make_unique<CAnimation>(true);
	//	TFrame Frame;
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		Frame.ImagePath = L"resources/image/Character/walk/01/Arm";
	//		Frame.ImagePath += std::to_wstring(i);
	//		Frame.ImagePath += L".png";
	//		
	//		const Vector3& BodyOrigin = AnimDatas[i].BodyOrigin;
	//		const Vector3& BodyNavel = AnimDatas[i].BodyNavel;
	//		const Vector2& BodyImageCenter = AnimDatas[i].BodyImageCenter;
	//		const Vector3& ArmNavel = AnimDatas[i].ArmNavel;
	//		// Head = (BodyOrigin + BodyNeck) - Vector3(BodyImageCenter.x, BodyImageCenter.y, 0.0f)
	//		Vector3 ArmPosition = Vector3(1.0f, -1.0f, 1.0f) * ((BodyOrigin + BodyNavel) - Vector3(BodyImageCenter.x, BodyImageCenter.y, 0.0f))
	//			- Vector3(1.0f, -1.0f, 1.0f) * ArmNavel;
	//		Frame.Offset = ArmPosition;

	//		Animation->AddFrame(Frame);
	//	}
	//	
	//	Animation->UnifyFrameDuration(1.0f);

	//	Arm->GetAnimator()->AddAnimation("Walk", std::move(Animation));
	//	AnimatorController->AddAnimator(*Arm->GetAnimator());
	//}
	CCharacterLoader& Loader = CCharacterLoader::GetInst();
	Loader.OpenCharacterData("resources/data/Character/Character.00002000.img.json");
	Loader.InitalizeCharacter(*Character);

	Loader.LoadCharacterAnimation("Base", "walk1", Character);
	Loader.LoadCharacterAnimation("Base", "walk2", Character);
	Loader.LoadCharacterAnimation("Base", "stand1", Character);
	Loader.LoadCharacterAnimation("Base", "stand2", Character);
	Loader.LoadCharacterAnimation("Base", "swingO1", Character);
	Loader.LoadCharacterAnimation("Base", "swingO2", Character);
	Loader.LoadCharacterAnimation("Base", "swingO3", Character);
	//Loader.LoadCharacterAnimation("Base", "swingOF", Character);

	Loader.CloseCharacterData("Base");

	Character->InfiniteAnimation("walk1");
	Character->InfiniteAnimation("walk2");
	Character->InfiniteAnimation("stand1");
	Character->InfiniteAnimation("stand2");
	Character->InfiniteAnimation("swingO1");
	Character->InfiniteAnimation("swingO2");
	Character->InfiniteAnimation("swingO3");
	//Character->InfiniteAnimation("swingOF");

	Character->PlayAnimation("swingO1");	

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
}
