#pragma once
#include <Core/public/Application.h>

#include "CoreSystem.h"
#include "Screen.h"
#include "01.Base/World/World.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/ImGuiManager.h"
#include "06.Interaction/MouseInteractionManager.h"
#include "07.Collision/PixelCollisionManager.h"

/*
리소스를 개발단계에서 사용한다음 빌드폴더에도 넣는 방법

1. 우선, Resources 폴더를 build 폴더가 아닌, 소스 코드가 있는 메인 프로젝트 폴더로 옮깁니다. (예: .vcxproj 파일이 있는 곳)

2. Visual Studio의 솔루션 탐색기에서 해당 프로젝트를 마우스 오른쪽 버튼으로 클릭 후 **[속성(Properties)]**으로 들어갑니다.

3. **[빌드 이벤트(Build Events)] -> [빌드 후 이벤트(Post-Build Event)]**로 이동합니다.

4. 오른쪽 [명령줄(Command Line)] 입력창에 아래 명령어를 복사해서 붙여넣습니다.
	xcopy "$(ProjectDir)Resources" "$(OutDir)Resources" /s /y /i /d
*/
class CGame : public Core::CApplication
{
public:
	CGame(UINT InScreenWidth, UINT InScreenHeight);
	~CGame();

public:
	bool Process() override;
	void ShutDown() override;

private:
	std::unique_ptr<Graphics::IGraphicInfra> GraphicInfra;
	CSpriteRenderer SpriteRenderer;
	CImGuiManager& ImGuiManager;
	CPixelCollisionManager PixelCollisionManager;
	CCollisionManager CollisionManager;
	CScreen Screen;

	CMouseInteractionManager MouseInteractionManager;

	CWorld World;
};


