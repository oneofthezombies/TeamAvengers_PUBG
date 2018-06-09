#pragma once
#include "BaseObject.h"

class IDisplayObject;

class IScene : public BaseObject
{
protected:
    unordered_set<IDisplayObject*> m_usetIDisplayObjects;
    unordered_map<IDisplayObject*, float> m_umapToDeleteIDisplayObjects;

    IScene();

public:
    virtual ~IScene();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    void AddSimpleDisplayObj(IDisplayObject* val);
    void RemoveSimpleDisplayObj(IDisplayObject* val);

    void OnUpdateIScene();
    void OnRenderIScene();
    void OnDestructIScene();

	void Destroy(IDisplayObject* p, const float t = 0.0f);
	void UpdateToDeleteIDisplayObjects();
};

