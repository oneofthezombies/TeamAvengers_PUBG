#pragma once
#include "IObject.h"
class Grid : public IObject
{
public:
	vector<VERTEX_PC> m_vecVertex;

	Grid();
	~Grid();

	void Init();
	void Render();

    // Inherited via IObject
    virtual void OnUpdate() override;
    virtual void OnRender() override;
};

