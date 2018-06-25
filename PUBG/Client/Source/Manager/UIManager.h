#pragma once
#include "Singleton.h"
#include "UIObject.h"
#include "UIButton.h"

class UIText;
class UIImage;

class UIManager : public Singleton<UIManager>
{
private:
    LPD3DXSPRITE m_pSprite;

    unordered_set<UIObject*>        m_UIObjects;
    unordered_map<UIObject*, float> m_toDeleteUIObjects;
    int                             m_UIObjectInstanceID;
    bool                            m_isDrawBorder;

             UIManager();
    virtual ~UIManager();

    void initSprite();
    void updateToDeleteUIObjects();

public:
    void Init();
    void Destroy();
    void Update();
    void Render();

    void RegisterUIObject(UIObject* p);
    void Destroy(UIObject* p, const float t = 0.0f);
    void Destroy(const int instanceID, const float t = 0.0f);

    const LPD3DXSPRITE GetSprite() const;
    void SetIsDrawBorder(const bool val);
    bool IsDrawBorder() const;

    friend Singleton<UIManager>;
};

struct UI
{
    UIManager* operator()()
    {
        return UIManager::GetInstance();
    }
};

struct Sprite
{
    LPD3DXSPRITE operator()()
    {
        return UIManager::GetInstance()->GetSprite();
    }
};