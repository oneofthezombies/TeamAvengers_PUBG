#include "stdafx.h"
#include "ICamera.h"
#include "ComponentTransform.h"
#include "Character.h"
#include "Ray.h"
#include "HeightMap.h"

const float ICamera::VISUAL_RANGE = 100000.0f;

Character::Info* ICamera::GetTargetInfo()//(전)TargetTransform* GetTarget()
{
    return Camera()()->GetTargetInfo();
}

ICamera::ICamera(const TAG_CAMERA tag)
    : MemoryAllocator()
    , m_tagCamera(tag)
    , m_eye(Vector3::ZERO)
{
    m_look = m_eye + D3DXVECTOR3(0, 1, 0);
    //지금 각 카메라 마다 8개의 projection과 world, 6개 plane 을 갖고 있다. 하나만 만들어서 나누어 쓰도록 하자!
    
    m_vecProj[0]=(D3DXVECTOR3(-1, 1, 1));	//좌상후
    m_vecProj[1]=(D3DXVECTOR3(1, 1, 1));	//우상후
    m_vecProj[2]=(D3DXVECTOR3(-1, 1, 0));	//좌상전
    m_vecProj[3]=(D3DXVECTOR3(1, 1, 0));	//우상전
    m_vecProj[4]=(D3DXVECTOR3(-1, -1, 1));	//좌하후
    m_vecProj[5]=(D3DXVECTOR3(1, -1, 1));	//우하후
    m_vecProj[6]=(D3DXVECTOR3(-1, -1, 0));	//좌하전 
    m_vecProj[7]=(D3DXVECTOR3(1, -1, 0));	//우하전
}

ICamera::~ICamera()
{

}

void ICamera::CameraRender()
{
    //Character::Info* pTarInfo = GetTargetInfo();
    //if (pTarInfo)
    //{    
    //    D3DXMATRIX tarR, matWorld;
    //    D3DXVECTOR3 vRot = *pTarInfo->pRotationForCamera;
    //    D3DXMatrixRotationYawPitchRoll(&tarR, vRot.y, vRot.x, vRot.z);
    //    
    //    D3DXMATRIX testT;
    //    D3DXMatrixTranslation(&testT, TP_BASEPOSX, TP_BASEPOSY, TP_DISTANCE);
    //    testT *=pTarInfo->pTransform->GetTransformationMatrix();
    //    //              (model space)                      (rotation get from character) 
    //    matWorld = pTarInfo->pTPP->CombinedTransformationMatrix    *    tarR    *      testT;
    //}

    drawIndices(FRUSTUM_INDICES, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    
    //if (temp)
    //    draw(drawRay, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}
void ICamera::draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color)
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [this, &color](LPD3DXEFFECT pEffect)
    {
        D3DXMATRIX mat;
        D3DXMatrixIdentity(&mat);
        pEffect->SetMatrix(Shader::World, &mat);
        pEffect->SetValue("Color", &color, sizeof color);
    },
        [this, &vertices]()
    {
        Device()()->DrawPrimitiveUP(
            D3DPT_LINELIST,
            static_cast<UINT>(vertices.size() / 2),
            vertices.data(),
            sizeof vertices.front());
    });
}
void ICamera::drawIndices(const vector<WORD>& indices, const D3DXCOLOR & color)
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [this, &color](LPD3DXEFFECT pEffect)
    {
        D3DXMATRIX mat;
        D3DXMatrixIdentity(&mat);
        pEffect->SetMatrix(Shader::World, &mat);
        pEffect->SetValue("Color", &color, sizeof color);
    },
        [this,&indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            sizeof m_vecWorld / sizeof m_vecWorld[0],
            static_cast<UINT>(indices.size() / 2),
            indices.data(),
            D3DFMT_INDEX16,
            &m_vecWorld[0],
            sizeof D3DXVECTOR3);
    });

}


