#include "stdafx.h"
#include "ComponentBase.h"

ComponentBase::ComponentBase(BaseObject& owner)
    : m_pOwner(&owner)
{
}

ComponentBase::~ComponentBase()
{
}

BaseObject* ComponentBase::GetOwner() const
{
    return m_pOwner;
}
