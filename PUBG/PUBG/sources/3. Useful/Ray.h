#pragma once
class Ray
{
public:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	Ray();
    Ray(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	~Ray();

    void SetRay(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
    {
        m_pos = pos;
        m_dir = dir;
    }

	static Ray RayAtViewSpace(int x, int y);
	static Ray RayAtWorldSpace(int x, int y);

	bool CalcIntersectSphere(BoundingSphere* pSphere);
	bool CalcIntersectTri(D3DXVECTOR3* pStart, float *distance);
};

