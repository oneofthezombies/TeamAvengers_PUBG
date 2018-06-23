#include "stdafx.h"
#include "CharacterPart.h"
#include "Collider.h"
#include "Renderer.h"
#include "MeshFilterAndAnimator.h"
#include "UIText.h"
#include "Character.h"
#include "CharacterCollisionListener.h"

CharacterPart::CharacterPart(const TAG_COLLIDER_CHARACTER_PART tag,
    Character* pCharacter)
    : IObject()

    , m_Tag(tag)
    
    , pCharacter(pCharacter)
    , pBoxCollider(nullptr)
    , pBoxColliderRenderer(nullptr)
{
    assert(pCharacter && 
        "CharacterPart::Constructor() failed. character is null.");

    auto pCollisionListener = 
        pCharacter->GetComponent<CharacterCollisionListener>();
    
    auto pMeshFilter = pCharacter->GetComponent<MeshFilter>();

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(TAG_COLLISION::BODY_OF_PLAYER_1);
    pBoxCollider->SetListener(pCollisionListener);

    pBoxColliderRenderer = AddComponent<BoxColliderRenderer>();

    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::HEAD:
        {
            addFrame("hair_f_01", pMeshFilter);
            addFrame("hair_b_01", pMeshFilter);
            addFrame("head", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::NECK, 
                pCharacter));

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
            addFrame("head", pMeshFilter);
            addFrame("neck_01", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::BREAST,
                pCharacter));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::CLAVICLE_LEFT, pCharacter));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::BREAST:
        {
            addFrame("neck_01", pMeshFilter);
            addFrame("spine_03", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER:
        {
            addFrame("spine_03", pMeshFilter);
            addFrame("spine_02", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER:
        {
            addFrame("spine_02", pMeshFilter);
            addFrame("spine_01", pMeshFilter);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::WAIST,
                pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::WAIST:
        {
            addFrame("pelvis", pMeshFilter);
            addFrame("spine_01", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER, pCharacter));
            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_LEFT:
        {
            addFrame("clavicle_l", pMeshFilter);
            addFrame("upperarm_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f), 
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT:
        {
            addFrame("clavicle_r", pMeshFilter);
            addFrame("upperarm_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER:
        {
            addFrame("upperarm_l", pMeshFilter);
            addFrame("lowerarm_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER:
        {
            addFrame("lowerarm_l", pMeshFilter);
            addFrame("hand_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_LEFT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER:
        {
            addFrame("upperarm_r", pMeshFilter);
            addFrame("lowerarm_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER:
        {
            addFrame("lowerarm_r", pMeshFilter);
            addFrame("hand_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_LEFT:
        {
            addFrame("hand_l", pMeshFilter);
            addFrame("middle_01_l", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT:
        {
            addFrame("hand_r", pMeshFilter);
            addFrame("middle_01_r", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER:
        {
            addFrame("thigh_l", pMeshFilter);
            addFrame("calf_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER:
        {
            addFrame("calf_l", pMeshFilter);
            addFrame("foot_l", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER:
        {
            addFrame("thigh_r", pMeshFilter);
            addFrame("calf_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER:
        {
            addFrame("calf_r", pMeshFilter);
            addFrame("foot_r", pMeshFilter);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT:
        {
            addFrame("foot_l", pMeshFilter);
            addFrame("ball_l", pMeshFilter);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT:
        {
            addFrame("foot_r", pMeshFilter);
            addFrame("ball_r", pMeshFilter);

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

    assert(m_Frames.front() && 
        "CharacterPart::Constructor() failed. frame is null.");
    D3DXVECTOR3 v(
        Matrix::GetTranslation(m_Frames.front()->TransformationMatrix));
    for (auto& f : m_Frames)
    {
        assert(f && "CharacterPart::Constructor() failed. frame is null.");
        v = (v + Matrix::GetTranslation(f->TransformationMatrix)) * 0.5f;
    }
}

CharacterPart::~CharacterPart()
{
}

void CharacterPart::OnUpdate()
{
    D3DXMATRIX world;
    memset(&world, 0, sizeof world);

    if (m_Tag == TAG_COLLIDER_CHARACTER_PART::HEAD)
    {
        auto topFront = Matrix::GetTranslation(m_Frames[0]->CombinedTM);
        auto topBack = Matrix::GetTranslation(m_Frames[1]->CombinedTM);
        auto top = (topFront + topBack) * 0.5f;
        auto bottom = Matrix::GetTranslation(m_Frames[2]->CombinedTM);
        auto center = (top + bottom) * 0.5f;
        world = m_Frames[2]->CombinedTM;
        world._41 = center.x;
        world._42 = center.y;
        world._43 = center.z;

    }
    else
    {
        for (auto& f : m_Frames)
            world += f->CombinedTM / static_cast<float>(m_Frames.size());
    }
    pBoxCollider->Update(world * 
        pCharacter->GetTransform()->GetTransformationMatrix());

    updateUI();
}

void CharacterPart::OnRender()
{
    pBoxColliderRenderer->Render();
}

TAG_COLLIDER_CHARACTER_PART CharacterPart::GetTagColliderCharacterPart() const
{
    return m_Tag;
}

void CharacterPart::addFrame(const string& name, MeshFilter* pMeshFilter)
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

void CharacterPart::updateUI()
{
    D3DVIEWPORT9 vp;
    g_pDevice->GetViewport(&vp);
    D3DXMATRIX view, proj;
    g_pDevice->GetTransform(D3DTS_VIEW, &view);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &proj);
    D3DXVECTOR3 v;

    for (unsigned int i = 0u; i < m_UITexts.size(); ++i)
    {
        D3DXVec3Project(&v, &(Matrix::GetTranslation(m_Frames[i]->CombinedTM)),
            &vp, &proj, &view, 
            &pCharacter->GetTransform()->GetTransformationMatrix());
        m_UITexts[i]->SetPosition(v);
    }
}
