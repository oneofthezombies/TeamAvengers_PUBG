#pragma once
#include "IObject.h"
#include "Singleton.h"

struct VERTEX_PC
{
    D3DXVECTOR3 p;
    D3DCOLOR c;

    VERTEX_PC() {}
    VERTEX_PC(D3DXVECTOR3 _p, D3DCOLOR _c) :p(_p), c(_c) {}

    enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

class BloodParticle :
    public IObject
{
    struct Blood_Hit
    {
        std::vector<LPDIRECT3DTEXTURE9>          m_pTex;
        std::vector<LPDIRECT3DVERTEXBUFFER9>    m_pVB;
        float                                   m_dt;
        int                                     m_currentIndex;
        const int                               m_maxIndex;


        Blood_Hit(int maxIndex)
            : m_maxIndex(maxIndex)
            , m_currentIndex(0)
            , m_dt(0.0f)
        {
        }

    };


private:
    std::vector<Blood_Hit>                      m_blood_hit_splash;
    std::vector<Blood_Hit>                      m_blood_hit_splurt;
    bool                                        m_IsActive;

public:
    BloodParticle();
    ~BloodParticle();

    void Init();
    // Inherited via IObject
    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void Set(const D3DXVECTOR3& pos,const D3DXQUATERNION& rot);

    bool IsActive() const
    {
        return m_IsActive;
    }

    void setBloodImagePosition(D3DXVECTOR3& p)
    {

    }
    void SetParticle(string filePath, Blood_Hit* blood_hit);
    void renderBloodHit_Splash(Blood_Hit& blood_hit);
    void renderBloodHit_Splurt(Blood_Hit& blood_hit);
    DWORD FtoDw(float f)
    {
        return *((DWORD*)&f);
    }

};

class _ParticlePool : public Singleton<_ParticlePool>
{
private:
    deque<BloodParticle*> m_BloodParticle;
    //deque<BulletHoleParticle*> m_BulletHoleParticle;

    LPD3DXEFFECT                 pParticleEffect;
    LPDIRECT3DVERTEXDECLARATION9 m_pParticleDecl;

private:
             _ParticlePool();
    virtual ~_ParticlePool();

public:
    void Init();
    void Destroy();
    void Render();

    BloodParticle* Hit_Blood(const D3DXVECTOR3& hitPos,const D3DXQUATERNION& hitRot);
    //BulletHoleParticle* Hit_BulletHole(const D3DXVECTOR3& hitPos);

    LPD3DXEFFECT GetEffect() const;

    friend Singleton<_ParticlePool>;
};

struct ParticlePool
{
    _ParticlePool* operator()()
    {
        return _ParticlePool::GetInstance();
    }
};

