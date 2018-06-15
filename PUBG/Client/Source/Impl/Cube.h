#pragma once
#include "IObject.h"

class UIText;

class Cube : public IObject
{
private:
    UIText* m_pUIText;
    bool m_isMine;
    string m_text;
    int m_ID;

public:
    Cube();
    virtual ~Cube();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void SetIsMine(const bool val);
    void SetID(const int val);
    int GetID() const;
    void SetPosition(const D3DXVECTOR3& pos);
};