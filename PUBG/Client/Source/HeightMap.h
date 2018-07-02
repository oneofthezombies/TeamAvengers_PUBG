#pragma once
#include "IMap.h"
struct VERTEX_PNT
{
    D3DXVECTOR3 p;
    D3DXVECTOR3	n;
    D3DXVECTOR2	t;

    VERTEX_PNT() {}
    VERTEX_PNT(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}

    enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

//extern D3DVERTEXELEMENT9 decl[];

class HeightMap :	public IMap
{
protected:
    D3DXVECTOR3			m_size;
    D3DXVECTOR3			m_scale;

	LPD3DXMESH	        m_pMesh;
	//MTLTEX*		m_pMtlTex;
	
	vector<D3DXVECTOR3>	m_vecSurfaceVertex;
	vector<D3DXVECTOR3>	m_vecObstacleVertex;

public:
	HeightMap();
	virtual ~HeightMap();

	void Load(LPCTSTR fullPath, D3DXMATRIXA16 * pMat = NULL);
    void SetSurface();
	
	

    // Inherited via IMap
    virtual void OnUpdate() override;
    virtual void OnRender() override;
    virtual bool GetHeight(const D3DXVECTOR3 & pos, OUT float * OutHeight) override;
    virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) override;
    
    //void SetMtlTex(D3DMATERIAL9 &Mtl, LPDIRECT3DTEXTURE9 pTex);
    /*void SetObstacle();*/
};
//
//
///*
//���� ���� �б�
//fopen_s(&fp, szFullPath, "rb");
//y = (unsigned char)fgetc(fp)
//
//����Ʈ�� ������ 257 * 257
//
//���ؽ��� ���� 257 * 257
//Ÿ���� ���� 256 * 256
//
//1. ���ؽ� ��ǥ ����(�븻�� 0, 1, 0)
//�ε��� ��ȣ = z * 257 + x
//y = ������ / 10.0f; �и�� ���� ����
//
//2. �ε��� ����(�ð����)
//1--3	0 = z * 257 + x
//|\ |	1 = (z + 1) * 257 + x
//| \|	2 = z * 257 + x + 1
//0--2	3 = (z + 1) * 257 + x + 1
//
//3. �븻�� ����
//---u---
//|\ |\ |
//| \| \|
//l--n--r
//|\ |\ |
//| \| \|
//---d---
//
//du ���Ϳ� lr ���͸� �����ؼ�
//n��ġ�� �븻 ���� ���Ѵ�.
//
//4. �޽��� ����(����ȭ����)
//
//5. MtlTex ���� ����
//
//== == == == == == == == == == == == == == == == == == ==
//GetHeight �Լ�
//ĳ������ ���� ����
//1. ���� ���̽� ����
//1--3	0�� x y ��ǥ ã��
//|\ |	x = (int)��ġx
//| \|	y = (int)��ġy
//0--2	deltaX = ��ġx - x
//deltaY = ��ġy - y
//
//deltaX + deltaY <  1 : �Ʒ��� �ﰢ��
//deltaX + deltaY	>= 1 : ���� �ﰢ��
//
//2. ���� ���� ����ϱ�
//
//1--3	�� �� ������ ���� ���� ���
//|\ |	�Ʒ��� �ﰢ���� ��� : 0 + �������� * ��Ÿ��
//| \|	���� �ﰢ���� ��� : 3 + �������� * ��Ÿ��
//0--2
//*/