void ICamera::UpdateFrustumCulling()
{
    D3DXMATRIX InvVP;
    InvVP = m_viewMatrix * m_projectionMatrix;
    D3DXMatrixInverse(&InvVP, NULL, &InvVP);

    for (int i = 0; i < 8; ++i)
    {
        D3DXVec3TransformCoord(&m_vecWorld[i], &m_vecProj[i], &InvVP);
        //D3DXVec3Unproject(&m_vecWorld[i], &m_vecProj[i], NULL,  &m_projectionMatrix, &m_viewMatrix, NULL);
    }

    //근평면//좌상전//우상전//좌하전
    D3DXPlaneFromPoints(&m_vecPlane[0], &m_vecWorld[2], &m_vecWorld[3], &m_vecWorld[6]);
    //원평면//우상후//좌상후//우하후
    D3DXPlaneFromPoints(&m_vecPlane[1], &m_vecWorld[1], &m_vecWorld[0], &m_vecWorld[5]);
    //좌평면//좌상후//좌상전//좌하후
    D3DXPlaneFromPoints(&m_vecPlane[2], &m_vecWorld[0], &m_vecWorld[2], &m_vecWorld[4]);
    //우평면//우상전//우상후//우하전
    D3DXPlaneFromPoints(&m_vecPlane[3], &m_vecWorld[3], &m_vecWorld[1], &m_vecWorld[7]);
    //상평면//좌상후//우상후//좌상전
    D3DXPlaneFromPoints(&m_vecPlane[4], &m_vecWorld[0], &m_vecWorld[1], &m_vecWorld[2]);
    //하평면//좌하전//우하전//좌하후
    D3DXPlaneFromPoints(&m_vecPlane[5], &m_vecWorld[6], &m_vecWorld[7], &m_vecWorld[4]);
    
}

bool ICamera::IsObjectInsideFrustum(const D3DXVECTOR3 center, const float radius)
{
    for (int i = 0; i < 6; i++)
    {
        if (D3DXPlaneDotCoord(&m_vecPlane[i], &center) > radius)
        {
            return false;
        }
    }
    return true;
}



const D3DXMATRIX& ICamera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const D3DXMATRIX& ICamera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

TAG_CAMERA ICamera::GetTagCamera() const
{
    return m_tagCamera;
}

const D3DXVECTOR3& ICamera::GetPosition() const
{
    return m_eye;
}

D3DXVECTOR3 ICamera::GetDirection()
{
    return m_look - m_eye;
}

D3DXVECTOR4 ICamera::GetFrustumArea()
{
    float minX = FLT_MAX; float minZ = FLT_MAX;
    float maxX = FLT_MIN; float maxZ = FLT_MIN;
    for (int i = 0; i < 8; i++)
    {
        if (m_vecWorld[i].x < minX)
            minX = m_vecWorld[i].x;
        if (m_vecWorld[i].z < minZ)
            minZ = m_vecWorld[i].z;
        if (m_vecWorld[i].x > maxX)
            maxX = m_vecWorld[i].x;
        if (m_vecWorld[i].z > maxZ)
            maxZ = m_vecWorld[i].z;
    }
    return D3DXVECTOR4(minX, minZ, maxX, maxZ);
}

bool ICamera::CalcPickedPosition(OUT D3DXVECTOR3 * vOut, WORD screenX, WORD screenY)
{
    Ray ray = Ray::RayAtWorldSpace(screenX, screenY);
    float intersectionDist;
    bool bIntersect = false;

    vector<D3DXVECTOR3>& rayBox = CurrentScene()()->GetHeightMap()->GetBoundaryBox(); 
    
    
    for (size_t i = 0u; i < rayBox.size(); i += 3)
    {
        if (ray.CalcIntersectTri(&rayBox[i], &intersectionDist))
        {
            bIntersect = true;
            *vOut = ray.m_pos + ray.m_dir * intersectionDist;


            ////총쏘면 나오는 빨간색 ray를 rendering 을 위해
            //temp = true;
            //drawRay.push_back(ray.m_pos);
            //drawRay.push_back(*vOut);
            ////--------------------------

            return bIntersect;
        }
    }

    return bIntersect;
}

