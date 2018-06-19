#pragma once
#include "Singleton.h"

#define g_pD3D    Direct3D::GetInstance()
#define g_pDevice Direct3D::GetInstance()->GetDevice()

class Direct3D : public Singleton<Direct3D>
{
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

	Direct3D();
	~Direct3D();

public:
	HRESULT				Init();
	LPDIRECT3DDEVICE9	GetDevice();
	void				Destroy();

	friend Singleton<Direct3D>;
};

//Direct3D �� 3D ���� �ϵ��� �̿��� 3D ���� ǥ���� �����ִ� �׷��� API.
//Application - Direct3D - HAL(Hardware Abstraction Layer)-Graphics Hardware
//Device ���� �������� ������ D3D���� �����ϴ� ����� REF ��ġ�� ���� �̿밡��.
//D3DDEVTYPE_HAL or D3DDEVTYPE_REF
//REF ��ġ�� ���� ���� �������� �����ȴ�. DirectX SDK ���� ���ԵǸ� ����� ����.

//COM(Component Object Model)
//IUnknown Interface �� ��ӹ���. �������� �κе��� ���������� �۵��ϸ� �ڽ���
//�޸� ������ ������ ������.
//Ư�� �Լ��� ���� ��ü�� �����͸� ��� Release Method �� ���� �����Ѵ�.
//(new, delete Ű���� ��� ����)






