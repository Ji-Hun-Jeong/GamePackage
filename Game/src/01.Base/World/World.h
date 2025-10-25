#pragma once
#include <fstream>
#include "WorldEvent.h"
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Scene.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

public:
	void Start();

	void Arrange()
	{
		for (auto& WorldActor : WorldActors)
		{
			if (WorldActor->bDestroy)
				WorldActor->EndPlay();
		}
		for (auto& WorldActor : WorldActors)
			WorldActor->ComponentArrange();

		if (bFlagDestroyedWorldObject == false)
			return;

		for (auto& WorldActor : WorldActors)
		{
			if (WorldActor->bDestroy && WorldActor->Owner)
				WorldActor->Owner->Detach(WorldActor.get());
		}

		// 3. 한 번에 제거 (O(n))
		auto NewEnd = std::remove_if(WorldActors.begin(), WorldActors.end(),
			[](const auto& Actor) { return Actor->bDestroy; });
		WorldActors.erase(NewEnd, WorldActors.end());

		bFlagDestroyedWorldObject = false;
	}

	void Ready()
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

	void Update()
	{
		for (auto& WorldActor : WorldActors)
			WorldActor->Update(1.0f / 60.0f);

		for (auto& WorldActor : WorldActors)
			WorldActor->FinalUpdate();
	}

	void CaptureSnapShot()
	{
		for (auto& WorldActor : WorldActors)
			WorldActor->CaptureSnapShot();
		int a = 1;
	}

	void Serialize(const CActor& InSerializeActor, const std::string& InSavePath)
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

	void Deserialize(CActor& InSerializeActor, const std::string& InReadPath)
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

	void PushWorldSynchronizeEvent(std::function<void()> InWorldSynchronizeEvent) { WorldSynchronizeEvents.push(InWorldSynchronizeEvent); }

	void InitalizeObject(CObject* InObject, CActor* InOwnerActor)
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
	void AddActor(CActor* InActor)
	{
		NextAddedWorldActors.emplace(InActor);
	}
	void ClearWorld()
	{
		while (NextAddedWorldActors.empty() == false)
			NextAddedWorldActors.pop();

		for (auto& WorldActor : WorldActors)
			WorldActor->Destroy();
	}
//	template <typename T>
//	T* NewObject(CActor* InOwner)
//	{
//		T* Object = new T;
//		Object->InstanceId = NumberGenerator.GenerateNumber();
//		Object->World = this;
//
//		if (InOwner)
//			InOwner->Attach(Object);
//
//		Object->Initalize();
//
//		auto Iter = NewObjectTypeEvents.find(T::GetStaticType());
//		if (Iter != NewObjectTypeEvents.end())
//		{
//			for (auto& NewObjectTypeEvent : Iter->second)
//				NewObjectTypeEvent->CreatedInWorld(*this, *Object);
//		}
//		for (auto& NewObjectEvent : NewObjectEvents)
//			NewObjectEvent->CreatedInWorld(*this, *Object);
//
//		return Object;
//	}
//
//	template <typename T>
//	T* SpawnActor(CActor* InOwner = nullptr)
//	{
//		T* Actor = NewObject<T>(InOwner);
//
//		NextAddedWorldActors.emplace(Actor);
//
//		return Actor;
//	}
//
//	template <typename T_SCENE>
//	void LoadScene()
//	{
//		WorldSynchronizeEvents.push([this]()->void
//			{
//				LoadSceneImmediate<T_SCENE>();
//			});
//	}
//
//private:
//	template <typename T_SCENE>
//	void LoadSceneImmediate()
//	{
//		while (NextAddedWorldActors.empty() == false)
//			NextAddedWorldActors.pop();
//
//		for (auto& WorldActor : WorldActors)
//			WorldActor->Destroy();
//		SpawnActor<T_SCENE>();
//	}

public:
	const std::vector<std::unique_ptr<CActor>>& GetWorldActors() const { return WorldActors; }
	void MarkDestroyed() { bFlagDestroyedWorldObject = true; }
	void AddNewObjectTypeEvent(ObjectType InObjectType, std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		auto Iter = NewObjectTypeEvents.find(InObjectType);
		if (Iter == NewObjectTypeEvents.end())
		{
			std::vector<std::unique_ptr<INewObjectEvent>> Vec;
			Vec.push_back(std::move(InNewObjectEvent));
			NewObjectTypeEvents.insert({ InObjectType, std::move(Vec) });
		}
		else
			Iter->second.push_back(std::move(InNewObjectEvent));
	}
	void AddNewObjectEvent(std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		NewObjectEvents.push_back(std::move(InNewObjectEvent));
	}

private:
	std::vector<std::unique_ptr<CActor>> WorldActors;
	std::queue<std::unique_ptr<CActor>> NextAddedWorldActors;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;
	
	std::map<ObjectType, std::vector<std::unique_ptr<INewObjectEvent>>> NewObjectTypeEvents;
	std::vector<std::unique_ptr<INewObjectEvent>> NewObjectEvents;

	std::queue<std::function<void()>> WorldSynchronizeEvents;

};

extern CWorld* g_World;

template <typename T>
T* NewObject(CActor* InOwnerActor)
{
	T* Object = new T;
	g_World->InitalizeObject(Object, InOwnerActor);
	return Object;
}

template <typename T>
T* SpawnActor(CActor* InOwnerActor = nullptr)
{
	T* Actor = NewObject<T>(InOwnerActor);
	g_World->AddActor(Actor);
	return Actor;
}

template <typename T_SCENE>
void LoadScene()
{
	g_World->PushWorldSynchronizeEvent([]()->void
		{
			g_World->ClearWorld();
			SpawnActor<T_SCENE>();
		});
}