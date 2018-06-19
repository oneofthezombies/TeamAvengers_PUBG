#include "stdafx.h"
#include "Direct3D.h"

Direct3D::Direct3D()
	: Singleton<Direct3D>()
	, m_pD3D(nullptr)
	, m_pD3DDevice(nullptr)
{
}

Direct3D::~Direct3D()
{
}

HRESULT Direct3D::Init()
{
    //버전 정보를 통해 IDirect3D9 Interface 의 포인터 획득
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3D == NULL) return E_FAIL;

    D3DCAPS9	caps;
    int			vp;

    //주 그래픽카드의 정보를 D3DCAPS9 에 받아온다.
    if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, &caps)))
        return E_FAIL;

    //하드웨어가 정점처리를 지원하는지 확인
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, vp, &d3dpp, &m_pD3DDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

LPDIRECT3DDEVICE9 Direct3D::GetDevice()
{
    return m_pD3DDevice;
}

void Direct3D::Destroy()
{
    if (m_pD3DDevice)
    {
        auto unreleased = m_pD3DDevice->Release();
        assert(unreleased == 0 && "디바이스를 이용해서 생성한 객체 중 소멸되지 않은 객체가 있습니다.");
    }

	SAFE_RELEASE(m_pD3D);
}