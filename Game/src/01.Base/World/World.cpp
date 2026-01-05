#include "pch.h"
#include "World.h"

#include <fstream>
#include "02.Contents/Scene/LobbyScene.h"
#include "02.Contents/Scene/EditUIScene.h"
#include "02.Contents/Scene/TestScene.h"
#include "02.Contents/Scene/LoginScene.h"
#include "02.Contents/Scene/Editor/MapEditorScene.h"

CWorld* g_World = nullptr;

CWorld::CWorld()
{
	g_World = this;
}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	LoadScene<CTestScene>();
}


void CWorld::Serialize(const CActor& InSerializeActor, const std::string& InSavePath)
{
	std::ofstream OutputFileStream(InSavePath);
	if (!OutputFileStream.is_open())
	{
		std::cerr << "Error: Could not open file " << InSavePath << std::endl;
		return;
	}
	CSerializer ActorData;
	InSerializeActor.Serialize(ActorData);
	OutputFileStream << ActorData.dump(4);
}

CObject* CWorld::Deserialize(const std::string& InReadPath, CActor* InOwnerActor)
{
	// 1. 파일 열기
	std::ifstream InputFileStream(InReadPath);
	if (!InputFileStream.is_open())
	{
		std::cerr << "Error: Could not open file " << InReadPath << std::endl;
		return nullptr;
	}

	// 2. JSON 파싱
	CSerializer ActorJson;
	try 
	{
		// 파일 스트림에서 직접 json 객체로 데이터를 읽어옵니다.
		InputFileStream >> ActorJson;
		std::cout << ActorJson;
	}
	catch (CSerializer::parse_error& e) 
	{
		std::cerr << "Error: Failed to parse JSON file " << InReadPath << "\n"
			<< e.what() << std::endl;
		return nullptr;
	}
	const std::string& Name = ActorJson["Name"];
	CClass* Class = CClassManager::GetInst().GetClassByName(Name);
	/*CObject* Object = Class->CreateObject<CObject>(InOwnerActor);
	Object->Deserialize(ActorJson);

	return Object;*/
	return nullptr;
}