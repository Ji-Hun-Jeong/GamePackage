#pragma once
#ifdef GetObject
#undef GetObject
#endif
#include "02.Contents/Component/Character.h"
#include "Utils.h"

struct TMap
{
	Vector2 Neck;
	Vector2 Navel;
	Vector2 Hand;
};

struct TPartData
{
	std::string Value;
	Vector2 Origin;
	TMap Map;
	std::string Z;
	std::string Group;
	std::string OutLink;
};

struct TKeyFrameData
{
	TPartData Body;
	TPartData Arm;
	TPartData Hand;
	TPartData ArmOverHair;
	TPartData LHand;
	TPartData RHand;

	std::string Action;
	int16_t Face;
	int32_t Delay;
	int32_t Frame;
	Vector2 Move;
};

class CAnimData
{
public:
	CAnimData(size_t InNumOfFrame)
	{
		KeyFrameDatas.resize(InNumOfFrame);
	}
	~CAnimData() = default;

public:
	TKeyFrameData& GetKeyFrame(size_t InIndex)
	{
		if (KeyFrameDatas.size() <= InIndex)
			assert(0);

		return KeyFrameDatas[InIndex];
	}
	void Clear()
	{
		KeyFrameDatas.clear();
	}

private:
	std::vector<TKeyFrameData> KeyFrameDatas;

};

class CCharacterData
{
public:
	CCharacterData() = default;
	~CCharacterData() = default;

public:
	CAnimData& GetAnimDataRef(const std::string& InAnimName)
	{
		auto Iter = AnimDatas.find(InAnimName);
		if (Iter == AnimDatas.end())
			assert(0);
		return Iter->second;
	}
	void AddAnimData(const std::string& InAnimName, size_t InNumOfFrame)
	{
		if (AnimDatas.contains(InAnimName))
			return;
		AnimDatas.emplace(InAnimName, InNumOfFrame);
	}
	void RemoveAnimData(const std::string& InAnimName)
	{
		AnimDatas.erase(InAnimName);
	}
	size_t GetAnimDataSize() const { return AnimDatas.size(); }

private:
	std::unordered_map<std::string, CAnimData> AnimDatas;


};

class CCharacterLoader
{
	SINGLE(CCharacterLoader)
public:
	~CCharacterLoader() = default;

public:
	void LoadCharacter(const std::string& InCharacterName, const std::string& InCharacterWzJsonPath, CCharacter* OutCharacter = nullptr)
	{
		if (CharacterDatas.contains(InCharacterName))
		{
			if (OutCharacter)
			{
				CoverToCharacter(*OutCharacter);
				return;
			}
		}
		else
			CharacterDatas.emplace(InCharacterName, CCharacterData{});

		auto Iter = CharacterDatas.find(InCharacterName);
		CurrentEditCharacterData = &Iter->second;
		assert(CurrentEditCharacterData);

		FILE* File = nullptr;
		fopen_s(&File, InCharacterWzJsonPath.c_str(), "rb"); // 바이너리 읽기 권장
		if (!File) return;

		char Buffer[65536]; // 데이터가 크므로 버퍼를 조금 더 늘렸습니다.
		rapidjson::FileReadStream JsonReader(File, Buffer, sizeof(Buffer));

		rapidjson::Document Document;
		Document.ParseStream(JsonReader);
		fclose(File);

		if (Document.HasParseError()) {
			std::cout << "JSON 파싱 에러!" << std::endl;
			return;
		}
		auto AnimBundle = Document["dir"]["dir"].GetArray();

		for (auto& Anim : AnimBundle)
		{
			// 1. 현재 노드의 이름을 확인해봅니다.
			std::string dirName = Anim["@name"].GetString();

			// 만약 "info"라면 애니메이션 데이터가 아니니 건너뜁니다.
			if (dirName == "info") continue;

			std::cout << "--- 애니메이션 발견: " << dirName << " ---" << std::endl;

			// 2. 애니메이션 노드(walk1 등) 안에는 "dir"이라는 배열이 프레임들을 들고 있습니다.
			if (Anim.HasMember("dir") && Anim["dir"].IsArray())
			{
				auto Frames = Anim["dir"].GetArray();
				CurrentEditCharacterData->AddAnimData(dirName, Frames.Size());
				CurrentEditAnimData = &CurrentEditCharacterData->GetAnimDataRef(dirName);
				for (size_t i = 0; i < Frames.Size(); ++i)
				{
					auto& Frame = Frames[i];
					CurrentEditKeyFrameData = &CurrentEditAnimData->GetKeyFrame(i);
					ParseJsonValue(Frame);
					if (bEmpty)
					{
						CurrentEditCharacterData->RemoveAnimData(dirName);
						break;
					}
				}
			}
			CurrentEditAnimData = nullptr;
			CurrentEditPartData = nullptr;
			CurrentEditKeyFrameData = nullptr;
			bEmpty = false;
			if (100 <= CurrentEditCharacterData->GetAnimDataSize())
				break;
		}
		CurrentEditCharacterData = nullptr;
	}
	void CoverToCharacter(CCharacter& OutCharacter)
	{

	}

private:
	void ParseObject(const rapidjson::GenericObject<true, rapidjson::Value>& InObject)
	{
		if (InObject.HasMember("@name") && InObject.HasMember("@value"))
		{
			const std::string& NodeName = InObject["@name"].GetString();
			const std::string& NodeValue = InObject["@value"].GetString();
			ParseNode(NodeName, NodeValue);
		}

		for (auto Iter = InObject.MemberBegin(); Iter != InObject.MemberEnd(); ++Iter)
		{
			auto& Name = Iter->name;
			auto& Value = Iter->value;
			if (Value.IsObject() || Value.IsArray())
				ParseJsonValue(Value);
		}
	}
	void ParseArray(const rapidjson::GenericArray<true, rapidjson::Value>& InArray)
	{
		for (auto& Element : InArray)
		{
			if (Element.IsObject())
			{
				auto Object = Element.GetObject();
				ParseObject(Object);
			}
		}
	}
	void ParseJsonValue(const rapidjson::Value& InValue)
	{
		if (InValue.IsObject())
			ParseObject(InValue.GetObject());
		else if (InValue.IsArray())
			ParseArray(InValue.GetArray());
	}

