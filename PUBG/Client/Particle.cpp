#include "stdafx.h"
#include "Particle.h"
#include "ComponentTransform.h"
#include "EffectMeshRenderer.h"
#include "ScenePlay.h"

BloodParticle::BloodParticle()
    :IObject(TAG_OBJECT::Particle)
    , m_IsActive(false)
{
    Init();
}


BloodParticle::~BloodParticle()
{
    for (auto& m : m_blood_hit_splash)
        for (auto& p : m.m_pVB)
            SAFE_RELEASE(p);
    for (auto& m : m_blood_hit_splurt)
        for (auto& p : m.m_pVB)
            SAFE_RELEASE(p);
}

void BloodParticle::Init()
{
    string filePath = "./Resource/particle/";


    Transform* tm = GetTransform();
    tm->SetPosition(Vector3::ZERO);
    tm->Update();
    
    size_t splashNum = 0;
    size_t splurtNum = 0;


    m_blood_hit_splash.emplace_back(Blood_Hit(8));
    SetParticle(filePath + "T_Blood_01", &m_blood_hit_splash[splashNum++]);

    m_blood_hit_splash.emplace_back(Blood_Hit(32));
    SetParticle(filePath + "T_Blood_02", &m_blood_hit_splash[splashNum++]);
    
    m_blood_hit_splash.emplace_back(Blood_Hit(8));
    SetParticle(filePath + "T_Blood_03", &m_blood_hit_splash[splashNum++]);

    m_blood_hit_splurt.emplace_back(Blood_Hit(16));
    SetParticle(filePath + "T_Blood_04", &m_blood_hit_splurt[splurtNum++]);

    m_blood_hit_splurt.emplace_back(Blood_Hit(16));
    SetParticle(filePath + "T_Blood_05", &m_blood_hit_splurt[splurtNum++]);

    m_blood_hit_splurt.emplace_back(Blood_Hit(16));
    SetParticle(filePath + "T_Blood_06", &m_blood_hit_splurt[splurtNum++]);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetParticle(filePath + "blood_hit_08", &m_blood_hit[num++]);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetParticle(filePath + "blood_hit_07", &m_blood_hit[num++]);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetParticle(filePath + "blood_hit_06", &m_blood_hit[num++]);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetParticle(filePath + "blood_hit_04", &m_blood_hit[num++]);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetParticle(filePath + "blood_hit_05", &m_blood_hit[num++]);



}

void BloodParticle::OnUpdate()
{
}

void BloodParticle::OnRender()
{
    if (!m_IsActive)
        return;

    {
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);// 버퍼를 그릴때 z 값을 넣지 않는다//texture의 외각 부분을 잘라낼 수 있다.
        g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
        g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
        g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); //점의 크기를 조절하겠다.
        g_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.4f));
        g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

        //g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
        //g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
        //g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(0.0f));

        g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        g_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
        //g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }


    
    int donePlayNum = 0;
    for (auto& p : m_blood_hit_splash)
    {

        if (p.m_currentIndex < p.m_maxIndex)
            renderBloodHit_Splash(p);
        else
            donePlayNum++;
    }
    for (auto& p : m_blood_hit_splurt)
    {
        if(p.m_currentIndex<p.m_maxIndex)
            renderBloodHit_Splurt(p);
        else
            donePlayNum++;
    }

    //여기에 m_IsActive 로직 false 로 하는 로직
    if (m_blood_hit_splash.size() + m_blood_hit_splurt.size() 
        == donePlayNum)
        m_IsActive = false;


    {
        g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
        g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
    }

}

void BloodParticle::Set(const D3DXVECTOR3 & pos)
{
    Transform* tm = GetTransform();
    tm->SetPosition(pos);
    tm->Update();

    m_IsActive = true;

    for (auto& p : m_blood_hit_splash)
    {
        p.m_currentIndex = 0;
    }

    for (auto& p : m_blood_hit_splurt)
    {
        p.m_currentIndex = 0;
    }

    CurrentScene()()->AddObject(this);
}

