#pragma once
#include "IObject.h"

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
        std::vector<LPDIRECT3DTEXTURE9>		    m_pTex;
        std::vector<LPDIRECT3DVERTEXBUFFER9>    m_pVB;
        int                                     m_currentIndex;
        const int                               m_maxIndex;
        

        Blood_Hit(int maxIndex)
            : m_maxIndex(maxIndex)
            , m_currentIndex(0)
        {
        }

    };

private:
    string m_fileName;//이미지 이름

    std::vector<Blood_Hit>                      m_blood_hit;
    D3DXVECTOR3                                 m_position;

public:
    BloodParticle();
    ~BloodParticle();

    void Init();
    // Inherited via IObject
    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void setBloodImagePosition(D3DXVECTOR3& p)
    {

    }
    void SetParticle(string filePath, Blood_Hit* blood_hit);
    void renderBloodHit(Blood_Hit& blood_hit);
    DWORD FtoDw(float f)
    {
        return *((DWORD*)&f);
    }

};

class _BloodParticlePool : public Singleton<_BloodParticlePool>
{
private:
    deque<BloodParticle*> m_BloodParticle;

    _BloodParticlePool();
    ~_BloodParticlePool();

public:
    void Render();

    friend Singleton<_BloodParticlePool>;
};

struct BloodParticlePool
{
    _BloodParticlePool* operator()()
    {
        return _BloodParticlePool::GetInstance();
    }
};
