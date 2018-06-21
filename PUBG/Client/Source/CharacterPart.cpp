#include "stdafx.h"
#include "CharacterPart.h"
#include "Collider.h"
#include "Renderer.h"
#include "MeshFilterAndAnimator.h"
#include "UIText.h"

CharacterPart::CharacterPart(const TAG_COLLIDER_CHARACTER_PART tag,
    MeshFilter* pMeshFilter, ICollisionListener* pCollisionListener)
    : IObject()
    , m_tag(tag)
    , pBoxCollider(nullptr)
    , pBoxColliderRenderer(nullptr)
{
    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::BODY_OF_PLAYER_1);
    pBoxCollider->SetListener(pCollisionListener);
    pBoxColliderRenderer = AddComponent<BoxColliderRenderer>();

    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::HEAD:
        {
            AddFrame("hair_f_01", pMeshFilter);
            AddFrame("hair_b_01", pMeshFilter);
            AddFrame("head", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::NECK, 
                pMeshFilter, pCollisionListener));

            auto pF = pMeshFilter->FindFrame("F_Face_03");
            auto pMeshContainer = static_cast<MeshContainer*>(pF->pMeshContainer);
            void* pData = nullptr;
            auto pMesh = pMeshContainer->pEffectMesh->pMesh;
            pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pData);
            D3DXVECTOR3 min, max;
            D3DXComputeBoundingBox((D3DXVECTOR3*)pData, 
                pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), 
                &min, &max);
            pMeshContainer->pEffectMesh->pMesh->UnlockVertexBuffer();
            min.y *= 0.5f;
            max.y *= 0.5f;
            pBoxCollider->Init(min, max);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::NECK:
        {
            AddFrame("head", pMeshFilter);
            AddFrame("neck_01", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::BREAST,
                pMeshFilter, pCollisionListener));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::CLAVICLE_LEFT, pMeshFilter, 
                pCollisionListener));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::BREAST:
        {
            AddFrame("neck_01", pMeshFilter);
            AddFrame("spine_03", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER:
        {
            AddFrame("spine_03", pMeshFilter);
            AddFrame("spine_02", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER:
        {
            AddFrame("spine_02", pMeshFilter);
            AddFrame("spine_01", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::WAIST, 
                pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::WAIST:
        {
            AddFrame("pelvis", pMeshFilter);
            AddFrame("spine_01", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER, pMeshFilter, pCollisionListener));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_LEFT:
        {
            AddFrame("clavicle_l", pMeshFilter);
            AddFrame("upperarm_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f), 
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT:
        {
            AddFrame("clavicle_r", pMeshFilter);
            AddFrame("upperarm_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER:
        {
            AddFrame("upperarm_l", pMeshFilter);
            AddFrame("lowerarm_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER:
        {
            AddFrame("lowerarm_l", pMeshFilter);
            AddFrame("hand_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_LEFT, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER:
        {
            AddFrame("upperarm_r", pMeshFilter);
            AddFrame("lowerarm_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER:
        {
            AddFrame("lowerarm_r", pMeshFilter);
            AddFrame("hand_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_LEFT:
        {
            AddFrame("hand_l", pMeshFilter);
            AddFrame("middle_01_l", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT:
        {
            AddFrame("hand_r", pMeshFilter);
            AddFrame("middle_01_r", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER:
        {
            AddFrame("thigh_l", pMeshFilter);
            AddFrame("calf_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER:
        {
            AddFrame("calf_l", pMeshFilter);
            AddFrame("foot_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER:
        {
            AddFrame("thigh_r", pMeshFilter);
            AddFrame("calf_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER:
        {
            AddFrame("calf_r", pMeshFilter);
            AddFrame("foot_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT, pMeshFilter, pCollisionListener));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT:
        {
            AddFrame("foot_l", pMeshFilter);
            AddFrame("ball_l", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT:
        {
            AddFrame("foot_r", pMeshFilter);
            AddFrame("ball_r", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    default:
        {
            assert(false && "You have not created all cases.");
        }
        break;
    }

    assert(m_Frames.front() && "CharacterPart::Constructor() failed. frame is null.");
    D3DXVECTOR3 v(GetTranslation(m_Frames.front()->TransformationMatrix));
    for (auto& f : m_Frames)
    {
        assert(f && "CharacterPart::Constructor() failed. frame is null.");
        v = (v + GetTranslation(f->TransformationMatrix)) * 0.5f;
    }
}

CharacterPart::~CharacterPart()
{
}

void CharacterPart::OnUpdate()
{
    if (m_tag == TAG_COLLIDER_CHARACTER_PART::HEAD)
    {
        auto topFront = GetTranslation(m_Frames[0]->CombinedTM);
        auto topBack = GetTranslation(m_Frames[1]->CombinedTM);
        auto top = (topFront + topBack) * 0.5f;
        auto bottom = GetTranslation(m_Frames[2]->CombinedTM);
        auto center = (top + bottom) * 0.5f;
        D3DXMATRIX m = m_Frames[2]->CombinedTM;
        m._41 = center.x;
        m._42 = center.y;
        m._43 = center.z;
        pBoxCollider->Update(m);
    }
    else
    {
        D3DXMATRIX result;
        memset(&result, 0, sizeof D3DXMATRIX);
        for (auto& f : m_Frames)
            result += f->CombinedTM / static_cast<float>(m_Frames.size());
        pBoxCollider->Update(result);
    }

    D3DVIEWPORT9 vp;
    g_pDevice->GetViewport(&vp);
    D3DXMATRIX view, proj;
    g_pDevice->GetTransform(D3DTS_VIEW, &view);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &proj);
    D3DXVECTOR3 v;
    for (unsigned int i = 0u; i < m_UITexts.size(); ++i)
    {
        D3DXVec3Project(&v, &GetTranslation(m_Frames[i]->CombinedTM), &vp, &proj, &view, NULL);
        m_UITexts[i]->SetPosition(v);
    }
}

void CharacterPart::OnRender()
{
    pBoxColliderRenderer->Render();
}

void CharacterPart::AddFrame(const string& name, MeshFilter* pMeshFilter)
{
    auto pFrame = pMeshFilter->FindFrame(name);
    if (!pFrame)
    {
        string str(name + "is null.");
        cout << str << '\n';
    }
    assert(pFrame && "CharacterPart::AddFrame() failed.");
    m_Frames.emplace_back(pFrame);

    auto pUIText = new UIText(g_pFontManager->GetFont(TAG_FONT::DEFAULT),
        D3DXVECTOR2(100.0f, 25.0f), string(pFrame->Name),
        D3DCOLOR_XRGB(0, 255, 255), nullptr);
    pUIText->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
    m_UITexts.emplace_back(pUIText);
}