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

    LPD3DXMESH          m_testSphereMesh;

	LPD3DXMESH	        m_pMesh;
    D3DXMATRIXA16       m_matWorld;

	
	vector<D3DXVECTOR3>	m_vecSurfaceVertex;


    D3DXVECTOR3         m_boundary[8];
    vector<D3DXVECTOR3> m_boundaryBox;
private:
    void drawIndices(const vector<WORD>& indices, const D3DXCOLOR & color);
public:
	HeightMap();
	virtual ~HeightMap();

	void Load(LPCTSTR fullPath, D3DXMATRIXA16 * pMat = NULL);
    void SetSurface();
    virtual void SetRayBox();
    vector<D3DXVECTOR3>& GetBoundaryBox()
    {
        return m_boundaryBox;
    }

    // Inherited via IMap
    virtual void OnUpdate() override;
    virtual void OnRender() override;
    virtual bool GetHeight(const D3DXVECTOR3 & pos, OUT float * OutHeight) override;

};