	void ParseNode(const std::string& InNodeName, const std::string& InValue)
	{
		if (InNodeName == "body")
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->Body;
			CurrentEditPartData->Value = InValue;
		}
		else if (InNodeName == "arm")
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->Arm;
			CurrentEditPartData->Value = InValue;
		}
		else if (InNodeName == "hand")
		{
			if (CurrentEditPartData)
			{
				Vector2 HandPos;
				if (StrToVec2(InValue, &HandPos))
					CurrentEditPartData->Map.Hand = HandPos;
			}
			else
			{
				CurrentEditPartData = &CurrentEditKeyFrameData->Hand;
				CurrentEditPartData->Value = InValue;
			}
		}
		else if (InNodeName == "armOverHair")
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->ArmOverHair;
			CurrentEditPartData->Value = InValue;
		}
		else if (InNodeName == "lHand")
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->LHand;
			CurrentEditPartData->Value = InValue;
		}
		else if (InNodeName == "rHand")
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->RHand;
			CurrentEditPartData->Value = InValue;
		}
		else if (InNodeName == "neck")
		{
			Vector2 NeckPos;
			if (StrToVec2(InValue, &NeckPos) && CurrentEditPartData)
				CurrentEditPartData->Map.Neck = NeckPos;
			else
				bEmpty = true;
		}
		else if (InNodeName == "navel")
		{
			Vector2 NavelPos;
			if (StrToVec2(InValue, &NavelPos) && CurrentEditPartData)
				CurrentEditPartData->Map.Navel = NavelPos;
			else
				bEmpty = true;
		}
		else if (InNodeName == "origin")
		{
			Vector2 OriginPos;
			if (StrToVec2(InValue, &OriginPos) && CurrentEditPartData)
				CurrentEditPartData->Origin = OriginPos;
			else
				bEmpty = true;
		}
		else if (InNodeName == "z")
		{
			if (CurrentEditPartData)
				CurrentEditPartData->Z = InValue;
			else
				bEmpty = true;
		}
		else if (InNodeName == "group")
		{
			if (CurrentEditPartData)
				CurrentEditPartData->Group = InValue;
			else
				bEmpty = true;
		}
		else if (InNodeName == "_outlink")
		{
			if (CurrentEditPartData)
				CurrentEditPartData->OutLink = InValue;
			else
				bEmpty = true;
		}
		else if (InNodeName == "face")
		{
			int16_t FaceValue = static_cast<int16_t>(std::stoi(InValue));
			CurrentEditKeyFrameData->Face = FaceValue;
		}
		else if (InNodeName == "delay")
		{
			int32_t FaceValue = static_cast<int32_t>(std::stoi(InValue));
			CurrentEditKeyFrameData->Delay = FaceValue;
		}
		else if (InNodeName == "frame")
		{
			int32_t FrameValue = static_cast<int32_t>(std::stoi(InValue));
			CurrentEditKeyFrameData->Frame = FrameValue;
		}
		else if (InNodeName == "move")
		{
			Vector2 Move;
			if (StrToVec2(InValue, &Move))
				CurrentEditKeyFrameData->Move = Move;
		}
		else if (InNodeName == "action")
		{
			CurrentEditKeyFrameData->Action = InValue;
		}
		else
			bEmpty = true;
	}

private:
	std::map<std::string, CCharacterData> CharacterDatas;
	CCharacterData* CurrentEditCharacterData = nullptr;
	CAnimData* CurrentEditAnimData = nullptr;
	TKeyFrameData* CurrentEditKeyFrameData = nullptr;
	TPartData* CurrentEditPartData = nullptr;

	bool bEmpty = false;
};

