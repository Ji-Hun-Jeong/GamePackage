#pragma once
#include "MouseInteracter.h"

class CMouseManager
{
public:
	CMouseManager()
		: MouseX(0.0f)
		, MouseY(0.0f)
		, CurrentInteracter(nullptr)
		, bMouseClicked(false)
		, bMouseReleased(false)
		, Camera(nullptr)
	{}
	~CMouseManager() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	CMouseInteracter* NewMouseInteracter()
	{
		CMouseInteracter* NewMouseInteracter = new CMouseInteracter;
		MouseInteracters.emplace_back(NewMouseInteracter);

		return NewMouseInteracter;
	}
	void SetMouseClick(bool bInMouseClicked) { bMouseClicked = bInMouseClicked; }
	void SetMouseRelease(bool bInMouseReleased) { bMouseReleased = bInMouseReleased; }
	void SetMousePosition(float InNDCMouseX, float InNDCMouseY) { MouseX = InNDCMouseX; MouseY = InNDCMouseY; }
	void FindCurrentInteracter()
	{
		CMouseInteracter* NewInteracter = nullptr;
		for (auto Iter = MouseInteracters.begin(); Iter != MouseInteracters.end();)
		{
			auto MouseInteracter = Iter->get();
			if (MouseInteracter->bDestroy)
			{
				if (MouseInteracter == CurrentInteracter)
				{
					bMouseReleased ? MouseInteracter->ActivateMouseReleaseEvent() : MouseInteracter->ActivateMouseExitEvent();
					CurrentInteracter = nullptr;
				}
				Iter = MouseInteracters.erase(Iter);
			}
			else
			{
				NewInteracter = TryFindOnInteracter(*MouseInteracter);
				if (NewInteracter)
					break;
				++Iter;
			}
		}

		if (CurrentInteracter != NewInteracter)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseExitEvent();

			if (NewInteracter)
				NewInteracter->ActivateMouseEnterEvent();
		}

		CurrentInteracter = NewInteracter;

		if (bMouseClicked)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseClickEvent();
			bMouseClicked = false;
		}
		if (bMouseReleased)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseReleaseEvent();
			bMouseReleased = false;
		}
	}
	void SetCamera(class CCamera* InCamera) { Camera = InCamera; }
private:
	CMouseInteracter* TryFindOnInteracter(CMouseInteracter& InMouseInteracter)
	{
		bool bMouseOn = IsMouseOn(InMouseInteracter);
		if (bMouseOn == false)
			return nullptr;

		CMouseInteracter* NewInteracter = &InMouseInteracter;
		for (auto& ChildInteracter : InMouseInteracter.ChildInteracters)
		{
			NewInteracter = TryFindOnInteracter(*ChildInteracter);
			if (NewInteracter)
				break;
		}
		return NewInteracter;
	}
	bool IsMouseOn(const CMouseInteracter& InMouseInteracter);
	
private:
	float MouseX;
	float MouseY;

	std::vector<std::unique_ptr<CMouseInteracter>> MouseInteracters;
	CMouseInteracter* CurrentInteracter;

	bool bMouseClicked;
	bool bMouseReleased;

	class CCamera* Camera;

};

