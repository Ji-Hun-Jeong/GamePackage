#pragma once
#include "IEditState.h"

class CObjectManager
{
public:
	CObjectManager() = default;
	~CObjectManager() = default;

public:
	void AddObject(CStaticActor& InObject)
	{
		for (auto Object : Objects)
		{
			if (Object == &InObject)
				return;
		}
		Objects.push_back(&InObject);
	}
	void RemoveObject(CStaticActor& InObject)
	{
		for (auto Iter = Objects.begin(); Iter != Objects.end(); ++Iter)
		{
			if (*Iter == &InObject)
			{
				Objects.erase(Iter);
				break;
			}
		}
	}
	CStaticActor* GetObjectByPosition(const Vector2& InPosition)
	{
		for (auto Object : Objects)
		{
			if (CTransformUtils::IsPositionInsideStaticActor(InPosition, *Object))
				return Object;
		}
		return nullptr;
	}

private:
	std::vector<CStaticActor*> Objects;

};

class CFreeEditState : public CEditState
{
public:
	CFreeEditState() = default;
	~CFreeEditState() = default;

public:
	void EnterEditState() override
	{
		GetTileMapEditContext().MainPanel->SetMouseFocusEvent([this]()->void {bOnMainPanel = true; });
		GetTileMapEditContext().TileMap->RenderTiles(false);
	}
	void OnEditState() override;
	void ExitEditState() override
	{
		GetTileMapEditContext().MainPanel->SetMouseFocusEvent(nullptr);
		GetTileMapEditContext().TileMap->RenderTiles(true);
	}
	void ToImGUI() override
	{
		if (ImGui::Button("OpenWindowDialog"))
			ImageImporter.AddImagePathByWindowManager(CWindowManager::GetInst());

		ImGui::BeginChild("ImageList", ImVec2(0, 0), true);

		const std::wstring& GeneratedActorImagePath = ImageImporter.GetCurrentImagePath();
		for (auto& Pair : ImageImporter.GetLoadedImagePaths())
		{
			const std::string& ImageName = Pair.first;
			const std::wstring& ImagePath = Pair.second;

			bool bSelected = (GeneratedActorImagePath == ImagePath);

			if (ImGui::Selectable(ImageName.c_str(), bSelected))
			{
				ImageImporter.SetCurrentImagePath(ImageName);
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndChild();

		if (ImGui::Button("Create"))
			bCreateImage = true;

		if (CurrentFocusObject == nullptr)
			return;

		ImGui::Separator(); // 구분선
		ImGui::Text("Object Properties");

		// [방법 1] DragFloat3: 마우스 드래그로 수치 조절 (에디터에서 가장 많이 씀)
		// 속도(v_speed)를 1.0f 정도로 주면 적당합니다.
		if (ImGui::InputFloat3("Position", PosArray))
		{
		
		}

		// 만약 Scale이나 Rotation도 필요하시다면 아래처럼 추가 가능합니다.
		/*
		Vector3 Scale = CurrentFocusObject->GetTransform()->GetLocalScale();
		float ScaleArray[3] = { Scale.x, Scale.y, Scale.z };
		if (ImGui::DragFloat3("Scale", ScaleArray, 0.1f, 0.1f, 1000.0f, "%.2f"))
		{
			CurrentFocusObject->GetTransform()->SetScale(Vector3(ScaleArray[0], ScaleArray[1], ScaleArray[2]));
		}
		*/
	}
private:
	bool bOnMainPanel = false;

	CImageImporter ImageImporter;
	CObjectManager ObjectManager;

	bool bCreateImage = false;

	CStaticActor* CurrentFocusObject = nullptr;

	float PosArray[3] = { 0 };
};