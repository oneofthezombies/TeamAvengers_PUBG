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

//Direct3D 는 3D 가속 하드웨어를 이용해 3D 세계 표현을 도와주는 그래픽 API.
//Application - Direct3D - HAL(Hardware Abstraction Layer)-Graphics Hardware
//Device 에서 지원하지 않지만 D3D에서 제공하는 기능을 REF 장치를 통해 이용가능.
//D3DDEVTYPE_HAL or D3DDEVTYPE_REF
//REF 장치는 순수 개발 목적으로 제공된다. DirectX SDK 에만 포함되며 상당히 느림.

//COM(Component Object Model)
//IUnknown Interface 를 상속받음. 세부적인 부분들은 내부적으로 작동하며 자신의
//메모리 관리를 스스로 수행함.
//특수 함수를 통해 객체의 포인터를 얻고 Release Method 를 통해 해제한다.
//(new, delete 키워드 사용 안함)






