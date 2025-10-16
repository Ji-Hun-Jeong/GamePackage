#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

#include <directxtk/SimpleMath.h>
#include <directxtk/WICTextureLoader.h>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <memory>
#include <cassert>
#include <DirectXMath.h>

#include "RenderCommon.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;