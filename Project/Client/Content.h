#pragma once
#include "EditorUI.h"


class Content :
    public EditorUI
{
private:
    class TreeUI*       m_Tree;
    vector<wstring>     m_ContentFile;

    HANDLE              m_NotifyHandle; // Content 폴더에 변경점이 생겼는지 확인하는 커널객체 핸들


public:
    void Selection(DWORD_PTR _Node);
    void Renew();

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;    

private:
    void FindContentFile(const wstring& _FolderPath);
    ASSET_TYPE GetAssetType(const wstring& _RelativePath);
    void Reload();
    void ObserveContent();


public:
    Content();
    ~Content();
};

