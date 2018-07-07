#include "stdafx.h"
#include "CharacterPart.h"
#include "Collider.h"
#include "UIText.h"
#include "Character.h"
#include "CharacterAnimation.h"

CharacterPart::CharacterPart(const TAG_COLLIDER_CHARACTER_PART tag,
    Character* pCharacter)
    : IObject(TAG_OBJECT::Idle)
    , m_tagColliderCharacterPart(tag)

    , pCharacter(pCharacter)
{
    assert(pCharacter && 
        "CharacterPart::Constructor(), character is null.");

    CharacterAnimation* pAnimation = 
        pCharacter->GetCharacterAnimation();

    //pBoxCollider = AddComponent<BoxCollider>();
    //pBoxCollider->SetTagCollision(
    //    pCharacter->GetTagCollisionBody(pCharacter->GetIndex()));
    //pBoxCollider->AddOnCollisionEnterCallback(
    //    std::bind(&Character::OnCollisionEnter, pCharacter, _1, _2));
    //pBoxCollider->AddOnCollisionStayCallback(
    //    std::bind(&Character::OnCollisionStay, pCharacter, _1, _2));
    //pBoxCollider->AddOnCollisionExitCallback(
    //    std::bind(&Character::OnCollisionExit, pCharacter, _1, _2));

    m_boundingBoxes.resize(1);
    BoundingBox& m_boundingBox = m_boundingBoxes.front();

    switch (tag)
    {
    case TAG_COLLIDER_CHARACTER_PART::Head:
        {
            addFrame("hair_f_01", pAnimation);
            addFrame("hair_b_01", pAnimation);
            addFrame("head", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Neck);

            //auto pF = pSkiCon->FindFrame("F_Face_03");
            //auto pMeshContainer = 
            //    static_cast<MeshContainer*>(pF->pMeshContainer);
            //void* pData = nullptr;
            //auto pMesh = pMeshContainer->pEffectMesh->m_pMesh;
            //pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pData);
            //D3DXVECTOR3 center;
            //float radius;
            //D3DXComputeBoundingSphere((D3DXVECTOR3*)pData, pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), &center, &radius);
            //pMesh->UnlockVertexBuffer();
            //D3DXComputeBoundingBox((D3DXVECTOR3*)pData, 
            //    pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), 
            //    &min, &max);
            //pMesh->UnlockVertexBuffer();
            ////const float extentY = (max.y - min.y) * 0.25f;
            ////min.y = -extentY;
            ////max.y = extentY;
            //min.y *= 0.5f;
            //max.y *= 0.5f;

            //pBoxCollider->Init(min, max);
            //pBoxCollider->Init(Vector3::ONE * -10.0, Vector3::ONE * 10.0f);
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -10.0f, Vector3::ONE * 10.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Neck:
        {
            addFrame("head", pAnimation);
            addFrame("neck_01", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Breast);
            addChild(TAG_COLLIDER_CHARACTER_PART::Clavicle_Left);
            addChild(TAG_COLLIDER_CHARACTER_PART::Clavicle_Right);

            //pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -5.0f, Vector3::ONE * 5.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Breast:
        {
            addFrame("neck_01", pAnimation);
            addFrame("spine_03", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Stomach_Upper);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
            //    D3DXVECTOR3(10.0f, 10.0f, 10.0f));
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -10.0f, Vector3::ONE * 10.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Upper:
        {
            addFrame("spine_03", pAnimation);
            addFrame("spine_02", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Stomach_Lower);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
            //    D3DXVECTOR3(10.0f, 10.0f, 10.0f));
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -10.0f, Vector3::ONE * 10.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Stomach_Lower:
        {
            addFrame("spine_02", pAnimation);
            addFrame("spine_01", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Waist);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
            //    D3DXVECTOR3(10.0f, 10.0f, 10.0f));
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -10.0f, Vector3::ONE * 10.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Waist:
        {
            addFrame("pelvis", pAnimation);
            addFrame("spine_01", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper);
            addChild(TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), 
            //    D3DXVECTOR3(10.0f, 10.0f, 10.0f));
            m_boundingBox = BoundingBox::Create(Vector3::ONE * -10.0f, Vector3::ONE * 10.0f);
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Left:
        {
            addFrame("clavicle_l", pAnimation);
            addFrame("upperarm_l", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper);

            //pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f), 
            //    D3DXVECTOR3(6.0f, 7.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Clavicle_Right:
        {
            addFrame("clavicle_r", pAnimation);
            addFrame("upperarm_r", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper);

            //pBoxCollider->Init(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
            //    D3DXVECTOR3(6.0f, 7.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-6.0f, -7.0f, -5.0f),
                D3DXVECTOR3(6.0f, 7.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Upper:
        {
            addFrame("upperarm_l", pAnimation);
            addFrame("lowerarm_l", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(10.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-10.0f, -5.0f, -5.0f),
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Left_Lower:
        {
            addFrame("lowerarm_l", pAnimation);
            addFrame("hand_l", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Hand_Left);

            //pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(12.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Upper:
        {
            addFrame("upperarm_r", pAnimation);
            addFrame("lowerarm_r", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower);

            //pBoxCollider->Init(D3DXVECTOR3(-10.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(10.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-10.0f, -5.0f, -5.0f),
                D3DXVECTOR3(10.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Arm_Right_Lower:
        {
            addFrame("lowerarm_r", pAnimation);
            addFrame("hand_r", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Hand_Right);

            //pBoxCollider->Init(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
            //    D3DXVECTOR3(12.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-12.0f, -5.0f, -5.0f),
                D3DXVECTOR3(12.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Left:
        {
            addFrame("hand_l", pAnimation);
            addFrame("middle_01_l", pAnimation);

            //pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-5.0f, -5.0f, -5.0f),
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Hand_Right:
        {
            addFrame("hand_r", pAnimation);
            addFrame("middle_01_r", pAnimation);

            //pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-5.0f, -5.0f, -5.0f),
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Upper:
        {
            addFrame("thigh_l", pAnimation);
            addFrame("calf_l", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower);

            //pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
            //    D3DXVECTOR3(23.0f, 7.0f, 7.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-23.0f, -7.0f, -7.0f),
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Left_Lower:
        {
            addFrame("calf_l", pAnimation);
            addFrame("foot_l", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Foot_Left);

            //pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(23.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-23.0f, -5.0f, -5.0f),
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Upper:
        {
            addFrame("thigh_r", pAnimation);
            addFrame("calf_r", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower);

            //pBoxCollider->Init(D3DXVECTOR3(-23.0f, -7.0f, -7.0f), 
            //    D3DXVECTOR3(23.0f, 7.0f, 7.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-23.0f, -7.0f, -7.0f),
                D3DXVECTOR3(23.0f, 7.0f, 7.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Leg_Right_Lower:
        {
            addFrame("calf_r", pAnimation);
            addFrame("foot_r", pAnimation);

            addChild(TAG_COLLIDER_CHARACTER_PART::Foot_Right);

            //pBoxCollider->Init(D3DXVECTOR3(-23.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(23.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-23.0f, -5.0f, -5.0f),
                D3DXVECTOR3(23.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Left:
        {
            addFrame("foot_l", pAnimation);
            addFrame("ball_l", pAnimation);

            //pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-5.0f, -5.0f, -5.0f),
                D3DXVECTOR3(5.0f, 5.0f, 5.0f));
        }
        break;
    case TAG_COLLIDER_CHARACTER_PART::Foot_Right:
        {
            addFrame("foot_r", pAnimation);
            addFrame("ball_r", pAnimation);

            //pBoxCollider->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), 
            //    D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            m_boundingBox = BoundingBox::Create(D3DXVECTOR3(-5.0f, -5.0f, -5.0f),
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
    D3DXMATRIX model;
    memset(&model, 0, sizeof model);

    if (m_tagColliderCharacterPart == TAG_COLLIDER_CHARACTER_PART::Head)
    {
        auto topFront = Matrix::GetTranslation(
            m_frames[0]->CombinedTransformationMatrix);
        auto topBack = Matrix::GetTranslation(
            m_frames[1]->CombinedTransformationMatrix);
        auto top = (topFront + topBack) * 0.5f;
        auto bottom = Matrix::GetTranslation(
            m_frames[2]->CombinedTransformationMatrix);
        auto center = (top + bottom) * 0.5f;
        model = m_frames[2]->CombinedTransformationMatrix;
        model._41 = center.x - 9.0f;
        model._42 = center.y;
        model._43 = center.z;
    }
    else
    {
        for (auto& f : m_frames)
            model += f->CombinedTransformationMatrix
                   / static_cast<float>(m_frames.size());
    }

    D3DXMATRIX world 
        = model 
        * pCharacter->GetTransform()->GetTransformationMatrix();
    D3DXQUATERNION r;
    Matrix::GetScaleAndRotation(world, nullptr, &r);
    D3DXVECTOR3 t = Matrix::GetTranslation(world);
    BoundingBox& m_boundingBox = m_boundingBoxes.front();
    m_boundingBox.rotation = r;
    m_boundingBox.position = t;

    //updateUI();
}

void CharacterPart::OnRender()
{
    m_boundingBoxes.front().Render();
}

TAG_COLLIDER_CHARACTER_PART CharacterPart::GetTagColliderCharacterPart() const
{
    return m_tagColliderCharacterPart;
}

void CharacterPart::addFrame(
    const string& name, CharacterAnimation* pCharacterAnimation)
{
    Frame* pFrame = pCharacterAnimation->FindFrame(name);
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

void CharacterPart::addChild(const TAG_COLLIDER_CHARACTER_PART tag)
{
    pCharacter->AddPart(new CharacterPart(tag, pCharacter));
}
