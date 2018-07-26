#pragma once
#include "IObject.h"

struct Vertex
{
    float x, y, z;
    DWORD color;
    float tu, tv;

    enum FVF 
    { 
        FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
    };

    void Set(
        float _x,
        float _y,
        float _z,
        DWORD _color,
        float _tu,
        float _tv)
    {
        x = _x;
        y = _y;
        z = _z;
        color = _color;
        tu = _tu;
        tv = _tv;
    }
};

struct OptionWater
{
    bool        isAlphaLayer;

    //파고를 조절하는 각종 변수들
    D3DXVECTOR2 textureScale;
    float       waveFreq;
    float       waveAmp;

    float       bumpScale;
    D3DXVECTOR2 bumpSpeed;
    D3DXVECTOR4 deepColor;
    D3DXVECTOR4 shallowColor;
    float       waterAmount;

    D3DXVECTOR4 reflectionColor;
    float       reflectionAmount;
    float       reflectionBlur;

    float       fresnelPower;
    float       fresnelBias;
    float       hdrMultiplier;
    
    OptionWater();
    ~OptionWater();
};

class Water : public IObject
{
public:
    OptionWater              m_optionWater;
    LPDIRECT3DDEVICE9        m_pD3DDevice;
    LPDIRECT3DVERTEXBUFFER9  m_pVertexBuffer;
    LPDIRECT3DTEXTURE9       m_pWaterBump;     //물
    LPDIRECT3DCUBETEXTURE9   m_pReflectionMap; //하늘의 반사맵
    LPD3DXEFFECT             m_pEffectWater;

    Vertex                   m_quadVertices[4];
    float                    m_timer;

public:
    Water();
    ~Water();

    virtual void OnUpdate() override;
    virtual void OnRender() override;


    void Init(float width, float height, float high);
    void InitEffect();

    void setTechniqueVariables();
};

