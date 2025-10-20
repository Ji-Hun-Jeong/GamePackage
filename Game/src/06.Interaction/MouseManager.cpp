#include "pch.h"
#include "MouseManager.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/Component/InteractionComponent.h"
#include "01.Base/Object/Camera.h"

class CSetMouseInteracter : public INewObjectEvent
{
public:
	CSetMouseInteracter(CMouseManager& InMouseManager)
		: MouseManager(InMouseManager)
	{}

private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		if (CInteractionComponent::GetStaticType() == InNewObject.GetType())
		{
			CInteractionComponent& InteractionComponent = static_cast<CInteractionComponent&>(InNewObject);
			InteractionComponent.SetMouseInteracter(MouseManager.NewMouseInteracter());
		}
		if (CCamera::GetStaticType() == InNewObject.GetType())
		{
			MouseManager.SetCamera(static_cast<CCamera*>(&InNewObject));
		}
	}
	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CSetMouseInteracter>(*this));
}

bool CMouseManager::IsMouseOn(const CMouseInteracter& InMouseInteracter)
{
	if (InMouseInteracter.Size.x == 0.0f && InMouseInteracter.Size.y == 0.0f)
	{
		std::cout << "MouseInteracterSize Is Zero\n";
		return false;
	}

	// 사각형의 중심 위치와 크기
	float centerX = InMouseInteracter.Position.x;
	float centerY = InMouseInteracter.Position.y;
	float halfWidth = InMouseInteracter.Size.x * 0.5f;
	float halfHeight = InMouseInteracter.Size.y * 0.5f;

	// 사각형의 경계
	float left = centerX - halfWidth;
	float right = centerX + halfWidth;
	float top = centerY + halfHeight;
	float bottom = centerY - halfHeight;
	float FinalMouseX = MouseX + Camera->GetTransform()->GetFinalPosition().x;
	float FinalMouseY = MouseY + Camera->GetTransform()->GetFinalPosition().y;

	// 마우스가 사각형 안에 있는지 확인
	return (FinalMouseX >= left && FinalMouseX <= right &&
		FinalMouseY >= bottom && FinalMouseY <= top);
}