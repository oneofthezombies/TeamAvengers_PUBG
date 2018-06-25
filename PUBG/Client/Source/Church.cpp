#include "stdafx.h"
#include "Church.h"
#include "EffectMeshRenderer.h"

Church::Church()
    : IObject()
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(
        Resource()()->FindEffectMesh("./Resource/Church/", "Church.x"));
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
