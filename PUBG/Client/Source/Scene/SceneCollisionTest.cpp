#include "stdafx.h"
#include "SceneCollisionTest.h"
#include "Collider.h"
#include "Grid.h"

SampleCollisionBox::SampleCollisionBox(
    const int instanceID,
    const TAG_COLLISION tagCollision, const D3DXMATRIX& transformationMatrix)
    : IObject(TAG_OBJECT::Idle)
    , m_instanceID(instanceID)
    , pTransform(nullptr)
    , pBoxCollider(nullptr)
{
    pTransform = GetTransform();
    pBoxCollider = AddComponent<BoxCollider>();

    pBoxCollider->Init(transformationMatrix);
    pBoxCollider->SetTagCollision(tagCollision);
    pBoxCollider->AddOnCollisionEnterCallback(
        std::bind(&SampleCollisionBox::OnCollisionEnter, this, _1, _2));
    pBoxCollider->AddOnCollisionStayCallback(
        std::bind(&SampleCollisionBox::OnCollisionStay, this, _1, _2));
    pBoxCollider->AddOnCollisionExitCallback(
        std::bind(&SampleCollisionBox::OnCollisionExit, this, _1, _2));
}

SampleCollisionBox::~SampleCollisionBox()
{
}

void SampleCollisionBox::OnUpdate()
{
    Debug << "Instance ID : " << m_instanceID << '\n'
          << "Position : " << pTransform->GetPosition() << '\n';
}

void SampleCollisionBox::OnRender()
{
    pBoxCollider->Render();
}

void SampleCollisionBox::OnCollisionEnter(Collider* o, Collider* d)
{
    cout << "Enter\n";
}

void SampleCollisionBox::OnCollisionStay(Collider* o, Collider* d)
{
    Debug << "Stay\n";
}

void SampleCollisionBox::OnCollisionExit(Collider* o, Collider* d)
{
    cout << "Exit\n";
}

SampleControllableCollisionBox::SampleControllableCollisionBox(
    const int instanceID,
    const TAG_COLLISION tagCollision,
    const D3DXMATRIX& transformationMatrix)
    : SampleCollisionBox(instanceID, tagCollision, transformationMatrix)
{
    D3DXVECTOR3 p = pTransform->GetPosition();
    p.x = 5.0f;
    pTransform->SetPosition(p);
}

SampleControllableCollisionBox::~SampleControllableCollisionBox()
{
}

void SampleControllableCollisionBox::OnUpdate()
{
    D3DXVECTOR3 t = pTransform->GetPosition();
    D3DXQUATERNION r = pTransform->GetRotation();
    D3DXQUATERNION tempR;
    D3DXQuaternionIdentity(&tempR);

    float factor = 1.0f;
    if (Input()()->IsStayKeyDown(VK_LEFT))
    {
        t.x -= factor;
    }
    if (Input()()->IsStayKeyDown(VK_RIGHT))
    {
        t.x += factor;
    }
    if (Input()()->IsStayKeyDown(VK_UP))
    {
        t.y += factor;
    }
    if (Input()()->IsStayKeyDown(VK_DOWN))
    {
        t.y -= factor;
    }
    if (Input()()->IsStayKeyDown('B'))
    {
        t.z += factor;
    }
    if (Input()()->IsStayKeyDown('N'))
    {
        t.z -= factor;
    }
    if (Input()()->IsStayKeyDown('1'))
    {
        D3DXQuaternionRotationAxis(&tempR, &Vector3::UP, 0.1f);
    }
    if (Input()()->IsStayKeyDown('2'))
    {
        D3DXQuaternionRotationAxis(&tempR, &Vector3::UP, -0.1f);
    }

    pTransform->SetPosition(t);
    pTransform->SetRotation(r * tempR);
    pTransform->Update();
    pBoxCollider->Update(pTransform->GetTransformationMatrix());

    SampleCollisionBox::OnUpdate();
}


SceneCollisionTest::SceneCollisionTest()
    : IScene()
{
}

SceneCollisionTest::~SceneCollisionTest()
{
}

void SceneCollisionTest::OnInit()
{
    AddObject(new Grid);
    AddObject(
        new SampleCollisionBox(
            0, 
            TAG_COLLISION::Player_0_Body, 
            D3DXMATRIX(100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f, 0.0f, 0.0f, -0.000015f, 0.0f, 1.0f)));
    AddObject(
        new SampleControllableCollisionBox(
            1, 
            TAG_COLLISION::Player_1_Body, 
            Matrix::IDENTITY * 200.0f));

    Collision()()->AddCollisionEvent(
        TAG_COLLISION::Player_1_Body, 
        TAG_COLLISION::Player_0_Body);
}

void SceneCollisionTest::OnUpdate()
{
    Debug << "Press P : on Camera Free\n";
}
