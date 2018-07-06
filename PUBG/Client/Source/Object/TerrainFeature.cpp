#include "stdafx.h"
#include "TerrainFeature.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"
#include "DirectionalLight.h"

TerrainFeature::TerrainFeature(
    const TAG_RES_STATIC tag,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const D3DXVECTOR3& scale)
    : IObject(TAG_OBJECT::TerrainFeature)
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(tag);

    Transform* tr = GetTransform();
    tr->SetPosition(position);
    tr->SetRotation(rotation);
    tr->SetScale(scale);
    tr->Update();

    // setup bounding sphere
    m_boundingSphereTerrainFeature = pEffectMeshRenderer->GetBoundingSphere();
    m_boundingSphereTerrainFeature.center += position;

    CurrentScene()()->InsertObjIntoTotalCellSpace(TAG_OBJECT::TerrainFeature, CurrentScene()()->GetCellIndex(position), this);
}

TerrainFeature::~TerrainFeature()
{
}

void TerrainFeature::OnUpdate()
{
    // do nothing
}

void TerrainFeature::OnRender()
{
    pEffectMeshRenderer->Render(
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });

    m_boundingSphereTerrainFeature.Render();
    for (auto bb : m_boundingBoxes)
        bb->Render();
}

BoundingSphere* TerrainFeature::GetBoundingSphere()
{
    return &m_boundingSphereTerrainFeature;
}

void TerrainFeature::AddBoundingBox(const D3DXMATRIX& transformationMatrix)
{
    BoundingBox* bb = new BoundingBox;
    D3DXVECTOR3 extent(Vector3::ONE * 0.5f);
    D3DXVECTOR3 vS;
    D3DXQUATERNION qR;
    Matrix::GetScaleAndRotation(transformationMatrix, &vS, &qR);
    D3DXVECTOR3 vT = Matrix::GetTranslation(transformationMatrix);

    D3DXMATRIX mS;
    D3DXMatrixScaling(&mS, vS.x, vS.y, vS.z);
    D3DXVec3TransformCoord(&extent, &extent, &mS);

    bb->center = Vector3::ZERO;
    bb->extent = extent;

    D3DXMATRIX invS;
    D3DXMatrixInverse(&invS, nullptr, &mS);
    bb->Update(invS * transformationMatrix);

    m_boundingBoxes.emplace_back(bb);
}
