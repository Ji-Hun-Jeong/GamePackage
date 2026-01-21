#pragma once
#include "Common/Json.h"

class CWzNode
{
public:
	CWzNode(const std::string& InName) : Name(InName) {}
	CWzNode() = default;

	void AddChildNode(const std::string_view InNodeName, std::unique_ptr<CWzNode> InChildNode) {
		ChildNodes.emplace(InNodeName, std::move(InChildNode));
	}
	CWzNode* GetChildNode(const std::string_view InNodeName) const {
		auto Iter = ChildNodes.find(InNodeName);
		return (Iter != ChildNodes.end()) ? Iter->second.get() : nullptr;
	}
	const CWzNode& operator[](const std::string_view InNodeName) const
	{
		return *GetChildNode(InNodeName);
	}
	// CWzNode.cpp 또는 헤더 내부
	void SetName(const std::string_view InName) { Name = InName; }
	void SetValue(const std::string& InValue) { Value = InValue; }
	const std::string& GetValue() const { return Value; }
	const std::string& GetName() const { return Name; }
	bool HasMember(const std::string_view InMemberName) const
	{
		return ChildNodes.contains(InMemberName);
	}

	const std::map<std::string, std::unique_ptr<CWzNode>, std::less<>>& GetMembers() const { return ChildNodes; }

private:
	std::string Name;
	std::string Value;
	std::map<std::string, std::unique_ptr<CWzNode>, std::less<>> ChildNodes;
};

class CWzLoader
{
public:
	CWzLoader() = default;
	~CWzLoader() = default;

public:
	bool OpenWzData(const std::string_view InWzJsonPath)
	{
		WzJsonPath = InWzJsonPath;
		FILE* File = nullptr;
		fopen_s(&File, WzJsonPath.c_str(), "rb"); // 바이너리 읽기 권장
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
		Document = rapidjson::Document();
		WzJsonPath = "";
	}

	std::unique_ptr<CWzNode> LoadWzData(const std::string_view InWzJsonPath)
	{
		if (IsOpen() == false)
			OpenWzData(InWzJsonPath);

		CWzNode* RootNode = new CWzNode;
		ParseJsonValue(Document["dir"], RootNode);

		CloseWzData();

		return std::unique_ptr<CWzNode>(RootNode);
	}

	bool IsOpen() const
	{
		return WzJsonPath.empty() == false;
	}

	const rapidjson::Document& GetLoadData() const { return Document; }

private:
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
	std::string WzJsonPath;
	rapidjson::Document Document;

};

class CWzUtils
{
public:
	static Vector3 GetWorldPositionFromOrigin(const class CSpriteRenderComponent& InSpriteRenderComponent, const Vector2& InWzOrigin);
	static bool SetWorldPositionFromOrigin(class CActor& InActor, const Vector2& InWzOrigin);
};