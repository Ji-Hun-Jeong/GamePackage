#include "pch.h"
#include "SpriteRenderer.h"
#include <Core/public/Window.h>
#include "01.Base/World/World.h"
#include "01.Base/Object/Camera.h"

class CInitalizeRenderComponent : public INewObjectEvent
{
public:
	CInitalizeRenderComponent(CSpriteRenderer& InRenderer, CRenderResourceLoader& InRenderResourceLoader)
		: Renderer(InRenderer)
		, RenderResourceLoader(InRenderResourceLoader)
	{}
private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		CRenderComponent& RenderComponent = static_cast<CRenderComponent&>(InNewObject);
		CRenderStateObject* RenderStateObject = Renderer.NewRenderStateObject();

		RenderComponent.SetRenderStateObject(RenderStateObject);
		RenderComponent.SetRenderResourceLoader(&RenderResourceLoader);
		RenderComponent.SetRenderer(&Renderer);
		RenderComponent.SetMesh(0);
		RenderComponent.SetPSO(EPSOType::Basic);
	}

	CSpriteRenderer& Renderer;
	CRenderResourceLoader& RenderResourceLoader;
};

class CCameraSetScreenSize : public INewObjectEvent
{
public:
	CCameraSetScreenSize(CSpriteRenderer& InRenderer)
		: Renderer(InRenderer)
	{}
private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		static_cast<CCamera&>(InNewObject).SetScreenSize(Renderer.GetScreenWidth(), Renderer.GetScreenHeight());
	}

	CSpriteRenderer& Renderer;
};

void CSpriteRenderer::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectTypeEvent(CRenderComponent::GetStaticType(), std::make_unique<CInitalizeRenderComponent>(*this, RenderResourceLoader));
	InWorld.AddNewObjectTypeEvent(CCamera::GetStaticType(), std::make_unique<CCameraSetScreenSize>(*this));
}

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