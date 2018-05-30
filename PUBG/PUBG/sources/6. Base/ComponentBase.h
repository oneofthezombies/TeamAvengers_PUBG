#pragma once

class BaseObject;

class ComponentBase
{
private:
    BaseObject* m_pOwner;

protected:
    ComponentBase(BaseObject& owner);

public:
    virtual ~ComponentBase();

    BaseObject* GetOwner() const;
};