Ray ICamera::PickedRayDistancePosition(OUT D3DXVECTOR3 * vOut, OUT float* distance, WORD screenX, WORD screenY)
{
    Ray ray = Ray::RayAtWorldSpace(screenX, screenY);

    vector<D3DXVECTOR3>& rayBox = CurrentScene()()->GetHeightMap()->GetBoundaryBox(); 

    for (size_t i = 0u; i < rayBox.size(); i += 3)
    {
        if (ray.CalcIntersectTri(&rayBox[i], distance))
        {
            *vOut = ray.m_pos + ray.m_dir * (*distance);
        }
    }
    return ray;
}





//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//---------------------------실제 카메라들--------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CameraFree::CameraFree()
    : ICamera(TAG_CAMERA::Default)
    , m_isOtho(false)
    , m_position(Vector3::ZERO)
    , m_rotation(Vector3::ZERO)
{
    //m_position = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
    //D3DXQuaternionRotationYawPitchRoll(&m_quarernion,0.0f,0.0f,0.0f);
}

CameraFree::~CameraFree()
{
}

void CameraFree::Reset()
{
    m_rotation = Vector3::ZERO;
    m_fovY = D3DX_PI * 0.5f;
    m_position = D3DXVECTOR3(0.0f, 160.0f, -350.0f);
}

void CameraFree::Update()
{
    Debug << endl;
    Debug << "  우로R(8)  좌로R(9)   " << endl;
    Debug << "     앞(U)상(I)뒤(O)    " << endl;
    Debug << " 좌(J)    하(K)    우(L)" << endl;
    InputManager* pInput = Input()();
    //const float factor = 2.f;
    const float factor = 50.f;
    if (pInput->IsStayKeyDown('K')) { m_position.y -= factor; }
    if (pInput->IsStayKeyDown('I')) { m_position.y += factor; }
    if (pInput->IsStayKeyDown('J')) { m_position.x -= factor; }
    if (pInput->IsStayKeyDown('L')) { m_position.x += factor; }
    if (pInput->IsStayKeyDown('O')) { m_position.z -= factor; }
    if (pInput->IsStayKeyDown('U')) { m_position.z += factor; }
    if (pInput->IsStayKeyDown('9')) { m_rotation.y -= factor * 0.01f; }
    if (pInput->IsStayKeyDown('8')) { m_rotation.y += factor * 0.01f; }
    //if (pInput->IsStayKeyDown('7')) { m_rotation.x -= factor * 0.01f; }
    //if (pInput->IsStayKeyDown('0')) { m_rotation.x += factor * 0.01f; }

    if (pInput->IsOnceKeyDown('6')) { m_isOtho = !m_isOtho; }

    if (!m_isOtho)
    {
        D3DXVECTOR3 eye = Vector3::ZERO;
        D3DXVECTOR3 look = eye + Vector3::FORWARD;

        D3DXMATRIX matR,matT , world, view, proj;
        D3DXMatrixRotationY(&matR, m_rotation.y);
        //D3DXMatrixRotationYawPitchRoll(&matR, m_rotation.y, m_rotation.x, 0.0f);
        D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
        world =  matT * matR ;
        D3DXVec3TransformCoord(&eye, &eye, &world);
        D3DXVec3TransformCoord(&look, &look, &world);

        D3DXMatrixLookAtLH(&view, &eye, &look, &Vector3::UP);
        SetViewMatrix(&view);
        auto pD = Device()();
        pD->SetTransform(D3DTS_VIEW, &GetViewMatrix());

        RECT rc;
        GetClientRect(g_hWnd, &rc);

        D3DXMatrixPerspectiveFovLH(&proj,
            m_fovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom),
            1.0f, ICamera::VISUAL_RANGE);
        SetProjectionMatrix(&proj);
        pD->SetTransform(D3DTS_PROJECTION, &GetProjectionMatrix());
    }
    else
    {
        D3DXVECTOR3 eye = D3DXVECTOR3(12800.0f, 6000.0f, 12800.0f);
        D3DXVECTOR3 look = eye + Vector3::DOWN;
        D3DXMATRIX view, proj;
        D3DXMatrixLookAtLH(&view, &eye, &look, &Vector3::FORWARD);
        SetViewMatrix(&view);
        auto pD = Device()();
        pD->SetTransform(D3DTS_VIEW, &GetViewMatrix());
        D3DXMatrixOrthoLH(&proj, 25600.0f, 25600.0f, 1.0f, ICamera::VISUAL_RANGE);
        SetProjectionMatrix(&proj);
        pD->SetTransform(D3DTS_PROJECTION, &GetProjectionMatrix());
    }

}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
CameraThirdPerson::CameraThirdPerson(const TAG_CAMERA tag)
    : ICamera(tag)
    , m_Position(Vector3::ZERO/*D3DXVECTOR3(-20.0f, 180.0f, 85.0f)*/)
{
}

