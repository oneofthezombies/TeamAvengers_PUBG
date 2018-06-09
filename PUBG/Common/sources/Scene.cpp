#include "stdafx.h"
#include "IScene.h"

IScene::IScene()
{
}

IScene::~IScene()
{
}

void IScene::Destroy(IDisplayObject* p, const float t)
{
    const auto search = m_umapToDeleteIDisplayObjects.find(p);
    if (search != m_umapToDeleteIDisplayObjects.end()) return;

    m_umapToDeleteIDisplayObjects.emplace(p, t);
}

void IScene::UpdateToDeleteIDisplayObjects()
{
    const float dt = g_pTimeManager->GetDeltaTime();

    for (auto it = m_umapToDeleteIDisplayObjects.begin(); it != m_umapToDeleteIDisplayObjects.end();)
    {
        auto p = it->first;
        auto& t = it->second;

        t -= dt;
        if (t > 0.0f)
        {
            ++it;
        }
        else
        {
            m_usetIDisplayObjects.erase(p);
            SAFE_RELEASE(p);
            SAFE_DELETE(p);
            it = m_umapToDeleteIDisplayObjects.erase(it);
        }
    }
}

void IScene::AddSimpleDisplayObj(IDisplayObject* val)
{
    m_usetIDisplayObjects.emplace(val);
}

void IScene::RemoveSimpleDisplayObj(IDisplayObject* val)
{
    m_usetIDisplayObjects.erase(val);
}

void IScene::OnUpdateIScene()
{
    for (auto d : m_usetIDisplayObjects)
        SAFE_UPDATE(d);

    UpdateToDeleteIDisplayObjects();
}

void IScene::OnRenderIScene()
{
    for (auto d : m_usetIDisplayObjects)
        SAFE_RENDER(d);
}

void IScene::OnDestructIScene()
{
    for (auto d : m_usetIDisplayObjects)
        SAFE_RELEASE(d);
}
