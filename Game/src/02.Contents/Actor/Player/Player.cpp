#include "pch.h"
#include "Player.h"
#include "GameCore.h"

#include "02.Contents/Skill/Skill.h"

CPlayer::CPlayer()
{
	Transform->SetScale(Vector3(37.0f, 37.0f, 1.0f));

	AddComponent<CSpriteRenderComponent>();
	SetLineActor();
	SpriteRenderComponent->SetColor(Vector3(0.0f, 1.0f, 0.0f), 1.0f);
	AddComponent<CRigidBody>();

	CharacterAnimator = AddComponent<CWzCharacterAnimator>();
	PartsManager = AddComponent<CWzPartsManager>();
	SkillCaster = AddComponent<CSkillCaster>();
}

CPlayer::~CPlayer()
{

}

void CPlayer::BeginPlay()
{
	/*WzLoader.OpenWzData("resources/data/Skill/Skill.2410.img.json");
	JValue* SkillValue;
	if (WzLoader.GetLoadData("skill", &SkillValue))
	{
		SkillLoad::DeSerializeSkillData(*SkillValue, "24101000", &SkillData);
	}*/

	WzLoader.OpenWzData("resources/data/Character/Character.00002000.img.json");

	/*CharacterAnimator->AddAnimation(WzLoader, "walk1");
	CharacterAnimator->AddAnimation(WzLoader, "walk2");
	CharacterAnimator->AddAnimation(WzLoader, "stand1");
	CharacterAnimator->AddAnimation(WzLoader, "stand2");
	CharacterAnimator->AddAnimation(WzLoader, "swingO1");
	CharacterAnimator->AddAnimation(WzLoader, "swingO2");
	CharacterAnimator->AddAnimation(WzLoader, "swingO3");
	CharacterAnimator->SetCurrentAnimation("walk1", true);*/

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));

}

void CPlayer::CaptureSnapShot()
{
	CActor::CaptureSnapShot();

	// 입력받은 문자열을 저장할 버퍼 (정적 변수나 멤버 변수 권장)
	static char inputBuffer[256] = "";

	ImGui::Begin("Snapshot Event");

	// 엔터키를 쳤을 때만 true를 반환하도록 설정
	if (ImGui::InputText("Command", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// 여기서 이벤트 발생!
		std::string command = inputBuffer;

		if (CharacterAnimator->IsExistAnim(command) == false)
		{
			JValue* AnimValue;
			if (WzLoader.GetLoadData(command, &AnimValue))
			{
				TWzCharacterAnimation* Anim = CWzAnimationLoader::GetInst().ParseWzCharacterAnimation(*AnimValue, command);
				if (Anim)
					CharacterAnimator->AddAnimation(*Anim);
			}
		}

		CharacterAnimator->SetCurrentAnimation(command, true);

		// 입력 후 버퍼 비우기 (필요 시)
		memset(inputBuffer, 0, sizeof(inputBuffer));

		// 입력 칸에 계속 포커스를 유지하고 싶다면
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
}
