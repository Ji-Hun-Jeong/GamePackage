#include "pch.h"
#include "SpriteRenderer.h"
#include <Core/public/Window.h>
#include "01.Base/World/World.h"
#include "01.Base/Actor/Camera.h"

//class CCameraSetScreenSize : public INewObjectEvent
//{
//public:
//	CCameraSetScreenSize(CSpriteRenderer& InRenderer)
//		: Renderer(InRenderer)
//	{}
//private:
//	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
//	{
//		static_cast<CCamera&>(InNewObject).SetScreenSize(Renderer.GetScreenWidth(), Renderer.GetScreenHeight());
//	}
//
//	CSpriteRenderer& Renderer;
//};
//
//void CSpriteRenderer::InitalizeFromWorld(CWorld& InWorld)
//{
//	InWorld.AddNewObjectTypeEvent(CCamera::GetStaticType(), std::make_unique<CCameraSetScreenSize>(*this));
//}

// 모듈이 내부를 전달 불가하기 때문에 인터페이스 사용해야될듯
class CRefreshScreen : public Core::IWindowResize
{
public:
	CRefreshScreen(CSpriteRenderer& InSprireRenderer)
		: SpriteRenderer(InSprireRenderer)
	{}
	void WindowResize(UINT InNewScreenWidth, UINT InNewScreenHeight) override
	{
		SpriteRenderer.SetWindowSize(InNewScreenWidth, InNewScreenHeight);
	}
private:
	CSpriteRenderer& SpriteRenderer;
};

void CSpriteRenderer::InitalizeFromWindow(Core::CWindow& InWindow)
{
	InWindow.RegistWindowResizeEvent(std::make_unique<CRefreshScreen>(*this));
}