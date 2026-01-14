#pragma once
#ifdef GetObject
#undef GetObject
#endif
#include "Utils.h"
#include "02.Contents/Component/Character.h"

#include "01.Base/Actor/Component/Animation/Animator.h"


//struct TMap
//{
//	Vector2 Neck;
//	Vector2 Navel;
//	Vector2 Hand;
//};
//
//struct TWzPartData
//{
//	std::string Value;
//	Vector2 Origin;
//	TMap Map;
//	std::string Z;
//	std::string Group;
//	std::string OutLink;
//};
//
//struct TKeyFrameData
//{
//	TWzPartData Body;
//	TWzPartData Arm;
//	TWzPartData Hand;
//	TWzPartData ArmOverHair;
//	TWzPartData LHand;
//	TWzPartData RHand;
//
//	std::string Action;
//	int16_t Face;
//	int32_t Delay;
//	int32_t Frame;
//	Vector2 Move;
//};
//
//class CAnimData
//{
//	friend class CCharacterLoader;
//public:
//	CAnimData(size_t InNumOfFrame)
//	{
//		KeyFrameDatas.resize(InNumOfFrame);
//	}
//	~CAnimData() = default;
//
//public:
//	TKeyFrameData& GetKeyFrame(size_t InIndex)
//	{
//		if (KeyFrameDatas.size() <= InIndex)
//			assert(0);
//
//		return KeyFrameDatas[InIndex];
//	}
//	void Clear()
//	{
//		KeyFrameDatas.clear();
//	}
//
//private:
//	std::vector<TKeyFrameData> KeyFrameDatas;
//
//};
//
//class CCharacterData
//{
//	friend class CCharacterLoader;
//public:
//	CCharacterData() = default;
//	~CCharacterData() = default;
//
//public:
//	CAnimData& GetAnimDataRef(const std::string& InAnimName)
//	{
//		auto Iter = AnimDatas.find(InAnimName);
//		if (Iter == AnimDatas.end())
//			assert(0);
//		return Iter->second;
//	}
//	void AddAnimData(const std::string& InAnimName, size_t InNumOfFrame)
//	{
//		if (AnimDatas.contains(InAnimName))
//			return;
//		AnimDatas.emplace(InAnimName, InNumOfFrame);
//	}
//	void RemoveAnimData(const std::string& InAnimName)
//	{
//		AnimDatas.erase(InAnimName);
//	}
//	size_t GetAnimDataSize() const { return AnimDatas.size(); }
//
//private:
//	std::unordered_map<std::string, CAnimData> AnimDatas;
//
//
//};

