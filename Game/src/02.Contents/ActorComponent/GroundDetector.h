#pragma once
#include "01.Base/Actor/Actor.h"
#include "02.Contents/Actor/Manager/GroundManager.h"
#include "Utils.h"

class CGroundDetector : public CActor
{
	GENERATE_OBJECT(CGroundDetector)
public:
	CGroundDetector();
	~CGroundDetector() = default;

public:
	void OnCollisionEnter(CCollider& InCollider) override
	{
		if (InCollider.GetOwnerActor()->GetType() == CGround::GetStaticType())
			Ground = static_cast<CGround*>(InCollider.GetOwnerActor());
	}
	void OnCollisionExit(CCollider& InCollider) override
	{
		if (InCollider.GetOwnerActor()->GetType() == CGround::GetStaticType())
			Ground = nullptr;
	}

	void SetDetectScale(const Vector2& InDetectScale) { DetectCollider->SetRectScale(InDetectScale); }

	bool IsOnGround() const { return Ground; }

	void AdjustPlayerPosition(class CPlayer& InPlayer);

private:
	CRectCollider* DetectCollider = nullptr;

	const CGround* Ground = nullptr;

};

