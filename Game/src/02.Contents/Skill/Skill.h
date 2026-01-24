#pragma once
#include "01.Base/Actor/StaticActor.h"

using JValue = rapidjson::Value;
using JObject = rapidjson::GenericObject<true, JValue>;

class CSkillNode
{
public:
	CSkillNode() = default;

	// 1. 복사 생성자 (깊은 복사)
	CSkillNode(const CSkillNode& InOther)
		: Value(InOther.Value)
	{
		for (const auto& [key, node] : InOther.Members)
		{
			if (node)
			{
				// 자식들을 새로 생성해서 깊은 복사 수행
				this->Members[key] = new CSkillNode(*node);
			}
		}
	}

	// 2. 복사 대입 연산자
	CSkillNode& operator=(const CSkillNode& InOther)
	{
		if (this != &InOther)
		{
			Clear(); // 기존 메모리 해제
			Value = InOther.Value;
			for (const auto& [key, node] : InOther.Members)
			{
				if (node) this->Members[key] = new CSkillNode(*node);
			}
		}
		return *this;
	}

	// 3. 이동 생성자
	CSkillNode(CSkillNode&& InOther) noexcept
		: Value(std::move(InOther.Value))
		, Members(std::move(InOther.Members)) // 맵 소유권을 통째로 뺏어옴
	{
		InOther.Members.clear(); // 원본은 안전하게 비움
	}

	// 4. 이동 대입 연산자
	CSkillNode& operator=(CSkillNode&& InOther) noexcept
	{
		if (this != &InOther)
		{
			Clear();
			Value = std::move(InOther.Value);
			Members = std::move(InOther.Members);
			InOther.Members.clear();
		}
		return *this;
	}

	// 5. 소멸자
	virtual ~CSkillNode()
	{
		Clear();
	}

	bool IsMember(const std::string_view InName)
	{
		return Members.contains(InName);
	}
	CSkillNode& operator[](const std::string_view InName)
	{
		return *Members.find(InName)->second;
	}

public:
	void SetValue(const std::string_view InValue) { Value = InValue; }
	const std::string_view GetValue() const { return Value; }
	void AddMember(const std::string_view InName, CSkillNode& InSkillNode) { Members.emplace(InName, &InSkillNode); }

private:
	void Clear()
	{
		for (auto& [key, node] : Members)
		{
			delete node; // 재귀적으로 자식들의 소멸자가 호출됨
		}
		Members.clear();
	}

protected:
	std::string Value;
	std::map<std::string, CSkillNode*, std::less<>> Members;
};
struct TSkillPng : public CSkillNode
{
	Vector2 Origin;
	int32_t Z = 0;
	int32_t Delay = 0;
	std::wstring OutLink;
};

struct TSkillCommon : public CSkillNode
{
	int32_t MaxLavel = 0;
	std::string MpCon;
	std::string MobCount;
	std::string Damage;
	std::string AttackCount;
	Vector2 LeftTop;
	Vector2 RightBottom;
};

struct TSkillInfo : public CSkillNode
{
	int32_t Type = 0;
	int32_t AreaAttack = 0;
};

struct TSkillHit : public CSkillNode
{
	std::vector<TSkillPng> Anim;
};

struct TSkillEffect : public CSkillNode
{
	std::vector<TSkillPng> Anim;
};

struct TSkillData : public CSkillNode
{
	TSkillPng Icon;
	TSkillPng IconMouseOver;
	TSkillPng IconDisabled;

	TSkillCommon Common;
	std::vector<TSkillHit> Hit;
	TSkillEffect Effect;
	TSkillInfo Info;

	int32_t Weapon = 0;
	int32_t SubWeapon = 0;
};

namespace SkillLoad
{
	extern CSkillNode* ParseWzData(const JValue& InValue);
	extern bool DeSerializeSkillData(const rapidjson::Value& InValue, const std::string_view InSkillCode
		, TSkillData* OutSkillData);
}
