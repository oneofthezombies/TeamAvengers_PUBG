#include "stdafx.h"
#include "Water.h"

OptionWater::OptionWater()
{
    isAlphaLayer = true;
    
    textureScale.x = 25.0f;
    textureScale.y = 26.0f;

    waveFreq    = 0.028f;
    waveAmp     = 1.8f;
    
    bumpScale   = 1.0f;
    bumpSpeed.x = 0.015f;
    bumpSpeed.y = 0.005f;

    deepColor.x = 0.0f;
    deepColor.y = 0.3f;
    deepColor.z = 0.5f;
    deepColor.w = 0.8f;

    shallowColor.x = 0.0f;
    shallowColor.y = 1.0f;
    shallowColor.z = 1.0f;
    shallowColor.w = 1.0f;

    waterAmount = 0.3f;
    
    reflectionColor.x = 0.95f;
    reflectionColor.y = 1.0f;
    reflectionColor.z = 1.0f;
    reflectionColor.w = 1.0f;

    reflectionAmount = 1.0f;
    reflectionBlur   = 0.0f;
    
    fresnelPower  = 5.0f;
    fresnelBias   = 0.328f;
    hdrMultiplier = 0.471f;
}

OptionWater::~OptionWater()
{
}

Water::Water()
    :IObject(TAG_OBJECT::Idle)
    , m_pVertexBuffer(nullptr)
    , m_pWaterBump(nullptr)
    , m_pReflectionMap(nullptr)
    , m_pEffectWater(nullptr)
    , m_timer(0.0f)
    , m_optionWater()
{
}


Water::~Water()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pWaterBump);
    SAFE_RELEASE(m_pReflectionMap);
    SAFE_RELEASE(m_pEffectWater);
}

void Water::OnUpdate()
{
}

void Water::OnRender()
{
    if (m_optionWater.isAlphaLayer)
    {
        // 텍스쳐 사용 준비. 
        DWORD alpha = 175;
        m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, (0x00ffffff) + (alpha << 24));
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

        // 알파 채널을 넣어서 반투명하게 하자.
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    }

    // Pick one of the techniques in the .fx file to use. For this sample, 
    // we only have one called, "Technique0".
    m_pEffectWater->SetTechnique("water");

    setTechniqueVariables();

    // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
    // save and restore all state modified by the effect.
    UINT uPasses;
    m_pEffectWater->Begin(&uPasses, 0);

    for (UINT uPass = 0; uPass < uPasses; ++uPass)
    {
        m_pEffectWater->BeginPass(uPass);

        //
        // With each pass, render geometry as you normally would...
        //

        m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
        m_pD3DDevice->SetFVF(Vertex::FVF_Flags);
        m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

        m_pEffectWater->EndPass();
    }

    m_pEffectWater->End();

    if(m_optionWater.isAlphaLayer)
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Water::Init(float width, float height, float high)
{
    m_pD3DDevice = DeviceMgr()()->GetDevice();

    m_quadVertices[0].Set(0000.0f, high, 0000.0f, 0xffffff00, 0.0f, 0.0f);
    m_quadVertices[1].Set(width, high, 0000.0f, 0xff00ff00, 1.0f, 0.0f);
    m_quadVertices[2].Set(0000.0f, high, -height, 0xffff0000, 0.0f, 1.0f);
    m_quadVertices[3].Set(width, high, -height, 0xff0000ff, 1.0f, 1.0f);

    Device()()->CreateVertexBuffer(
        4 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF_Flags,
        D3DPOOL_DEFAULT,
        &m_pVertexBuffer,
        NULL
    );
    void* pVertices = NULL;

    m_pVertexBuffer->Lock(
        0,
        sizeof(m_quadVertices),
        (void**)&pVertices,
        0);
    memcpy(pVertices, m_quadVertices, sizeof(m_quadVertices));
    m_pVertexBuffer->Unlock();

    InitEffect();
}

void Water::InitEffect()
{
    D3DXCreateTextureFromFile(m_pD3DDevice     , "./Resource/Effect/Texture/wave.jpg", &m_pWaterBump);
    D3DXCreateCubeTextureFromFile(m_pD3DDevice , "./Resource/Effect/Texture/cubemap.dds", &m_pReflectionMap);

    HRESULT hr;
    LPD3DXBUFFER pBufferErrors = nullptr;

    hr = D3DXCreateEffectFromFile(
        m_pD3DDevice,
        "./Resource/Effect/Shader/water.fx",
        NULL,
        NULL,
        D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
        NULL,
        &m_pEffectWater,
        &pBufferErrors);

    if (FAILED(hr))
    {
        LPVOID pCompileErrors = pBufferErrors->GetBufferPointer();
        MessageBox(
            NULL,
            (LPCSTR)pCompileErrors,
            "FX compile Error",
            MB_OK | MB_ICONEXCLAMATION);
    }
}

void Water::setTechniqueVariables()
{
    m_timer += 0.02f;

    D3DXMATRIX				matWorldViewProj;		// WorldViewProjMatrix
    D3DXMATRIX				matWorld;				// World		Matrix
    D3DXMATRIX				matView;				// View			Matrix
    D3DXMATRIX				matProj;				// Projection	Matrix

    m_pD3DDevice->GetTransform(D3DTS_WORLD,      &matWorld);
    m_pD3DDevice->GetTransform(D3DTS_VIEW,       &matView);
    m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

    //Calculate the matrices
    matWorldViewProj = matWorld * matView * matProj;

    //set the textures
    m_pEffectWater->SetTexture("texture0", m_pWaterBump);
    m_pEffectWater->SetTexture("texture1", m_pReflectionMap);

    //set the matrices
    m_pEffectWater->SetMatrix("WorldViewProj", &matWorldViewProj);

    //set eye position
    D3DXVECTOR4 eye(Camera()()->GetCurrentCamera()->GetPosition(), 1.0f);
    m_pEffectWater->SetVector("eyePosition", &eye);

    //set time
    m_pEffectWater->SetFloat("time", m_timer);

    //파고 조절 컨트롤
    m_pEffectWater->SetValue("textureScale"     , m_optionWater.textureScale, sizeof(D3DXVECTOR2));
    m_pEffectWater->SetFloat("waveFreq"         , m_optionWater.waveFreq);
    m_pEffectWater->SetFloat("waveAmp"          , m_optionWater.waveAmp);
                                                
    m_pEffectWater->SetFloat("BumpScale"        , m_optionWater.bumpScale);
    m_pEffectWater->SetValue("bumpSpeed"        , m_optionWater.bumpSpeed, sizeof(D3DXVECTOR2));
    m_pEffectWater->SetVector("deepColor"       , &m_optionWater.deepColor);
    m_pEffectWater->SetVector("shallowColor"    , &m_optionWater.shallowColor);
    m_pEffectWater->SetFloat("waterAmount"      , m_optionWater.waterAmount);

    m_pEffectWater->SetVector("reflectionColor" , &m_optionWater.reflectionColor);
    m_pEffectWater->SetFloat("reflectionAmount" , m_optionWater.reflectionAmount);
    m_pEffectWater->SetFloat("reflectionBlur"   , m_optionWater.reflectionBlur);

    m_pEffectWater->SetFloat("fresnelPower"     , m_optionWater.fresnelPower);
    m_pEffectWater->SetFloat("fresnelBias"      , m_optionWater.fresnelBias);
    m_pEffectWater->SetFloat("hdrMultiplier"    , m_optionWater.hdrMultiplier);
}
