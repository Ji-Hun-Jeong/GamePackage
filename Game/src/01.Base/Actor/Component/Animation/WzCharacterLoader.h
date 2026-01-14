#pragma once
#ifdef GetObject
#undef GetObject
#endif
#include "Utils.h"
#include "WzCharacterAnimator.h"
#include "Common/Json.h"
// 애니메이션 재생해달라고 요청하면 그 애니메이션 재생
// 재생을 하려면 어떤 데이터들이 있을거고 그 데이터들을 BoneAnimator한테 넣어줌
// BoneAnimator가 그 데이터를 해석해서 자기가 가진 Part들한테 전달해줌
// 각 파트가 자신의 새로운 위치를 계산함.
// 프레임이 넘어갈 때마다 파츠를 갱신

class CWzNode
{
public:
	CWzNode(const std::string& InName)
		: Name(InName)
	{
	}
	~CWzNode() = default;

public:
	void AddChildNode(const std::string& InNodeName, std::unique_ptr<CWzNode> InChildNode)
	{
		ChildNodes.emplace(InNodeName, std::move(InChildNode));
	}
	CWzNode* GetChildNode(const std::string& InNodeName) const
	{
		auto Iter = ChildNodes.find(InNodeName);
		if (Iter == ChildNodes.end())
			return nullptr;
		return Iter->second.get();
	}
	void SetValue(const std::string& InValue) { Value = InValue; }
	const std::string& GetValue() const { return Value; }

private:
	std::string Name;
	std::string Value;
	std::map<std::string, std::unique_ptr<CWzNode>> ChildNodes;

};

class CWzCharacterLoader
{
	SINGLE(CWzCharacterLoader)
public:
	~CWzCharacterLoader() = default;

public:
	bool OpenWzData(const std::string& InCharacterWzJsonPath)
	{
		FILE* File = nullptr;
		fopen_s(&File, InCharacterWzJsonPath.c_str(), "rb"); // 바이너리 읽기 권장
		if (!File)
			return false;

		char Buffer[65536]; // 데이터가 크므로 버퍼를 조금 더 늘렸습니다.
		rapidjson::FileReadStream JsonReader(File, Buffer, sizeof(Buffer));

		Document.ParseStream(JsonReader);
		fclose(File);

		if (Document.HasParseError()) {
			std::cout << "JSON 파싱 에러!" << std::endl;
			return false;
		}
		return true;
	}
	void CloseWzData()
	{
		Document = rapidjson::Document{};
	}
	CWzNode* LoadWzAnimation(const std::string& InCharacterName, const std::string& InAnimName
		, CWzCharacterAnimator* OutWzCharacterAnimator = nullptr)
	{
		if (WzCharacters.contains(InCharacterName) == false)
			WzCharacters.emplace(InCharacterName, CCharacter{});

		auto Iter = WzCharacters.find(InCharacterName);
		CCharacter& Character = Iter->second;

		if (Character.contains(InAnimName))
			return Character.at(InAnimName).get();

		auto AnimArray = Document["dir"]["dir"].GetArray();
		for (auto& Anim : AnimArray)
		{
			std::string dirName = Anim["@name"].GetString();
			if (dirName == "info")
				continue;
			if (dirName != InAnimName)
				continue;

			std::cout << "--- 애니메이션 발견: " << dirName << " ---" << std::endl;
			auto AnimIter = Character.find(dirName);

			auto WzNode = std::make_unique<CWzNode>(dirName);
			ParseJsonValue(Anim, WzNode.get());

			CWzNode* RawNode = WzNode.get();
			Character.emplace(dirName, std::move(WzNode));

			if (OutWzCharacterAnimator)
			{
				// WzFrameData로 바꿔서 넣어주기
				//OutWzCharacterAnimator->AddWzAnimation(dirName, *RawNode);
			}

			return RawNode;
		}
	}
	void ParseJsonValue(const rapidjson::Value& InValue, CWzNode* OutNode)
	{
		if (InValue.IsObject())
		{
			CWzNode* NewNode = nullptr;
			auto Object = InValue.GetObject();
			for (auto Iter = Object.MemberBegin(); Iter != Object.MemberEnd(); ++Iter)
			{
				std::string Name = Iter->name.GetString();
				auto& Value = Iter->value;
				if (Name == "@name")
				{
					NewNode = new CWzNode(Value.GetString());
					OutNode->AddChildNode(Value.GetString(), std::unique_ptr<CWzNode>(NewNode));
				}
				else if (Name == "@value")
					NewNode->SetValue(Value.GetString());
				else
					ParseJsonValue(Value, NewNode);
			}
		}
		else if (InValue.IsArray())
		{
			auto Arr = InValue.GetArray();
			for (auto& Element : Arr)
				ParseJsonValue(Element, OutNode);
		}
	}
private:
	rapidjson::Document Document;

	using CCharacter = std::map<std::string, std::unique_ptr<CWzNode>>;
	std::map<std::string, CCharacter> WzCharacters;

};

