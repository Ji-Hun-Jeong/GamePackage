#pragma once
#include "WzBase.h"

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

	bool IsOpen() const
	{
		return WzJsonPath.empty() == false;
	}

	bool GetLoadData(const std::string_view InDataName, JValue** OutValue)
	{
		if (Document.HasMember(InDataName.data()) == false)
			return false;
		*OutValue = &Document[InDataName.data()];
		return true;
	}

	const rapidjson::Document& GetDocument() const { return Document; }

private:
	std::string WzJsonPath;
	rapidjson::Document Document;

};

