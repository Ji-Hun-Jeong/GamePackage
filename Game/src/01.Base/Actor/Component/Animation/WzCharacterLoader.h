#pragma once

#include "Utils.h"
#include "Common/Json.h"
#include "WzCharacterAnimator.h"
#include "01.Base/Manager/WzLoader.h"

class CWzCharacterLoader
{
	SINGLE(CWzCharacterLoader)
public:
	~CWzCharacterLoader() = default;

public:
	bool LoadWzCharacterAnimation(CWzNode& InWzNode, const std::string_view InCharacterImgName
		, const std::string_view InAnimName, CWzCharacterAnimator* OutWzCharacterAnimator)
	{
		if (InWzNode.HasMember(InCharacterImgName) == false)
			return;
		const auto& CharacterData = InWzNode[InCharacterImgName];
		if (CharacterData.HasMember(InAnimName) == false)
			return;
		const auto& AnimData = CharacterData[InAnimName];

		ParseWzAnimNode(AnimData, *OutWzCharacterAnimator);

		return true;
		/*auto AnimArray = InLoadData["dir"]["dir"].GetArray();
		for (auto& Anim : AnimArray)
		{
			std::string dirName = Anim["@name"].GetString();
			if (dirName == "info")
				continue;
			if (dirName != InAnimName)
				continue;

			std::cout << "--- 애니메이션 발견: " << dirName << " ---" << std::endl;

			auto RootNode = std::make_unique<CWzNode>("root");
			ParseJsonValue(Anim, RootNode.get());

			CWzNode* RawNode = RootNode->GetChildNode(dirName);

			if (OutWzCharacterAnimator)
				ParseWzAnimNode(*RawNode, *OutWzCharacterAnimator);

			return RawNode;
		}
		return nullptr;*/
	}

private:
	void ParseWzAnimNode(const CWzNode& InAnimNode, CWzCharacterAnimator& OutWzCharacterAnimator)
	{
		const std::string& AnimName = InAnimNode.GetName();
		CWzCharacterAnimation& WzCharacterAnimation = OutWzCharacterAnimator.GetAnimationRef(AnimName);

		const auto& Members = InAnimNode.GetMembers();
		for (auto Iter = Members.begin(); Iter != Members.end(); ++Iter)
		{
			const std::string& FrameNumber = Iter->first;

		}
		/*for (auto& AnimChildIter : InAnimNode.ChildNodes)
		{
			const std::string& Frame = AnimChildIter.first;
			CWzNode* FrameNode = AnimChildIter.second.get();

			CWzFrameData FrameData;
			for (auto& FrameChildIter : FrameNode->ChildNodes)
			{
				const std::string& FrameInfo = FrameChildIter.first;
				EWzPartType PartType = CWzPart::GetPartTypeByName(FrameInfo);
				if (PartType == EWzPartType::End)
					InterpretFrameData(*FrameNode, FrameData);
				else
				{
					CWzNode* PartNode = FrameNode->ChildNodes[FrameInfo].get();
					TWzPartData& PartData = FrameData.GetPartDataRef(PartType);
					InterpretPartData(*PartNode, PartData);
				}
			}
			WzCharacterAnimation.AddFrameData(FrameData);
		}*/
	}
	void InterpretFrameData(const CWzNode& InEtcNode, CWzFrameData& OutFrameData)
	{
		/*const auto& EtcNodes = InEtcNode.ChildNodes;

		if (EtcNodes.contains("action"))
			OutFrameData.SetAction(EtcNodes.at("action")->GetValue());
		if (EtcNodes.contains("face"))
			OutFrameData.SetFace(std::stoi(EtcNodes.at("face")->GetValue()));
		if (EtcNodes.contains("delay"))
			OutFrameData.SetDelay(std::stoi(EtcNodes.at("delay")->GetValue()));
		if (EtcNodes.contains("frame"))
			OutFrameData.SetFrame(std::stoi(EtcNodes.at("frame")->GetValue()));
		if (EtcNodes.contains("move"))
		{
			Vector2 Move;
			if (StrToVec2(EtcNodes.at("move")->GetValue(), &Move))
				OutFrameData.SetMove(Move);
		}*/
	}
	void InterpretPartData(const CWzNode& InPartNode, TWzPartData& OutPartData)
	{
		/*const auto& PartNodes = InPartNode.ChildNodes;
		if (PartNodes.contains("value"))
			OutPartData.Value = PartNodes.at("value")->GetValue();
		if (PartNodes.contains("origin"))
		{
			Vector2 Origin;
			if (StrToVec2(PartNodes.at("origin")->GetValue(), &Origin))
				OutPartData.Origin = Origin;
		}
		if (PartNodes.contains("map"))
		{
			const auto& MapNodes = PartNodes.at("map")->ChildNodes;
			if (MapNodes.contains("neck"))
			{
				Vector2 Neck;
				if (StrToVec2(MapNodes.at("neck")->GetValue(), &Neck))
					OutPartData.Map.Neck = Neck;
			}
			if (MapNodes.contains("navel"))
			{
				Vector2 Navel;
				if (StrToVec2(MapNodes.at("navel")->GetValue(), &Navel))
					OutPartData.Map.Navel = Navel;
			}
			if (MapNodes.contains("hand"))
			{
				Vector2 Hand;
				if (StrToVec2(MapNodes.at("hand")->GetValue(), &Hand))
					OutPartData.Map.Hand = Hand;
			}
			if (MapNodes.contains("handMove"))
			{
				Vector2 HandMove;
				if (StrToVec2(MapNodes.at("handMove")->GetValue(), &HandMove))
					OutPartData.Map.HandMove = HandMove;
			}
		}
		if (PartNodes.contains("z"))
			OutPartData.Z = PartNodes.at("z")->GetValue();
		if (PartNodes.contains("group"))
			OutPartData.Group = PartNodes.at("group")->GetValue();
		if (PartNodes.contains("_outlink"))
			OutPartData.OutLink = "resources/image/" + PartNodes.at("_outlink")->GetValue() + ".png";*/
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
	using CCharacter = std::map<std::string, std::unique_ptr<CWzNode>>;
	std::map<std::string, CCharacter> WzCharacters;

};

