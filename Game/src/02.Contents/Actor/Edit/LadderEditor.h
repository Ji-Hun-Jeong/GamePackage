#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "03.Utils/CTransformUtils.h"
#include "02.Contents/Actor/UI/UI.h"

class CLadderForm : public CStaticActor
{
	friend class CLadderEditor;
	GENERATE_OBJECT(CLadderForm)
public:
	CLadderForm()
	{
		DetachComponent(GetSpriteRenderComponent());
	}
	~CLadderForm() = default;

public:
	void SetHead(CStaticActor& InHead) { Head = &InHead; }
	void AddBody(CStaticActor& InBody) { Bodys.push_back(&InBody); }
	void SetFoot(CStaticActor& InFoot) { Foot = &InFoot; }

private:
	CStaticActor* Head = nullptr;
	std::list<CStaticActor*> Bodys;
	CStaticActor* Foot = nullptr;

};

class CLadderEditor : public CActor
{
	GENERATE_OBJECT(CLadderEditor)
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
	void StretchToUp(CLadderForm& InLadder);
	void StretchToDown(CLadderForm& InLadder);
	CLadderForm* CreateLadder(const Vector3& InPosition);
	CLadderForm* GetProximateLadder(const Vector2& InPosition)
	{
		for (auto Ladder : Ladders)
		{
			if (CTransformUtils::IsPositionInsideActor(InPosition, *Ladder))
				return Ladder;
		}
		return nullptr;
	}
	void DestroyLadder(CLadderForm& InLadder)
	{
		for (auto Iter = Ladders.begin(); Iter != Ladders.end(); ++Iter)
		{
			if (*Iter == &InLadder)
			{
				(*Iter)->Destroy();
				Ladders.erase(Iter);
				break;
			}
		}
	}

private:
	Vector3 ReCalculatePosition(CLadderForm& InLadder)
	{
		Vector3 NewPosition;
		NewPosition += InLadder.Head->GetTransform()->GetPosition();
		NewPosition += InLadder.Foot->GetTransform()->GetPosition();
		for (auto Body : InLadder.Bodys)
			NewPosition += Body->GetTransform()->GetPosition();
		NewPosition += InLadder.GetTransform()->GetPosition() * static_cast<float>((InLadder.Bodys.size() + 2));
		NewPosition /= static_cast<float>((InLadder.Bodys.size() + 2));

		return NewPosition;
	}
	void ReBuild(CLadderForm& InLadder, CStaticActor& InNewBody)
	{
		const Vector3& OriginPosition = InLadder.GetTransform()->GetPosition();
		const Vector3& OriginScale = InLadder.GetTransform()->GetScale();
		Vector2 NewBodyScale = InNewBody.GetSpriteRenderComponent()->GetImageScale();

		Vector3 NewPosition = ReCalculatePosition(InLadder);
		InLadder.GetTransform()->SetPosition(NewPosition);
		Vector3 NewScale = Vector3(OriginScale.x, OriginScale.y + NewBodyScale.y, 1.0f);
		InLadder.GetTransform()->SetScale(NewScale);

		Vector3 OriginNewDist = NewPosition - OriginPosition;
		InLadder.Head->GetTransform()->SetPosition(InLadder.Head->GetTransform()->GetPosition() - OriginNewDist);
		for (auto Body : InLadder.Bodys)
			Body->GetTransform()->SetPosition(Body->GetTransform()->GetPosition() - OriginNewDist);
		InLadder.Foot->GetTransform()->SetPosition(InLadder.Foot->GetTransform()->GetPosition() - OriginNewDist);
	}

private:
	std::wstring HeadImagePath;
	std::vector<std::wstring> BodyImagePaths;
	std::wstring FootImagePath;

	std::vector<CLadderForm*> Ladders;

};