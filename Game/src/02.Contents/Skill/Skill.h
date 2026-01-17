#pragma once
#include "01.Base/Actor/StaticActor.h"

enum class ESkillType
{
	Active,
	Passive,
	KeyDown,
};

class CSkill
{
public:
	CSkill(ESkillType InSkillType, const std::string& InName, const std::string& InDescription,
		const std::wstring& InIcon)
		: SkillType(InSkillType), Name(InName), Description(InDescription), Icon(InIcon)
	{}
	virtual ~CSkill() = 0 {}
	const std::string& GetName() const { return Name; }
	const std::string& GetDescription() const { return Description; }
	ESkillType GetSkillType() const { return SkillType; }
	const std::wstring& GetIcon() const { return Icon; }

private:
	ESkillType SkillType;
	std::string Name;
	std::string Description;
	std::wstring Icon;
};

class CActiveSkillActor : public CStaticActor
{
	GENERATE_OBJECT(CActiveSkillActor)
public:
	CActiveSkillActor()
	{
		AddComponent<CAnimator>();
	}
	~CActiveSkillActor() = default;

public:
	bool IsEndActive() const
	{
		// Animator
	}

private:
	
};