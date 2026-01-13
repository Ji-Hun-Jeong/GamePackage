#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"
#include "01.Base/Actor/Component/Animation/Animator.h"
#include "Common/Json.h"

class CPart : public CStaticActor
{
	GENERATE_OBJECT(CPart)
public:
	CPart()
	{
	}
	~CPart() = default;

public:
	// Origin
	void SetOrigin(const Vector2& InOrigin) { Origin = InOrigin; }
	const Vector2& GetOrigin() const { return Origin; }

	// Neck
	void SetNeck(const Vector2& InNeck) { Neck = InNeck; }
	const Vector2& GetNeck() const { return Neck; }

	// Navel
	void SetNavel(const Vector2& InNavel) { Navel = InNavel; }
	const Vector2& GetNavel() const { return Navel; }

	// Hand
	void SetHand(const Vector2& InHand) { Hand = InHand; }
	const Vector2& GetHand() const { return Hand; }

private:
	Vector2 Origin;
	Vector2 Neck;
	Vector2 Navel;
	Vector2 Hand;
};

class CCharacter : public CComponent
{
	GENERATE_OBJECT(CCharacter)
public:
	CCharacter() = default;
	~CCharacter() = default;

public:
	void AddPart(const std::string& InPartName, CPart* InPart = nullptr);
	CPart* GetPart(const std::string& InPartName) const
	{
		auto Iter = Parts.find(InPartName);
		if (Iter == Parts.end())
			return nullptr;
		return Iter->second;
	}
	void RemovePart(const std::string& InPartName)
	{
		if (Parts.contains(InPartName))
			Parts.erase(InPartName);
	}
	void PlayAnimation(const std::string& InAnimName)
	{
		for (auto& PartPair : Parts)
		{
			CPart* Part = PartPair.second;
			CAnimator* Animator = Part->GetAnimator();
			if (Animator == nullptr)
				continue;
			if(Animator->GetAnimation(InAnimName) == nullptr)
				continue;
			Animator->SetCurrentAnimation(InAnimName);
		}
	}

private:
	std::unordered_map<std::string, CPart*> Parts;

};

