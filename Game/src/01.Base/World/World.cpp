#include "pch.h"
#include "World.h"

#include <fstream>
#include "02.Contents/Scene/LobbyScene.h"
#include "02.Contents/Scene/EditUIScene.h"

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
	LoadScene<CLobbyScene>();
}

void CWorld::Arrange()
{
	while (NextDeletedObjects.empty() == false)
	{
		CObject* Object = NextDeletedObjects.front();
		NextDeletedObjects.pop();

		Object->EndPlay();
		Object->ResetOwner(Object->GetOwner());
	}

	// 3. 한 번에 제거 (O(n))
	auto NewEnd = std::remove_if(WorldActors.begin(), WorldActors.end(),
		[](const auto& Actor) { return Actor->bDestroy; });
	WorldActors.erase(NewEnd, WorldActors.end());

}

void CWorld::Ready()
{
	while (NextAddedWorldActors.empty() == false)
	{
		std::unique_ptr<CActor> Actor = std::move(NextAddedWorldActors.front());
		NextAddedWorldActors.pop();

		Actor->BeginPlay();

		WorldActors.push_back(std::move(Actor));
	}

	while (WorldSynchronizeEvents.empty() == false)
	{
		WorldSynchronizeEvents.front()();
		WorldSynchronizeEvents.pop();
	}
}

void CWorld::Update()
{
	for (auto& WorldActor : WorldActors)
		WorldActor->Update(1.0f / 60.0f);

	for (auto& WorldActor : WorldActors)
		WorldActor->FinalUpdate();
}

void CWorld::CaptureSnapShot()
{
	for (auto& WorldActor : WorldActors)
		WorldActor->CaptureSnapShot();
}



void CWorld::InitalizeObject(CObject* InObject, CActor* InOwnerActor)
{
	InObject->InstanceId = NumberGenerator.GenerateNumber();
	InObject->World = this;

	if (InOwnerActor)
		InObject->SetOwner(InOwnerActor);

	InObject->Initalize();

	auto Iter = NewObjectTypeEvents.find(InObject->GetType());
	if (Iter != NewObjectTypeEvents.end())
	{
		for (auto& NewObjectTypeEvent : Iter->second)
			NewObjectTypeEvent->CreatedInWorld(*this, *InObject);
	}
	for (auto& NewObjectEvent : NewObjectEvents)
		NewObjectEvent->CreatedInWorld(*this, *InObject);
}

void CWorld::AppearActor(CActor* InActor)
{
	NextAddedWorldActors.emplace(InActor);
}

void CWorld::ClearWorld()
{
	while (NextAddedWorldActors.empty() == false)
		NextAddedWorldActors.pop();

	for (auto& WorldActor : WorldActors)
		WorldActor->Destroy();
}

void CWorld::DestroyObject(CObject* InObject)
{
	PushWorldSynchronizeEvent([this, InObject]()->void
		{
			if (InObject->bDestroy)
				return;
			InObject->bDestroy = true;
			NextDeletedObjects.push(InObject);
		});
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

void CWorld::Deserialize(CActor& InSerializeActor, const std::string& InReadPath)
{
	// 1. 파일 열기
	std::ifstream InputFileStream(InReadPath);
	if (!InputFileStream.is_open())
	{
		std::cerr << "Error: Could not open file " << InReadPath << std::endl;
		return;
	}

	// 2. JSON 파싱
	CSerializer ActorJson;
	try {
		// 파일 스트림에서 직접 json 객체로 데이터를 읽어옵니다.
		InputFileStream >> ActorJson;
		std::cout << ActorJson;
	}
	catch (CSerializer::parse_error& e) {
		std::cerr << "Error: Failed to parse JSON file " << InReadPath << "\n"
			<< e.what() << std::endl;
		return;
	}

	InSerializeActor.Deserialize(ActorJson);
}