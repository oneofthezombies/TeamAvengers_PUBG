#pragma once

class IObject;

class IScene
{
private:
    unordered_set<IObject*>        m_objects;
    unordered_map<IObject*, float> m_toDeleteObjects;

protected:
    IScene();

	void UpdateToDeleteObjects();

public:
    virtual ~IScene();
	
	void Init();
	void Update();
	void Render();

    void AddObject(IObject* p);
    void RemoveObject(IObject* p);

	void Destroy(IObject* p, const float t = 0.0f);

    virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
};

