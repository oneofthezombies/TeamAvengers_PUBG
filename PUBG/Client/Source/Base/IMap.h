#pragma once
#include "IObject.h"

class IMap : public IObject
{
protected:
	vector<D3DXVECTOR3>	m_vecVertex;
	
    int	m_numTile;
	int	m_dimension;

    IMap();

public:
    virtual ~IMap();

	virtual bool GetHeight(const D3DXVECTOR3& pos,OUT float* OutHeight) = 0;
    virtual void SetRayBox() = 0;
	//virtual bool CalcPickedPosition(D3DXVECTOR3 &vOut, WORD screenX, WORD screenY) = 0;

    const vector<D3DXVECTOR3>& GetVertices() const;

    void SetDimension(const int dimension);
    int  GetDimension() const {return m_dimension;}
};