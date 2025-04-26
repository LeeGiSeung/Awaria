#pragma once
#include "CRenderComponent.h"

#include "CFlipbook.h"

class CFlipbookRender :
    public CRenderComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;
    Ptr<CFlipbook>          m_CurFlipbook;
    int                     m_SpriteIdx;

    float                   m_Time;
    float                   m_FPS;
    bool                    m_Repeat;
    bool                    m_Finish;

    int                     m_Flipbook_idx;
    int                     Dir;


public:
    void AddFlipbook(int _Idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat, int _SpriteIdx = 0)
    {
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Time = 0.f;
        m_Finish = false;
        m_Repeat = _Repeat;
        m_SpriteIdx = 0;
        m_Flipbook_idx = _Idx;
    }
    bool IsFinish() { return m_Finish; }
    int GetSpriteIdx() { return m_SpriteIdx; }
    int GetFlipbookIdx() { return m_Flipbook_idx; }

    void SetDir(int _in) { Dir = _in; } //0 Left 1 Right
    int GetDir() { return Dir; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateFlipbookMaterial();

public:
    CLONE(CFlipbookRender);
    CFlipbookRender();
    ~CFlipbookRender();
};

