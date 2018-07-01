#include "stdafx.h"
#include "Shader.h"

const char* Shader::World      = "World";
const char* Shader::View       = "View";
const char* Shader::Projection = "Projection";
const char* Shader::bEmissiveColor = "bEmissiveColor";
const char* Shader::bLight = "bLight";
const char* Shader::lightDirection = "lightDirection";
const char* Shader::DiffuseColor = "DiffuseColor";
const char* Shader::SpecularPower = "SpecularPower";
const char* Shader::SpecularColor = "SpecularColor";
const char* Shader::EmissiveColor = "EmissiveColor";

void Shader::Draw(
    LPD3DXEFFECT pEffect, 
    D3DXHANDLE hParam, 
    LPD3DXMESH pMesh, 
    const DWORD attribID, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(pEffect && pMesh && setGlobalVariable && "argument is null.");

    if (hParam)
        pEffect->ApplyParameterBlock(hParam);

    pEffect->SetMatrix(Shader::View, &CurrentCamera()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::Projection,
        &CurrentCamera()()->GetProjectionMatrix());

    setGlobalVariable(pEffect);
    pEffect->CommitChanges();

    UINT numPasses = 0;
    pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pEffect->BeginPass(i);
        pMesh->DrawSubset(attribID);
        pEffect->EndPass();
    }
    pEffect->End();
}

void Shader::Draw(
    const std::vector<EffectParam>& effectParams, 
    LPD3DXMESH pMesh, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(pMesh && setGlobalVariable && "Effectf::Draw(), argument is null.");

    for (std::size_t i = 0; i < effectParams.size(); ++i)
    {
        Draw(
            effectParams[i].pEffect, 
            effectParams[i].hParam, 
            pMesh, 
            i, 
            setGlobalVariable);
    }
}

void Shader::Draw(
    LPD3DXEFFECT pEffect, 
    D3DXHANDLE hParam,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
    const std::function<void()>& drawVertices)
{
    assert(pEffect && setGlobalVariable && drawVertices && 
        "Effectf::Draw(), argument is null.");

    if (hParam)
        pEffect->ApplyParameterBlock(hParam);

    pEffect->SetMatrix(Shader::View, &CurrentCamera()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::Projection,
        &CurrentCamera()()->GetProjectionMatrix());

    setGlobalVariable(pEffect);
    pEffect->CommitChanges();

    UINT numPasses = 0;
    pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pEffect->BeginPass(i);
        drawVertices();
        pEffect->EndPass();
    }
    pEffect->End();
}

void Shader::Draw(
    const std::vector<EffectParam>& effectParams,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
    const std::function<void()>& drawVertices)
{
    assert(setGlobalVariable && drawVertices && 
        "Effectf::Draw(), argument is null.");

    for (std::size_t i = 0; i < effectParams.size(); ++i)
    {
        Draw(
            effectParams[i].pEffect, 
            effectParams[i].hParam, 
            setGlobalVariable, 
            drawVertices);
    }
}
