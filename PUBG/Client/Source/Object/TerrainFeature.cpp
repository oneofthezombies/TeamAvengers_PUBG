#include "stdafx.h"
#include "TerrainFeature.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"
#include "Light.h"

const float TerrainFeature::DISTANCE_FOR_CULLING = 10000.0f;

TerrainFeature::TerrainFeature(
    const TAG_RES_STATIC tag,
    const std::string&   name,
    const D3DXVECTOR3&   position,
    const D3DXVECTOR3&   rotation,
    const D3DXVECTOR3&   scale)
    : IObject(TAG_OBJECT::TerrainFeature)
    , m_name(name)
    , m_tagResStatic(tag)
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
    m_boundingSphere = pEffectMeshRenderer->GetBoundingSphere();
    m_boundingSphere.position = position;

    //Total Cell Space에 넣는것
    CurrentScene()()->InsertObjIntoTotalCellSpace(TAG_OBJECT::TerrainFeature, CurrentScene()()->GetCellIndex(position), this);
}

TerrainFeature::~TerrainFeature()
{
}

void TerrainFeature::OnUpdate()
{
    //distance culling
    D3DXVECTOR3 vLength = GetTransform()->GetPosition() - CurrentCamera()()->GetPosition();
    if (D3DXVec3Length(&vLength) < Shader::SHADOW_CULLING_DISTANCE)
    {
        switch (m_tagResStatic)
        {
        case TAG_RES_STATIC::Museum:
        {
            D3DXMATRIX mat = GetTransform()->GetTransformationMatrix();
            EffectMesh* EM = pEffectMeshRenderer->GetEffectMesh();
            for (int i = 0; i < 1; i++)
            {
                Shader()()->AddShadowSource(mat, EM->m_pMesh, static_cast<DWORD>(i));
            }
        }
        break;
        case TAG_RES_STATIC::PoliceStation:
        {
            D3DXMATRIX mat = GetTransform()->GetTransformationMatrix();
            EffectMesh* EM = pEffectMeshRenderer->GetEffectMesh();
            for (int i = 0; i < 1; i++)
            {
                Shader()()->AddShadowSource(mat, EM->m_pMesh, static_cast<DWORD>(i));
            }
        }
            break;
        default:
            Shader()()->AddShadowSource(
                GetTransform()->GetTransformationMatrix(),
                pEffectMeshRenderer->GetEffectMesh());
            break;
        }
    }

    // clear isRender
    {
        m_boundingSphere.isRender = false;
        for (auto& box : m_boundingBoxes)
            box.isRender = false;
    }
}

void TerrainFeature::OnRender()
{
    //if (DebugMgr()()->IsHoonsComputer())
    //{
    //    const TAG_RES_STATIC tag = m_tagResStatic;
    //    if (tag == TAG_RES_STATIC::AlaskaCedar ||
    //        tag == TAG_RES_STATIC::AmericanElem ||
    //        tag == TAG_RES_STATIC::LondonPlane ||

    //        tag == TAG_RES_STATIC::DeadGrass ||
    //        tag == TAG_RES_STATIC::Dogwood ||
    //        tag == TAG_RES_STATIC::Grass_1 ||
    //        tag == TAG_RES_STATIC::Grass_2 ||

    //        tag == TAG_RES_STATIC::Rock_1 ||
    //        tag == TAG_RES_STATIC::Rock_2 ||

    //        tag == TAG_RES_STATIC::Powerline_1 ||
    //        tag == TAG_RES_STATIC::Powerline_2 ||
    //        tag == TAG_RES_STATIC::Wall_1 ||
    //        tag == TAG_RES_STATIC::Wall_2 ||
    //        tag == TAG_RES_STATIC::Wall_3 ||
    //        tag == TAG_RES_STATIC::Wall_4 ||
    //        tag == TAG_RES_STATIC::Wall_End || 
    //        tag == TAG_RES_STATIC::Wall_End_Long ||
    //        
    //        tag == TAG_RES_STATIC::Museum ||
    //        tag == TAG_RES_STATIC::PoliceStation ||
    //        
    //        tag == TAG_RES_STATIC::RadioTower_1 ||
    //        tag == TAG_RES_STATIC::RadioTower_2 ||
    //        
    //        tag == TAG_RES_STATIC::Sandbag_1 ||
    //        tag == TAG_RES_STATIC::Sandbag_2 ||

    //        tag == TAG_RES_STATIC::Silo_A ||
    //        tag == TAG_RES_STATIC::Silo_B ||
    //        
    //        tag == TAG_RES_STATIC::BrokenTractorGunnyBag ||
    //        tag == TAG_RES_STATIC::HayBale_1 ||
    //        
    //        tag == TAG_RES_STATIC::BrokenBus ||
    //        tag == TAG_RES_STATIC::BrokenCar ||
    //        
    //        tag == TAG_RES_STATIC::OldWoodenShed_1 ||
    //        tag == TAG_RES_STATIC::PicketFence_Short_A ||
    //        tag == TAG_RES_STATIC::PicketFence_Short_B)
    //    {
    //        for (auto& bb : m_boundingBoxes)
    //        {
    //            bb.isRender = true;
    //            bb.Render();
    //        }

    //        return;
    //    }
    //}

    //frustum culling
    if (CurrentCamera()()->IsObjectInsideFrustum(
        m_boundingSphere.center + m_boundingSphere.position,
        m_boundingSphere.radius))
    {
        //distance culling
        D3DXVECTOR3 vlength = (m_boundingSphere.center + m_boundingSphere.position) - CurrentCamera()()->GetPosition();

        float distanceForCulling = DISTANCE_FOR_CULLING;
        //if (DebugMgr()()->IsHoonsComputer())
        //    distanceForCulling = 5000.0f;
        
        if (D3DXVec3Length(&vlength) < distanceForCulling)
        {
            if (DebugMgr()()->IsHoonsComputer())
            {
                for (auto& bb : m_boundingBoxes)
                {
                    Shader::Draw(
                        Resource()()->GetEffect("./Resource/VertexPN.fx"),
                        nullptr,
                        Resource()()->GetBoundingBoxMesh(),
                        0,
                        [&bb](LPD3DXEFFECT pEffect)
                    {
                        D3DXMATRIX m;
                        D3DXVECTOR3 position(bb.center + bb.position);
                        D3DXMatrixTransformation(
                            &m,
                            nullptr,
                            nullptr,
                            &D3DXVECTOR3(bb.extent.x, bb.extent.y, bb.extent.z), 
                            nullptr, 
                            &bb.rotation, 
                            &position);

                        pEffect->SetMatrix(
                            Shader::World,
                            &m);
                    });
                }
            }
            else
            {
                pEffectMeshRenderer->Render(
                    [this](LPD3DXEFFECT pEffect)
                {
                    pEffect->SetMatrix(
                        Shader::World,
                        &GetTransform()->GetTransformationMatrix());
                });
            }

            for (auto& bb : m_boundingBoxes)
                bb.Render();

            m_boundingSphere.Render();
        }
    }    
}

void TerrainFeature::AddBoundingBox(const D3DXMATRIX& transformationMatrix)
{
    m_boundingBoxes.emplace_back(BoundingBox::Create(transformationMatrix));
}

TAG_RES_STATIC TerrainFeature::GetTagResStatic() const
{
    return m_tagResStatic;
}
