#pragma once
#include "BaseObject.h"

class IDisplayObject : public BaseObject
{
protected:
	D3DXVECTOR3				m_pos;
	D3DXVECTOR3				m_rot;
	D3DXMATRIX			m_matWorld;
    float                   m_heightOffset;

	IDisplayObject*			m_pParent;
	vector<IDisplayObject*> m_vecPChild;

public:
	IDisplayObject();
	virtual ~IDisplayObject();

    virtual void Release() override;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

    void UpdateTransform();

	virtual void AddChild(IDisplayObject& val);

    void			     SetPosition(const D3DXVECTOR3& pos);
    D3DXVECTOR3		     GetPosition() const;

    D3DXVECTOR3		     GetRotation() const;
    const D3DXMATRIX& GetWorldMatrix() const;
    const vector<IDisplayObject*>& GetChildVec() const;

    void SetHeightOffset(const float val);

    void UpdateChildren();
    void RenderChildren();
    void ReleaseChildren();

    void UpdatePositionYOnMap();
};

