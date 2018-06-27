#include "stdafx.h"
#include "CharacterPart.h"
#include "Collider.h"
#include "UIText.h"
#include "Character.h"
#include "SkinnedMeshController.h"

CharacterPart::CharacterPart(const TAG_COLLIDER_CHARACTER_PART tag,
    Character* pCharacter)
    : IObject()
    , m_tagColliderCharacterPart(tag)

    , pCharacter(pCharacter)
    , pBoxCollider(nullptr)
{
    assert(pCharacter && 
        "CharacterPart::Constructor() failed. character is null.");

    auto pSkiCon = pCharacter->GetComponent<SkinnedMeshController>();

    pBoxCollider = AddComponent<BoxCollider>();
    pBoxCollider->SetTag(
        pCharacter->GetTagCollisionBody(pCharacter->GetIndex()));

    pBoxCollider->AddOnCollisionEnterCallback(
        bind(&Character::OnCollisionEnter, pCharacter, _1, _2));
    pBoxCollider->AddOnCollisionStayCallback(
        bind(&Character::OnCollisionStay, pCharacter, _1, _2));
    pBoxCollider->AddOnCollisionExitCallback(
        bind(&Character::OnCollisionExit, pCharacter, _1, _2));

    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::HEAD:
        {
            addFrame("hair_f_01", pSkiCon);
            addFrame("hair_b_01", pSkiCon);
            addFrame("head", pSkiCon);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::NECK, 
                pCharacter));

            auto pF = pSkiCon->FindFrame("F_Face_03");
            auto pMeshContainer = 
                static_cast<MeshContainer*>(pF->pMeshContainer);
            void* pData = nullptr;
            auto pMesh = pMeshContainer->pEffectMesh->m_pMesh;
            pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pData);
            D3DXVECTOR3 min, max;
            D3DXComputeBoundingBox((D3DXVECTOR3*)pData, 
                pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), 
                &min, &max);
            pMesh->UnlockVertexBuffer();
            min.y *= 0.5f;
            max.y *= 0.5f;
            pBoxCollider->Init(min, max);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::NECK:
        {
            addFrame("head", pSkiCon);
            addFrame("neck_01", pSkiCon);

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
            addFrame("neck_01", pSkiCon);
            addFrame("spine_03", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_UPPER:
        {
            addFrame("spine_03", pSkiCon);
            addFrame("spine_02", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::STOMACH_LOWER:
        {
            addFrame("spine_02", pSkiCon);
            addFrame("spine_01", pSkiCon);

            AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::WAIST,
                pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
                D3DXVECTOR3(10.0f, 10.0f, 10.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::WAIST:
        {
            addFrame("pelvis", pSkiCon);
            addFrame("spine_01", pSkiCon);

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
            addFrame("clavicle_l", pSkiCon);
            addFrame("upperarm_l", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f), 
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::CLAVICLE_RIGHT:
        {
            addFrame("clavicle_r", pSkiCon);
            addFrame("upperarm_r", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_UPPER:
        {
            addFrame("upperarm_l", pSkiCon);
            addFrame("lowerarm_l", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_LEFT_LOWER:
        {
            addFrame("lowerarm_l", pSkiCon);
            addFrame("hand_l", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_LEFT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_UPPER:
        {
            addFrame("upperarm_r", pSkiCon);
            addFrame("lowerarm_r", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::ARM_RIGHT_LOWER:
        {
            addFrame("lowerarm_r", pSkiCon);
            addFrame("hand_r", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_LEFT:
        {
            addFrame("hand_l", pSkiCon);
            addFrame("middle_01_l", pSkiCon);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::HAND_RIGHT:
        {
            addFrame("hand_r", pSkiCon);
            addFrame("middle_01_r", pSkiCon);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_UPPER:
        {
            addFrame("thigh_l", pSkiCon);
            addFrame("calf_l", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_LEFT_LOWER:
        {
            addFrame("calf_l", pSkiCon);
            addFrame("foot_l", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_UPPER:
        {
            addFrame("thigh_r", pSkiCon);
            addFrame("calf_r", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::LEG_RIGHT_LOWER:
        {
            addFrame("calf_r", pSkiCon);
            addFrame("foot_r", pSkiCon);

            AddChildren(new CharacterPart(
                TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT, pCharacter));

            pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_LEFT:
        {
            addFrame("foot_l", pSkiCon);
            addFrame("ball_l", pSkiCon);

            pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::FOOT_RIGHT:
        {
            addFrame("foot_r", pSkiCon);
            addFrame("ball_r", pSkiCon);

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

    assert(m_frames.front() && 
        "CharacterPart::Constructor() failed. frame is null.");
    D3DXVECTOR3 v(
        Matrix::GetTranslation(m_frames.front()->TransformationMatrix));
    for (auto& f : m_frames)
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

    if (m_tagColliderCharacterPart == TAG_COLLIDER_CHARACTER_PART::HEAD)
    {
        auto topFront = Matrix::GetTranslation(
            m_frames[0]->CombinedTransformationMatrix);
        auto topBack = Matrix::GetTranslation(
            m_frames[1]->CombinedTransformationMatrix);
        auto top = (topFront + topBack) * 0.5f;
        auto bottom = Matrix::GetTranslation(
            m_frames[2]->CombinedTransformationMatrix);
        auto center = (top + bottom) * 0.5f;
        world = m_frames[2]->CombinedTransformationMatrix;
        world._41 = center.x;
        world._42 = center.y;
        world._43 = center.z;
    }
    else
    {
        for (auto& f : m_frames)
            world += f->CombinedTransformationMatrix
                   / static_cast<float>(m_frames.size());
    }

    pBoxCollider->Update(world * 
        pCharacter->GetTransform()->GetTransformationMatrix());

    //updateUI();
}

void CharacterPart::OnRender()
{
    pBoxCollider->Render();
}

TAG_COLLIDER_CHARACTER_PART CharacterPart::GetTagColliderCharacterPart() const
{
    return m_tagColliderCharacterPart;
}

void CharacterPart::addFrame(
    const string& name, SkinnedMeshController* pSkiCon)
{
    auto pFrame = pSkiCon->FindFrame(name);
    if (!pFrame)
    {
        string str(name + "is null.");
        cout << str << '\n';
    }
    assert(pFrame && "CharacterPart::AddFrame(), frame is null.");
    m_frames.emplace_back(pFrame);

    //auto pUIText = new UIText(Resource()()->GetFont(TAG_FONT::DEFAULT),
    //    D3DXVECTOR2(100.0f, 25.0f), string(pFrame->Name),
    //    D3DCOLOR_XRGB(0, 255, 255), nullptr);
    //pUIText->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
    //m_UITexts.emplace_back(pUIText);
}

void CharacterPart::updateUI()
{
    auto pD = Device()();
    D3DVIEWPORT9 vp;
    pD->GetViewport(&vp);
    D3DXMATRIX view, proj;
    pD->GetTransform(D3DTS_VIEW, &view);
    pD->GetTransform(D3DTS_PROJECTION, &proj);
    D3DXVECTOR3 v;
    for (size_t i = 0u; i < m_UITexts.size(); ++i)
    {
        D3DXVec3Project(&v, &(Matrix::GetTranslation(
            m_frames[i]->CombinedTransformationMatrix)),
            &vp, &proj, &view, 
            &pCharacter->GetTransform()->GetTransformationMatrix());
        m_UITexts[i]->SetPosition(v);
    }
}