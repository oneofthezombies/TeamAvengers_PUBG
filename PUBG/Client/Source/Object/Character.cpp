#include "stdafx.h"
#include "TerrainFeature.h" //�̰� �����

void Character::RifleShootingTest() //bullet ��ü�� ����
{



}

D3DXVECTOR3 Character::FindShootingTargetPos()
{
    //�� �Լ��� ���� ����� Object�� ã�Ƽ� 

    //Screen���� �� Ray �������� cell���� ����.
    IScene* CS = CurrentScene()();
    Ray ray = CS->m_RayArea.CreateRayArea();

    vector<D3DXVECTOR3> vecP; //�̰����� ���� ����� pos�� ���� ���̴�

                              //1.terrain���� �浹
    auto tfs(CS->m_RayArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        // ���� terrain features�� �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
        BoundingSphere bs = tf->GetBoundingSphere();

        if (!D3DXSphereBoundProbe(
            &(bs.center + bs.position),
            bs.radius,
            &ray.m_pos,
            &ray.m_dir)) continue;


        // ��ü�� �ٿ���ڽ���� �浹�� �˻��Ѵ�.
        float minDist = std::numeric_limits<float>::max();
        float dist = std::numeric_limits<float>::max();

        const auto& obbs = tf->GetBoundingBoxes();
        for (std::size_t i = 0; i < obbs.size(); i++)
        {
            auto& obb = obbs[i];

            if (Collision::HasCollision(ray, obb, &dist))
            {
                // hit
                if (dist < minDist)
                {
                    minDist = dist;

                }
            }
        }
        if (minDist != std::numeric_limits<float>::max())
        {
            vecP.emplace_back(ray.m_pos + ray.m_dir * minDist);
        }
    }

    ////2. character���� �浹
    //auto chrs(CS->m_RayArea.GetCharacters());
    //for (auto chr : chrs)
    //{
    //    // ���� ĳ������ �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
    //    BoundingSphere bs = chr->GetBoundingSphere();

    //    if (!D3DXSphereBoundProbe(
    //        &(bs.center + bs.position),
    //        bs.radius,
    //        &ray.m_pos,
    //        &ray.m_dir)) continue;

    //    

    //}
    D3DXVECTOR3 targetPos;
    float save = FLT_MAX;
    for (int i = 0; i < vecP.size(); i++)
    {
        float length = D3DXVec3Length(&(vecP[i] - ray.m_pos));
        if (save > length)
        {
            save = length;
            targetPos = vecP[i];
        }
    }

    return targetPos;
}
