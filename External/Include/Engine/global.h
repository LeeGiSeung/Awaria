#pragma once


#include <Windows.h>
#include <typeinfo>
#include <filesystem>
using namespace std::filesystem;


// ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;

// DirectX 라이브러리 링크
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// DirectxTex 라이브러리 링크
#include <DirectxTex/DirectXTex.h>
#include <DirectxTex/DirectXTex.inl>

#ifndef _DEBUG
#pragma comment(lib, "DirectxTex/DirectXTex.lib")
#else
#pragma comment(lib, "DirectxTex/DirectXTex_debug.lib")
#endif

// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif



#include <vector>
#include <list>
#include <map>
#include <string>
using namespace std;

#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"

#include "singleton.h"
#include "ptr.h"