#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	: CRenderComponent(MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{	
	// 위치정보 업데이트
	Transform()->Binding();

	// 사용할 쉐이더 바인딩
	GetMaterial()->Binding();

	// 렌더링 시작
	GetMesh()->Render();
}