void BloodParticle::SetParticle(string filePath, Blood_Hit * blood_hit)
{
    blood_hit->m_pTex.resize(blood_hit->m_maxIndex);
    for (int i = 0; i <blood_hit->m_maxIndex; i++)
    {
        string  str = filePath + "/tile0";
        if (i < 10)
            str = str + "0" + to_string(i);
        else
            str = str + to_string(i);
        str += ".png";
        blood_hit->m_pTex[i] = Resource()()->GetTexture(str);
        if (blood_hit->m_pTex[i] == NULL)
        {
            assert(false && "BloodParticle::SetParticle( , 텍스쳐 없어");
            return;
        }
    }

    blood_hit->m_pVB.resize(blood_hit->m_maxIndex);

    for (int i = 0; i < blood_hit->m_maxIndex; i++)
    {
        g_pDevice->CreateVertexBuffer(sizeof(VERTEX_PC),
            D3DUSAGE_POINTS/*포인트전용의 버퍼로 쓰겠다*/
            | D3DUSAGE_DYNAMIC/*값을 계속 바꾸어 줄것인데, dynamic으로 해 놓으면 속도가 빠르다*/
            | D3DUSAGE_WRITEONLY/*쓰기 전용으로*/,
            VERTEX_PC::FVF,
            D3DPOOL_DEFAULT,/*dynamic을 사용할때에는 Managed를 못쓴다. 빨리 값을 바꾸어 주어야 하는데 GPU에서
                            변경을 하는 것이 가장 좋으니, 모두 GPU에 옴긴다*/
            &blood_hit->m_pVB[i], 0);

        VERTEX_PC* v;
        blood_hit->m_pVB[i]->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD/*기존에 있었던 값들은 신경쓰지 않는다는 뜻*/);
        v->p = GetTransform()->GetPosition();
        v->c = D3DXCOLOR(1, 1, 1, 1);
        blood_hit->m_pVB[i]->Unlock();
    }
}

void BloodParticle::renderBloodHit_Splash(Blood_Hit & blood_hit)
{
    g_pDevice->SetTransform(D3DTS_WORLD, &GetTransform()->GetTransformationMatrix());
    g_pDevice->SetTexture(0, blood_hit.m_pTex[blood_hit.m_currentIndex]);
    g_pDevice->SetFVF(VERTEX_PC::FVF);
    g_pDevice->SetStreamSource(0, blood_hit.m_pVB[blood_hit.m_currentIndex], 0, sizeof(VERTEX_PC));
    g_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 1);

    blood_hit.m_currentIndex++;
}

void BloodParticle::renderBloodHit_Splurt(Blood_Hit & blood_hit)
{
    ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
    
    D3DXMATRIX r, t, mat;
    D3DXMatrixRotationQuaternion(&r, &pScenePlay->GetPlayer()->GetTransform()->GetRotation());
    D3DXMatrixTranslation(&t, -500.0f, 0.0f, 0.0f);
    mat = r * t * GetTransform()->GetTransformationMatrix();

    g_pDevice->SetTransform(D3DTS_WORLD, &mat);
    g_pDevice->SetTexture(0, blood_hit.m_pTex[blood_hit.m_currentIndex]);
    g_pDevice->SetFVF(VERTEX_PC::FVF);
    g_pDevice->SetStreamSource(0, blood_hit.m_pVB[blood_hit.m_currentIndex], 0, sizeof(VERTEX_PC));
    g_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 1);

    blood_hit.m_currentIndex++;
}

_ParticlePool::_ParticlePool()
    : Singleton<_ParticlePool>()
{
}

_ParticlePool::~_ParticlePool()
{
}

void _ParticlePool::Render()
{

}

BloodParticle * _ParticlePool::Hit_Blood(
    const D3DXVECTOR3 & hitPos)
{
    for (auto& b : m_BloodParticle)
    {
        if (!b->IsActive())
        {
            b->Set(hitPos);
            return b;
        }
    }

    BloodParticle* b = new BloodParticle;
    m_BloodParticle.emplace_back(b);
    b->Set(hitPos);
    return b;
}
