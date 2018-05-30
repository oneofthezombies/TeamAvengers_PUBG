#pragma once

#define g_pDeviceManager DeviceManager::GetInstance()
#define g_pDevice DeviceManager::GetInstance()->GetDevice()

class DeviceManager
{
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

	DeviceManager();
	~DeviceManager();

public:
	static DeviceManager* GetInstance()
	{
		static DeviceManager instance;
		return &instance;
	}

	HRESULT				Init();
	LPDIRECT3DDEVICE9	GetDevice();
	void				Destroy();
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






