#pragma once

#include "CTexture.h"

class CCamera;
class CGameObject;
class CLight2D;
class CStructuredBuffer;



class CRenderMgr
	: public singleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>		m_vecCam;
	CCamera*				m_EditorCam;

	vector<CLight2D*>		m_vecLight2D;
	CStructuredBuffer*		m_Light2DBuffer;

	CGameObject*			m_DebugObject;
	vector<tDebugShapeInfo> m_vecDebugInfo;

	Ptr<CTexture>			m_PostProcessTex;

	void(CRenderMgr::		*m_RenderFunc)();

public:
	void RegisterCamera(CCamera* _Cam, int _Priority);	
	void RegisterEditorCamera(CCamera* _Cam) { m_EditorCam = _Cam; }
	void ClearCamera() { m_vecCam.clear(); }


	void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecDebugInfo.push_back(_info); }
	void CopyRenderTarget();
	Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }

	void SetRenderMode(bool _IsPlay)
	{
		if(_IsPlay)
			m_RenderFunc = &CRenderMgr::Render_Play;
		else
			m_RenderFunc = &CRenderMgr::Render_Editor;
	}

public:
	void Init();
	void Tick();

private:
	void Binding();	
	void Render_Play();
	void Render_Editor();

	void Clear();
	void DebugRender();
};

