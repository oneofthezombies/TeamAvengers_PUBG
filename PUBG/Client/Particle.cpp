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
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);// ���۸� �׸��� z ���� ���� �ʴ´�//texture�� �ܰ� �κ��� �߶� �� �ִ�.
        g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
        g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
        g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); //���� ũ�⸦ �����ϰڴ�.
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

    //���⿡ m_IsActive ���� false �� �ϴ� ����
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
            assert(false && "BloodParticle::SetParticle( , �ؽ��� ����");
            return;
        }
    }

    blood_hit->m_pVB.resize(blood_hit->m_maxIndex);

    for (int i = 0; i < blood_hit->m_maxIndex; i++)
    {
        g_pDevice->CreateVertexBuffer(sizeof(VERTEX_PC),
            D3DUSAGE_POINTS/*����Ʈ������ ���۷� ���ڴ�*/
            | D3DUSAGE_DYNAMIC/*���� ��� �ٲپ� �ٰ��ε�, dynamic���� �� ������ �ӵ��� ������*/
            | D3DUSAGE_WRITEONLY/*���� ��������*/,
            VERTEX_PC::FVF,
            D3DPOOL_DEFAULT,/*dynamic�� ����Ҷ����� Managed�� ������. ���� ���� �ٲپ� �־�� �ϴµ� GPU����
                            ������ �ϴ� ���� ���� ������, ��� GPU�� �ȱ��*/
            &blood_hit->m_pVB[i], 0);

        VERTEX_PC* v;
        blood_hit->m_pVB[i]->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD/*������ �־��� ������ �Ű澲�� �ʴ´ٴ� ��*/);
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