CameraThirdPerson::~CameraThirdPerson()
{
}

void CameraThirdPerson::Reset()
{
    //m_position = D3DXVECTOR3(TP_BASEPOSX, TP_BASEPOSY, TP_DISTANCE);
    //m_Position = D3DXVECTOR3(-20.0f, 180.0f, 85.0f);
    m_Position = D3DXVECTOR3(-20.0f, 180.0f, 250.0f);
    //80 Degrees TP sight
    m_fovY = D3DX_PI * (80.0f / 180.0f);
    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + (D3DXVECTOR3(0, 1, 0));


}

void CameraThirdPerson::Update()
{
    //견착하는 부분은 3인칭에서만 있기에
    if (Input()()->IsStayKeyDown(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);


    Character::Info* pTarInfo = GetTargetInfo();
    if (!pTarInfo)
    {
        D3DXMatrixIdentity(&m_worldMatrix);

    }
    else
    {
        D3DXMATRIX tarR, baseT;
        D3DXVECTOR3 vRot = *pTarInfo->pRotationForCamera;
        D3DXMatrixRotationYawPitchRoll(&tarR, vRot.y, vRot.x, 0.0f);

        D3DXMATRIX baseY, baseZ;
        D3DXMatrixTranslation(&baseY, m_Position.x, m_Position.y, 0.0f);
        D3DXMatrixTranslation(&baseZ, 0.0f, 0.0f, m_Position.z);
        m_worldMatrix
            = baseZ
            * tarR
            * baseY
            * pTarInfo->pTransform->GetTransformationMatrix();
    }

    m_eye = Vector3::ZERO;
    m_look = m_eye - Vector3::FORWARD;
}

void CameraThirdPerson::Render()
{
    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        Resource()()->GetBoundingSphereMesh(),
        0,
        [this](LPD3DXEFFECT pEffect)
    {
        D3DXMATRIX s, m;
        D3DXMatrixScaling(&s, 10.0f, 10.0f, 10.0f);
        m = s * m_worldMatrix;
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR red(1.0f, 0.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &red, sizeof red);
    });
}

void ICamera::UpdateViewProjMatrix()
{

    D3DXVec3TransformCoord(&m_eye, &m_eye, &m_worldMatrix);
    D3DXVec3TransformCoord(&m_look, &m_look, &m_worldMatrix);
    D3DXMatrixLookAtLH(&m_viewMatrix, &m_eye, &m_look, &Vector3::UP);

    auto pD = Device()();
    pD->SetTransform(D3DTS_VIEW, &m_viewMatrix);
    if (m_tagCamera != TAG_CAMERA::Scope2X)
    {
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        D3DXMatrixPerspectiveFovLH(&m_projectionMatrix,
            m_fovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom),
            1.0f, ICamera::VISUAL_RANGE);
        pD->SetTransform(D3DTS_PROJECTION, &m_projectionMatrix);
    }

}

