#pragma once
#include "CRenderComponent.h"

#include "CMesh.h"
#include "CGraphicShader.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void FinalTick() override;
    virtual void Render() override;

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

