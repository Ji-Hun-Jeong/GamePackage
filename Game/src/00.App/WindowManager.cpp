#include "pch.h"
#include "WindowManager.h"

#include <shobjidl.h> // IFileOpenDialog
#include <wrl/client.h> // ComPtr (선택 사항이지만 매우 권장)
using Microsoft::WRL::ComPtr;

CWindowManager::CWindowManager()
{

}

bool CWindowManager::TryGetFilePathByDialog(std::wstring* OutFilePath)
{
	ComPtr<IFileOpenDialog> pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(pFileOpen.GetAddressOf()));

	if (FAILED(hr))
		assert(0);

	COMDLG_FILTERSPEC rgSpec[] =
	{
		{ L"이미지 파일", L"*.png;*.jpg;*.bmp" },
		{ L"모든 파일", L"*.*" },
	};
	pFileOpen->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);

	hr = pFileOpen->Show(NULL);

	if (FAILED(hr))
		false;

	// 6. 선택된 파일의 경로 가져오기
	ComPtr<IShellItem> pItem;
	hr = pFileOpen->GetResult(&pItem);
	if (SUCCEEDED(hr))
	{
		PWSTR pszFilePath = nullptr;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

		if (SUCCEEDED(hr))
		{
			*OutFilePath = pszFilePath;
			CoTaskMemFree(pszFilePath);
		}
		return true;
	}
	return false;
}