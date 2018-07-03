#include "stdafx.h"
#include "CollisionTestBox.h"
#include "Collider.h"

SampleCollisionBox::SampleCollisionBox()
    : IObject()
    , pBC(nullptr)
    , v(Vector3::ZERO)
{
    pBC = AddComponent < BoxCollider>();
    D3DXMATRIX m;
    //....


    D3DXMatrixScaling(&m, 50.0f, 50.0f, 50.f);


    ///////////////////////////////////////////
    pBC->Init(m);
    pBC->SetTag(TAG_COLLISION::Player_0_Body);
    pBC->AddOnCollisionEnterCallback(std::bind(&SampleCollisionBox::OnCollisionEnter, this, _1, _2));
    pBC->AddOnCollisionStayCallback(std::bind(&SampleCollisionBox::OnCollisionStay, this, _1, _2));
    pBC->AddOnCollisionExitCallback(std::bind(&SampleCollisionBox::OnCollisionExit, this, _1, _2));
}

SampleCollisionBox::~SampleCollisionBox()
{
}

void SampleCollisionBox::OnUpdate()
{
    Transform* pTr = GetTransform();

    //... input
    float factor = 0.1f;
    if (Input()()->IsStayKeyDown(VK_LEFT))
    {
        v.x -= factor;
    }
    if (Input()()->IsStayKeyDown(VK_RIGHT))
    {
        v.x += factor;
    }
    if (Input()()->IsStayKeyDown(VK_UP))
    {
        v.y += factor;
    }
    if (Input()()->IsStayKeyDown(VK_DOWN))
    {
        v.y -= factor;
    }
    if (Input()()->IsStayKeyDown('B'))
    {
        v.z += factor;
    }
    if (Input()()->IsStayKeyDown('N'))
    {
        v.z -= factor;
    }

    pTr->SetPosition(v);

    ///////////////////////////////////////////
    pTr->Update();
    pBC->Update(pTr->GetTransformationMatrix());
}

void SampleCollisionBox::OnRender()
{
    pBC->Render();
}

void SampleCollisionBox::OnCollisionEnter(Collider * a, Collider * b)
{
    cout << "Enter\n";
}

void SampleCollisionBox::OnCollisionStay(Collider * a, Collider * b)
{
    Debug << "Stay\n";
}

void SampleCollisionBox::OnCollisionExit(Collider * a, Collider * b)
{
    cout << "Exit\n";
}
