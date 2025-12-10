#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "03.Utils/CTransformUtils.h"

class CLadder : public CActor
{
	GENERATE_OBJECT(CLadder)
public:
	CLadder() {}
	~CLadder() = default;

public:
	void InitalizeLadder(const std::wstring& InHeadImagePath, const std::vector<std::wstring>& InBodyImagePath, const std::wstring& InFootImagePath);
	void StretchToUp();
	void StretchToDown();

private:
	Vector3 ReCalculatePosition()
	{
		Vector3 NewPosition;
		NewPosition += Head->GetTransform()->GetPosition();
		NewPosition += Foot->GetTransform()->GetPosition();
		for (auto Body : Bodys)
			NewPosition += Body->GetTransform()->GetPosition();
		NewPosition += Transform->GetPosition() * (Bodys.size() + 2);
		NewPosition /= (Bodys.size() + 2);

		return NewPosition;
	}
	void ReBuild(CStaticActor& InNewBody)
	{
		const Vector3& OriginPosition = Transform->GetFinalPosition();
		const Vector3& OriginScale = Transform->GetScale();
		Vector2 NewBodyScale = InNewBody.GetSpriteRenderComponent()->GetImageScale();

		Vector3 NewPosition = ReCalculatePosition();
		Transform->SetPosition(NewPosition);
		Vector3 NewScale = Vector3(OriginScale.x, OriginScale.y + NewBodyScale.y, 1.0f);
		Transform->SetScale(NewScale);

		Vector3 OriginNewDist = NewPosition - OriginPosition;
		Head->GetTransform()->SetPosition(Head->GetTransform()->GetPosition() - OriginNewDist);
		for(auto Body : Bodys)
			Body->GetTransform()->SetPosition(Body->GetTransform()->GetPosition() - OriginNewDist);
		Foot->GetTransform()->SetPosition(Foot->GetTransform()->GetPosition() - OriginNewDist);
	}
	

private:
	std::wstring HeadImagePath;
	std::vector<std::wstring> BodyImagePaths;
	std::wstring FootImagePath;

	CStaticActor* Head = nullptr;
	std::list<CStaticActor*> Bodys;
	CStaticActor* Foot = nullptr;

};

class CLadderEditor
{
public:
	CLadderEditor() = default;
	~CLadderEditor() = default;

public:
	void SetHeadImagePath(const std::wstring& InHeadImagePath)
	{
		HeadImagePath = InHeadImagePath;
	}
	void AddBodyImagePath(const std::wstring& InBodyImagePath)
	{
		BodyImagePaths.push_back(InBodyImagePath);
	}
	void SetFootImagePath(const std::wstring& InFootImagePath)
	{
		FootImagePath = InFootImagePath;
	}
	bool IsEditReady()
	{
		if (HeadImagePath.empty())
			return false;
		if (BodyImagePaths.empty())
			return false;
		if (FootImagePath.empty())
			return false;
		return true;
	}
	void SetLadder(CLadder& InLadder, const Vector3& InPosition)
	{
		InLadder.InitalizeLadder(HeadImagePath, BodyImagePaths, FootImagePath);
		InLadder.GetTransform()->SetPosition(InPosition);
		ManagingLadders.push_back(&InLadder);
	}
	CLadder* GetLadder(const Vector2& InPosition)
	{
		for (auto Ladder : ManagingLadders)
			if (CTransformUtils::IsPositionInsideActor(InPosition, *Ladder))
				return Ladder;
		return nullptr;
	}
	void SetFocusLadder(CLadder* InLadder)
	{
		for (auto Ladder : ManagingLadders)
		{
			if (Ladder == InLadder)
			{
				CurrentFocusLadder = Ladder;
				break;
			}
		}
	}
	CLadder* GetFocusLadder() const { return CurrentFocusLadder; }

private:
	std::wstring HeadImagePath;
	std::vector<std::wstring> BodyImagePaths;
	std::wstring FootImagePath;

	CLadder* CurrentFocusLadder = nullptr;
	std::vector<CLadder*> ManagingLadders;
};