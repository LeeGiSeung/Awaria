#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"

#include "CPrefab.h"

CEngine::CEngine()
    : m_hInst(nullptr)
    , m_hMainWnd(nullptr)    
    , m_FMODSystem(nullptr)
{
}

CEngine::~CEngine()
{
    if (nullptr != m_FMODSystem)
    {
        m_FMODSystem->release();
        m_FMODSystem = nullptr;
    }
}

int CEngine::Init(HINSTANCE _Inst, UINT _Width, UINT _Height
    , void(*_SAVE)(CGameObject*, FILE*), CGameObject* (*_LOAD)(FILE*))
{
    m_hInst = _Inst;
    m_vResolution = Vec2(_Width, _Height);

    // ������ ���̺� �ε� �Լ� �ּ� ���
    CPrefab::g_Save = (GAMEOBJECT_SAVE)_SAVE;
    CPrefab::g_Load = (GAMEOBJECT_LOAD)_LOAD;

    if (FAILED(CreateMainWindow()))
    {
        return E_FAIL;
    }

    // Device �ʱ�ȭ
    if (FAILED(CDevice::GetInst()->Init(m_hMainWnd, m_vResolution)))
    {
        return E_FAIL;
    }

    // FMOD �ʱ�ȭ		
    FMOD::System_Create(&m_FMODSystem);
    assert(m_FMODSystem);

    // 32�� ä�� ����
    m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);


    // Manager �ʱ�ȭ
    CPathMgr::Init();
    CKeyMgr::GetInst()->Init();
    CTimeMgr::GetInst()->Init();
    CAssetMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();
    CRenderMgr::GetInst()->Init();

    return S_OK;
}

void CEngine::Progress()
{
    // Manager Tick
    CTimeMgr::GetInst()->Tick();
    CKeyMgr::GetInst()->Tick();

    // FMOD Tick
    m_FMODSystem->update();

    // Level Progress
    CLevelMgr::GetInst()->Progress();

    // Collision Check
    CCollisionMgr::GetInst()->Tick();

    // Render
    CRenderMgr::GetInst()->Tick();

    // Task
    CTaskMgr::GetInst()->Tick();
}

int CEngine::CreateMainWindow()
{
    m_hMainWnd = CreateWindowW(L"MyWindow", L"DirectX53", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hMainWnd)
        return E_FAIL;

    ShowWindow(m_hMainWnd, true);
    UpdateWindow(m_hMainWnd);

    // �Էµ� �ػ󵵿� �´� ������ ũ�� ����
    RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
    SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

    return S_OK;
}
