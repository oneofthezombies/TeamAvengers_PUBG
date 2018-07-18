#include "stdafx.h"
#include "BloodParticle.h"
#include "ComponentTransform.h"
#include "EffectMeshRenderer.h"

BloodParticle::BloodParticle()
    :IObject(TAG_OBJECT::Particle)
{
}


BloodParticle::~BloodParticle()
{
    for (auto& m : m_blood_hit)
        for (auto& p : m.m_pVB)
            SAFE_RELEASE(p);
}



void BloodParticle::Init()
{
    string filePath = "resources/particle/";

    int num = 0;

    //m_blood_hit.emplace_back(Blood_Hit(8));
    //SetTexture(filePath + "T_Blood", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(31));
    //SetTexture(filePath + "T_Blood_02", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(8));
    //SetTexture(filePath + "T_Blood_03", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "T_Blood_04", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "T_Blood_05", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "T_Blood_06", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);


    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "blood_hit_08", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.0f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "blood_hit_07", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.1f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "blood_hit_06", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.2f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "blood_hit_05", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.3f, -7.0f);

    //m_blood_hit.emplace_back(Blood_Hit(16));
    //SetTexture(filePath + "blood_hit_04", &m_blood_hit[num]);
    //D3DXMatrixTranslation(&m_blood_hit[num++].m_matWorld, 0.0f, 8.4f, -7.0f);


}

void BloodParticle::OnUpdate()
{
}

void BloodParticle::OnRender()
{
    if (GetAsyncKeyState(VK_END) & 0x0001)
    {
        for (auto& p : m_blood_hit)
        {
            p.m_currentIndex = 0;
        }
    }

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
        g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        //g_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
        g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    static int a = 0;
    if (a++ > 1)
    {
        a = 0;
        for (auto& p : m_blood_hit)
        {

            if (p.m_currentIndex < p.m_maxIndex)
                renderBloodHit(p);
        }
    }




    {
        g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
        g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
    }

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
            MessageBox(NULL, TEXT("텍스쳐 없어\n"), 0, 0);
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
        v->p = m_position;
        v->c = D3DXCOLOR(1, 1, 1, 1);
        blood_hit->m_pVB[i]->Unlock();
    }
}

void BloodParticle::renderBloodHit(Blood_Hit & blood_hit)
{
    
    g_pDevice->SetTransform(D3DTS_WORLD, &GetTransform()->GetTransformationMatrix());
    g_pDevice->SetTexture(0, blood_hit.m_pTex[blood_hit.m_currentIndex]);
    g_pDevice->SetFVF(VERTEX_PC::FVF);
    g_pDevice->SetStreamSource(0, blood_hit.m_pVB[blood_hit.m_currentIndex], 0, sizeof(VERTEX_PC));
    g_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 1);

    blood_hit.m_currentIndex++;
}
