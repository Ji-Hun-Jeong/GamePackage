#include "pch.h"
#include "World.h"
#include "00.App/Screen.h"
#include "01.Base/Actor/Camera.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderSorter.h"
#include "04.Renderer/ImGuiManager.h"
#include "06.Interaction/MouseInteractionManager.h"
#include "07.Collision/PixelCollisionManager.h"

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	for (auto& WorldActor : WorldActors)
	{
		WorldActor->RenderActor(InRenderer);
	}
	InRenderer.Draw();
}