//-----------------------------------------------------------------------
CameraKyunChak::CameraKyunChak()
    : CameraThirdPerson(TAG_CAMERA::KyunChak)
    , m_vel(0.0f)
    , m_KyunChakPos(Vector3::ZERO)
    , m_isCloser(false)
    , m_val(0.0f)
{

}

CameraKyunChak::~CameraKyunChak()
{
}

void CameraKyunChak::Reset()
{
    CameraThirdPerson::Reset();
    m_KyunChakPos = D3DXVECTOR3(-43.0f, 158.0f, 35.0f);
    m_val = 0.0f;
    m_durTime = 0.0f;
}

void CameraKyunChak::Update()
{
    InputManager*  pInput = Input()();

    //if (pInput->IsStayKeyDown('K')) { m_KyunChakPos.y -= 1.0f; }
    //if (pInput->IsStayKeyDown('I')) { m_KyunChakPos.y += 1.0f; }
    //if (pInput->IsStayKeyDown('J')) { m_KyunChakPos.x -= 1.0f; }
    //if (pInput->IsStayKeyDown('L')) { m_KyunChakPos.x += 1.0f; }
    //if (pInput->IsStayKeyDown('O')) { m_KyunChakPos.z -= 1.0f; }
    //if (pInput->IsStayKeyDown('U')) { m_KyunChakPos.z += 1.0f; }

    //Debug << "여기야 : " << m_KyunChakPos << endl;


    m_isCloser = pInput->IsStayKeyDown(VK_RBUTTON);

    const float endTime = 1.0f;


    D3DXVECTOR3 now;
    if (m_isCloser)
    {
        m_val += pow(Time()()->GetDeltaTime(), 2.0f);
        m_durTime += m_val;
    }
    else
    {
        m_val -= pow(Time()()->GetDeltaTime(), 2.0f);
        m_durTime -= m_val;
    }
    if (m_durTime >= 1.0f)
        m_durTime = 1.0f;

    D3DXVec3Lerp(&now, &m_Position, &m_KyunChakPos, m_durTime / endTime);

    Character::Info* pTarInfo = GetTargetInfo();
    if (!pTarInfo)
    {
        D3DXMatrixIdentity(&m_worldMatrix);
    }
    else
    {
        D3DXMATRIX tarR, baseT;
        D3DXVECTOR3 vRot = *pTarInfo->pRotationForCamera;
        D3DXMatrixRotationYawPitchRoll(&tarR, vRot.y, vRot.x, 0.0f);

        D3DXMATRIX baseY, baseZ;
        D3DXMatrixTranslation(&baseY, now.x, now.y, 0.0f);
        D3DXMatrixTranslation(&baseZ, 0.0f, 0.0f, now.z);
        m_worldMatrix
            = baseZ
            * tarR
            * baseY
            * pTarInfo->pTransform->GetTransformationMatrix();
    }

    m_eye = Vector3::ZERO;
    m_look = m_eye - Vector3::FORWARD;
    

    //const auto dt = Time()()->GetDeltaTime();
    //const float factor = 100.0f;
    //const float dtPow = pow(dt, 2); //dt^3

    //if (bR_buttonStay)//R_button이 눌릴때 까지만
    //{
    //    if (m_position.z > TP_DISTANCE * 0.5f)//견착모드
    //    {
    //        m_vel += dtPow * factor;
    //        m_position.z -= m_vel;
    //        m_position.y -= m_vel * 0.5f;
    //    }
    //}
    //else//R_button이 때어졌을때
    //{
    //    //bR_buttonUp true, distance가 약간 작을때/ 즉 (우측 클릭을 잠깐 눌렸을때)(조준으로 넘어감)
    //    if (bR_buttonUp&&m_position.z >= TP_DISTANCE*0.9f)
    //    {
    //        //!!! 앞으로 이곳에서 캐릭터가 들고 있는 아이템에 따라(2배율,4배율 no 배율 등) 바꾸어 주는 코드를 만들어야 한다.
    //        Camera()()->SetCurrentCamera(TAG_CAMERA::First_Person);
    //        //Camera()()->SetCurrentCamera(TAG_CAMERA::Scope2X);
    //        //Camera()()->SetCurrentCamera(TAG_CAMERA::Scope4X);
    //    }
    //    else//아닌경우 계속 줄여주다가 끝에 다달게 되면 TP로 바꿈
    //    {
    //        if (m_position.z < TP_DISTANCE - (factor*0.018f))
    //        {
    //            m_vel -= dtPow * factor;
    //            m_position.z += m_vel;
    //            m_position.y += m_vel * 0.5f;
    //        }
    //        else
    //        {
    //            m_vel = 0.0f;
    //            Camera()()->SetCurrentCamera(TAG_CAMERA::Third_Person);
    //        }
    //    }

    //}


}




