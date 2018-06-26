#include "stdafx.h"
#include "Church.h"
#include "EffectMeshRenderer.h"
#include "ResPathFileName.h"

Church::Church()
    : IObject()
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();

    pair<string, string> p = ResPathFileName::Get(TAG_RES_STATIC::Church);
    pEffectMeshRenderer->SetEffectMesh(
        Resource()()->FindEffectMesh(p.first, p.second));
}

Church::~Church()
{
}

void Church::OnUpdate()
{
}

void Church::OnRender()
{
    pEffectMeshRenderer->Render();
}
