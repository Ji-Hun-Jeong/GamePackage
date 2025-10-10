@echo off

rem =====================================================================
rem vcpkg 설치 경로 지정
rem vcpkg new --application
rem vcpkg add port fmt
rem c++ 속성에서 명령줄에 /utf-8넣기
rem =====================================================================

set VCPKG_DIR=C:\vcpkg
if not exist "%VCPKG_DIR%" (
    echo vcpkg가 설치되어 있지 않습니다.
    echo VCPKG_DIR 경로를 수정하거나, 수동으로 설치해주세요.
    pause
    exit /b 1
)

echo ===================================================================
echo [Game] 프로젝트 의존성 설치 시작...
echo ===================================================================
cd Game
"%VCPKG_DIR%\vcpkg.exe" install
cd ..
echo [Game] 프로젝트 의존성 설치 완료.
echo.

echo ===================================================================
echo [Renderer] 프로젝트 의존성 설치 시작...
echo ===================================================================
cd Renderer
"%VCPKG_DIR%\vcpkg.exe" install
cd ..
echo [Renderer] 프로젝트 의존성 설치 완료.
echo.

echo 모든 프로젝트의 의존성 설치가 완료되었습니다.
pause