//---------------------------------------------------xx

Camera2xScope::Camera2xScope()
    : CameraFirstPerson(TAG_CAMERA::Scope2X)
    , m_currTime(0.0f)
    , m_totalTime(0.5f)
{
    //44.44 Degrees FP sight //선형보간할 것이다.
    m_fovY_2x = D3DX_PI * (44.44f / 180.0f);
}

Camera2xScope::~Camera2xScope()
{
}

void Camera2xScope::Reset()
{
    CameraFirstPerson::Reset();
    m_currTime = 0.0f;
    m_deltaFovY = 0.0f;

}

void Camera2xScope::Update()
{
    CameraFirstPerson::Update();
    const auto dt = Time()()->GetDeltaTime();
    D3DXMATRIX proj;
    if (m_currTime <= m_totalTime)
    {

        m_currTime += dt;
        float timeFactor = m_currTime / m_totalTime;
        m_deltaFovY = m_fovY * (1.0f - timeFactor) + m_fovY_2x * timeFactor;

        RECT rc;
        GetClientRect(g_hWnd, &rc);
        D3DXMatrixPerspectiveFovLH(&proj, m_deltaFovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom), 1, ICamera::VISUAL_RANGE);
        SetProjectionMatrix(&proj);
        g_pDevice->SetTransform(D3DTS_PROJECTION, &GetProjectionMatrix());
    }

}

CameraLobby::CameraLobby()
    : ICamera(TAG_CAMERA::Lobby)
    , m_rotation(Vector3::ZERO)
    , m_position(Vector3::ZERO)
{
}

CameraLobby::~CameraLobby()
{
}

void CameraLobby::Reset()
{
    m_fovY = D3DX_PI * 0.5f;
    m_position = D3DXVECTOR3(970.0f, 146.0f, -164.0f);
    m_eye = Vector3::ZERO;
    m_look = Vector3::FORWARD;
}

void CameraLobby::Update()
{
    //const float factor = 2.f;

    //InputManager* pInput = Input()();

    //Debug << endl;
    //Debug << "     앞(U)상(I)뒤(O)    " << endl;
    //Debug << " 좌(J)    하(K)    우(L)" << endl;

    //if (pInput->IsStayKeyDown('K')) { m_position.y -= factor; }
    //if (pInput->IsStayKeyDown('I')) { m_position.y += factor; }
    //if (pInput->IsStayKeyDown('J')) { m_position.x -= factor; }
    //if (pInput->IsStayKeyDown('L')) { m_position.x += factor; }
    //if (pInput->IsStayKeyDown('O')) { m_position.z -= factor; }
    //if (pInput->IsStayKeyDown('U')) { m_position.z += factor; }

    //Debug << "CameraLobby Position : " << m_position << endl;

    D3DXMATRIX t;
    D3DXMatrixTranslation(&t, m_position.x, m_position.y, m_position.z);
    m_worldMatrix = t;
    m_eye = Vector3::ZERO;
    m_look = Vector3::FORWARD;
}
