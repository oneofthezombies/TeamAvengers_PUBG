#pragma once
#include "IObject.h"

class IMap : public IObject
{
protected:
	vector<D3DXVECTOR3>	m_Vertices;
	
    int	m_NumTile;
	int	m_Dimension;

    IMap();

public:
    virtual ~IMap();

	virtual bool GetHeight(const D3DXVECTOR3& pos, float* OutHeight) = 0;
	virtual bool CalcPickedPosition(D3DXVECTOR3 &vOut, WORD screenX, WORD screenY) = 0;

    const vector<D3DXVECTOR3>& GetVertices() const;

    void SetDimension(const int dimension);
    int  GetDimension() const;	
};