class CCharacterLoader
{
//	SINGLE(CCharacterLoader)
//public:
//	~CCharacterLoader() = default;
//
//public:
//	bool OpenCharacterData(const std::string& InCharacterWzJsonPath)
//	{
//		FILE* File = nullptr;
//		fopen_s(&File, InCharacterWzJsonPath.c_str(), "rb"); // 바이너리 읽기 권장
//		if (!File)
//			return false;
//
//		char Buffer[65536]; // 데이터가 크므로 버퍼를 조금 더 늘렸습니다.
//		rapidjson::FileReadStream JsonReader(File, Buffer, sizeof(Buffer));
//
//		Document.ParseStream(JsonReader);
//		fclose(File);
//
//		if (Document.HasParseError()) {
//			std::cout << "JSON 파싱 에러!" << std::endl;
//			return false;
//		}
//		return true;
//	}
//	void CloseCharacterData(const std::string& InCharacterName)
//	{
//		Document = rapidjson::Document{};
//	}
//	void LoadCharacterAnimation(const std::string& InCharacterName, const std::string& InAnimName, CCharacter* OutCharacter = nullptr)
//	{
//		if (CharacterDatas.contains(InCharacterName) == false)
//			CharacterDatas.emplace(InCharacterName, CCharacterData{});
//
//		auto Iter = CharacterDatas.find(InCharacterName);
//		CurrentEditCharacterData = &Iter->second;
//		
//		auto AnimBundle = Document["dir"]["dir"].GetArray();
//		// AnimBundle에서 InAnimName과 일치하는 애니메이션만 로드
//
//		for (auto& Anim : AnimBundle)
//		{
//			// 1. 현재 노드의 이름을 확인해봅니다.
//			std::string dirName = Anim["@name"].GetString();
//
//			// 만약 "info"라면 애니메이션 데이터가 아니니 건너뜁니다.
//			if (dirName == "info") 
//				continue;
//			if (dirName == InAnimName)
//			{
//				std::cout << "--- 애니메이션 발견: " << dirName << " ---" << std::endl;
//
//				// 2. 애니메이션 노드(walk1 등) 안에는 "dir"이라는 배열이 프레임들을 들고 있습니다.
//				if (Anim.HasMember("dir") && Anim["dir"].IsArray())
//				{
//					auto Frames = Anim["dir"].GetArray();
//					CurrentEditCharacterData->AddAnimData(dirName, Frames.Size());
//					CurrentEditAnimData = &CurrentEditCharacterData->GetAnimDataRef(dirName);
//					for (size_t i = 0; i < Frames.Size(); ++i)
//					{
//						auto& Frame = Frames[i];
//						CurrentEditKeyFrameData = &CurrentEditAnimData->GetKeyFrame(i);
//						ParseJsonValue(Frame);
//						if (bEmpty)
//						{
//							CurrentEditCharacterData->RemoveAnimData(dirName);
//							break;
//						}
//					}
//					CurrentEditAnimData = nullptr;
//					CurrentEditPartData = nullptr;
//					CurrentEditKeyFrameData = nullptr;
//					bEmpty = false;
//					break;
//				}
//			}
//		}
//		CurrentEditCharacterData = nullptr;
//
//		if(OutCharacter)
//			CoverToCharacter(InCharacterName, InAnimName, *OutCharacter);
//	}
//	void InitalizeCharacter(CCharacter& OutCharacter);
//	void CoverToCharacter(const std::string& InCharacterName, const std::string& InAnimName, CCharacter& OutCharacter);
//
//private:
//	void ParseObject(const rapidjson::GenericObject<true, rapidjson::Value>& InObject)
//	{
//		if (InObject.HasMember("@name") && InObject.HasMember("@value"))
//		{
//			const std::string& NodeName = InObject["@name"].GetString();
//			const std::string& NodeValue = InObject["@value"].GetString();
//			ParseNode(NodeName, NodeValue);
//		}
//
//		for (auto Iter = InObject.MemberBegin(); Iter != InObject.MemberEnd(); ++Iter)
//		{
//			auto& Name = Iter->name;
//			auto& Value = Iter->value;
//			if (Value.IsObject() || Value.IsArray())
//				ParseJsonValue(Value);
//		}
//	}
//	void ParseArray(const rapidjson::GenericArray<true, rapidjson::Value>& InArray)
//	{
//		for (auto& Element : InArray)
//		{
//			if (Element.IsObject())
//			{
//				auto Object = Element.GetObject();
//				ParseObject(Object);
//			}
//		}
//	}
//	void ParseJsonValue(const rapidjson::Value& InValue)
//	{
//		if (InValue.IsObject())
//			ParseObject(InValue.GetObject());
//		else if (InValue.IsArray())
//			ParseArray(InValue.GetArray());
//	}
//
//	void ParseNode(const std::string& InNodeName, const std::string& InValue);
//
//private:
//	void CompositeCharacterFrame(const std::string& InAnimName, const TKeyFrameData& InKeyFrameData, size_t InFrame, CCharacter& OutCharacter);
//	void AddFrameToPart(const std::string& InAnimName, const TKeyFrameData& InKeyFrameData, const TWzPartData& InPartData, const std::string& InPartName
//		, size_t InFrame, CCharacter& OutCharacter);
//	// AnimName, FrameNumber, OutCharacter, KeyFrameData, PartData, PartName
//	// 각자의 Offset계산하기 함수 만들어줘
//	Vector3 CalculatePartOffset(const std::string& InAnimName, const std::string& InPartName, size_t InFrameNumber, const CCharacter& InCharacter
//		, const TKeyFrameData& InKeyFrameData, const TWzPartData& InPartData);
//	bool IsValidPartData(const TWzPartData& InPartData)
//	{
//		return InPartData.Value.empty() == false;
//	}
//
//	std::string DetachRelativePath(const std::string& InFullPath)
//	{
//		// Ex: ../../walk1/0/body
//		size_t Pos = InFullPath.find("../", 0);
//		if (Pos == std::string::npos)
//			return InFullPath;
//		return DetachRelativePath(InFullPath.substr(Pos + 3));
//	}
//	std::string GetAnimationNameInRefPath(const std::string& InRefPath, std::string* OutFramePath)
//	{
//		// Ex: walk1/0/body
//		size_t SlashPos = InRefPath.find('/');
//		*OutFramePath = InRefPath.substr(SlashPos + 1);
//		return InRefPath.substr(0, SlashPos);
//	}
//	uint32_t GetFrameNumberInRefPath(const std::string& InFramePath)
//	{
//		// Ex: 0/body
//		size_t SlashPos = InFramePath.find('/');
//		std::string FrameNumberStr = InFramePath.substr(0, SlashPos);
//		return static_cast<uint32_t>(std::stoul(FrameNumberStr));
//	}
//	bool IsFrameRefPath(const std::string& InFramePath)
//	{
//		// /의 개수가 1개인지 확인
//		return std::count(InFramePath.begin(), InFramePath.end(), '/') == 1;
//	}
//	bool IsRefValue(const std::string& InValue)
//	{
//		return InValue.find("../") != std::string::npos;
//	}
//	float GetDurationFromDelay(int32_t InDelay)
//	{
//		// 1000ms -> 1s
//		return static_cast<float>(InDelay) / 1000.0f;
//	}
//
//private:
//	rapidjson::Document Document;
//
//	std::map<std::string, CCharacterData> CharacterDatas;
//	CCharacterData* CurrentEditCharacterData = nullptr;
//	CAnimData* CurrentEditAnimData = nullptr;
//	TKeyFrameData* CurrentEditKeyFrameData = nullptr;
//	TWzPartData* CurrentEditPartData = nullptr;
//
//	bool bEmpty = false;
};

