#pragma once
#include "Common/Json.h"

using JValue = rapidjson::Value;
using JObject = rapidjson::GenericObject<true, JValue>;

class CWzNode
{
public:
	CWzNode() = default;

	// 1. 복사 생성자 (깊은 복사)
	CWzNode(const CWzNode& InOther)
		: Value(InOther.Value)
	{
		for (const auto& [key, node] : InOther.Members)
		{
			if (node)
			{
				// 자식들을 새로 생성해서 깊은 복사 수행
				this->Members[key] = new CWzNode(*node);
			}
		}
	}

	// 2. 복사 대입 연산자
	CWzNode& operator=(const CWzNode& InOther)
	{
		if (this != &InOther)
		{
			Clear(); // 기존 메모리 해제
			Value = InOther.Value;
			for (const auto& [key, node] : InOther.Members)
			{
				if (node) this->Members[key] = new CWzNode(*node);
			}
		}
		return *this;
	}

	// 3. 이동 생성자
	CWzNode(CWzNode&& InOther) noexcept
		: Value(std::move(InOther.Value))
		, Members(std::move(InOther.Members)) // 맵 소유권을 통째로 뺏어옴
	{
		InOther.Members.clear(); // 원본은 안전하게 비움
	}

	// 4. 이동 대입 연산자
	CWzNode& operator=(CWzNode&& InOther) noexcept
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
	virtual ~CWzNode()
	{
		Clear();
	}

	bool IsMember(const std::string_view InName)
	{
		return Members.contains(InName);
	}
	CWzNode& operator[](const std::string_view InName)
	{
		return *Members.find(InName)->second;
	}

public:
	void SetValue(const std::string_view InValue) { Value = InValue; }
	const std::string_view GetValue() const { return Value; }
	void AddMember(const std::string_view InName, CWzNode& InSkillNode) { Members.emplace(InName, &InSkillNode); }

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
	std::map<std::string, CWzNode*, std::less<>> Members;
};

struct TWzPng : public CWzNode
{
	Vector2 Origin;
	int32_t Z = 0;
	int32_t Delay = 0;
	std::wstring OutLink;
};

namespace Wz
{
	extern CWzNode* GenerateWzData(const JValue& InValue);
}

class CWzUtils
{
public:
	static Vector3 GetWorldPositionFromWzPosition(Vector2 InImageScale, Vector2 InWzOrigin);
	static Vector3 GetWorldPositionFromWzPosition(const std::wstring& InImagePath, Vector2 InWzOrigin);
	static Vector3 GetWorldPositionFromWzPosition(const class CSpriteRenderComponent& InSpriteRenderComponent, Vector2 InWzOrigin);
	static bool SetWorldPositionFromWzPosition(class CActor& InActor, Vector2 InWzOrigin);
};