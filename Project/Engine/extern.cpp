#include "pch.h"


tTransform g_Trans = {};

tGlobal g_global = {};

const Matrix Matrix::Identity = Matrix( 1.f, 0.f, 0.f, 0.f
									  , 0.f, 1.f, 0.f, 0.f
									  , 0.f, 0.f, 1.f, 0.f
									  , 0.f, 0.f, 0.f, 1.f);

extern const char* COLLISION_LAYER_STR[] =
{
	"TILE",
	"WALL",
	"TRAP",
	"GHOST",
	"GENERATOR",
	"PLAYER",
	"END"
};

extern const wchar_t* COLLISION_LAYER_WSTR[] =
{
	L"TILE",
	L"WALL",
	L"TRAP",
	L"GHOST",
	L"GENERATOR",
	L"PLAYER",
	L"END"
};

extern const char* COMPONENT_TYPE_STR[] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"LIGHT2D",
	"LIGHT3D",
	"CAMERA",
	"BOUNDINGBOX",
	"MESHRENDER",
	"SPRITERENDER",
	"FLIPBOOKRENDER",
	"TILERENDER",
	"PARTICLERENDER",
	"SKYBOX",
	"DECAL",
	"LANDSCAPE",
	"COMPONENT_END",
	"SCRIPT",
};

extern const wchar_t* COMPONENT_TYPE_WSTR[] =
{
	L"TRANSFORM",
	L"COLLIDER2D",
	L"COLLIDER3D",
	L"LIGHT2D",
	L"LIGHT3D",
	L"CAMERA",
	L"BOUNDINGBOX",
	L"MESHRENDER",
	L"SPRITERENDER",
	L"FLIPBOOKRENDER",
	L"TILERENDER",
	L"PARTICLERENDER",
	L"SKYBOX",
	L"DECAL",
	L"LANDSCAPE",
	L"COMPONENT_END",
	L"SCRIPT",
};

extern const char* ASSET_TYPE_STR[] =
{
	"MESH",
	"MESHDATA",
	"TEXTURE",
	"MATERIAL",
	"PREFAB",
	"SOUND",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
	"SPRITE",
	"FLIPBOOK",
};

extern const wchar_t* ASSET_TYPE_WSTR[] =
{
	L"MESH",
	L"MESHDATA",
	L"TEXTURE",
	L"MATERIAL",
	L"PREFAB",
	L"SOUND",
	L"GRAPHIC_SHADER",
	L"COMPUTE_SHADER",
	L"SPRITE",
	L"FLIPBOOK",
};