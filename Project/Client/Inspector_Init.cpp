#include "pch.h"
#include "Inspector.h"


void Inspector::Init()
{
	// 자식 UI 생성
	CreateComponentUI();

	CreateAssetUI();
}


#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"
#include "MeshRenderUI.h"
#include "FlipbookUI.h"

void Inspector::CreateComponentUI()
{
	m_ComUI[TRANSFORM] = new TransformUI;
	m_ComUI[TRANSFORM]->SetSizeAsChild(ImVec2(0.f, 130.f));
	AddChildUI(m_ComUI[TRANSFORM]);

	m_ComUI[COLLIDER2D] = new Collider2DUI;
	m_ComUI[COLLIDER2D]->SetSizeAsChild(ImVec2(0.f, 100.f));
	AddChildUI(m_ComUI[COLLIDER2D]);

	m_ComUI[CAMERA] = new CameraUI;
	m_ComUI[CAMERA]->SetSizeAsChild(ImVec2(0.f, 200.f));
	AddChildUI(m_ComUI[CAMERA]);

	m_ComUI[LIGHT2D] = new Light2DUI;
	m_ComUI[LIGHT2D]->SetSizeAsChild(ImVec2(0.f, 200.f));
	AddChildUI(m_ComUI[LIGHT2D]);

	m_ComUI[MESHRENDER] = new MeshRenderUI;
	m_ComUI[MESHRENDER]->SetSizeAsChild(ImVec2(0.f, 100.f));
	AddChildUI(m_ComUI[MESHRENDER]);

}


#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "MaterialUI.h"
#include "PrefabUI.h"
#include "SoundUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"
#include "SpriteUI.h"
#include "FlipbookUI.h"
void Inspector::CreateAssetUI()
{
	m_AssetUI[MESH] = new MeshUI;
	m_AssetUI[MESH]->SetActive(false);
	AddChildUI(m_AssetUI[MESH]);

	m_AssetUI[MESHDATA] = new MeshDataUI;
	m_AssetUI[MESHDATA]->SetActive(false);
	AddChildUI(m_AssetUI[MESHDATA]);

	m_AssetUI[TEXTURE] = new TextureUI;
	m_AssetUI[TEXTURE]->SetActive(false);
	AddChildUI(m_AssetUI[TEXTURE]);

	m_AssetUI[MATERIAL] = new MaterialUI;
	m_AssetUI[MATERIAL]->SetActive(false);
	AddChildUI(m_AssetUI[MATERIAL]);

	m_AssetUI[PREFAB] = new PrefabUI ;
	m_AssetUI[PREFAB]->SetActive(false);
	AddChildUI(m_AssetUI[PREFAB]);

	m_AssetUI[SOUND] = new SoundUI;
	m_AssetUI[SOUND]->SetActive(false);
	AddChildUI(m_AssetUI[SOUND]);

	m_AssetUI[GRAPHIC_SHADER] = new GraphicShaderUI;
	m_AssetUI[GRAPHIC_SHADER]->SetActive(false);
	AddChildUI(m_AssetUI[GRAPHIC_SHADER]);

	m_AssetUI[COMPUTE_SHADER] = new ComputeShaderUI;
	m_AssetUI[COMPUTE_SHADER]->SetActive(false);
	AddChildUI(m_AssetUI[COMPUTE_SHADER]);

	m_AssetUI[SPRITE] = new SpriteUI;
	m_AssetUI[SPRITE]->SetActive(false);
	AddChildUI(m_AssetUI[SPRITE]);

	m_AssetUI[FLIPBOOK] = new FlipbookUI;
	m_AssetUI[FLIPBOOK]->SetActive(false);
	AddChildUI(m_AssetUI[FLIPBOOK]);


}
