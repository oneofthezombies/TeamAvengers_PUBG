#pragma once
#include "IObject.h"

class EffectMeshRenderer;

class Item : public IObject
{
public:
    static const int DEFAULT_NUM_BULLET = 30;

private:
    const TAG_RES_STATIC m_tagResStatic;
    bool  m_isInField;  //�ʵ忡 �����ִ���

    float m_durability; //������
    int   m_numBullet;  //�Ѿ��� ����

    EffectMeshRenderer* pEffectMeshRenderer;

private:
    void setup(const TAG_RES_STATIC tag);

public:
             Item(
                 const TAG_RES_STATIC tag, 
                 const D3DXVECTOR3& position, 
                 const D3DXVECTOR3& rotation, 
                 const D3DXVECTOR3& scale);
    virtual ~Item();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_RES_STATIC GetTagResStatic() const;

    void SetDurability(const float durability);
    float GetDurability() const;

    void SetNumBullet(const int numBullet);
    int GetNumBullet() const;
};