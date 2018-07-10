#include "stdafx.h"
#include "TerrainFeature.h" //이거 지우삼

void Character::RifleShootingTest() //bullet 객체에 대한
{



}

D3DXVECTOR3 Character::FindShootingTargetPos()
{
    //이 함수는 가장 가까운 Object를 찾아서 

    //Screen에서 쏜 Ray 방향으로 cell들이 담긴다.
    IScene* CS = CurrentScene()();
    Ray ray = CS->m_RayArea.CreateRayArea();

    vector<D3DXVECTOR3> vecP; //이곳에서 가장 가까운 pos를 구할 것이다

                              //1.terrain과의 충돌
    auto tfs(CS->m_RayArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        // 먼저 terrain features의 바운딩스피어와 충돌을 검사한다.
        BoundingSphere bs = tf->GetBoundingSphere();

        if (!D3DXSphereBoundProbe(
            &(bs.center + bs.position),
            bs.radius,
            &ray.m_pos,
            &ray.m_dir)) continue;


        // 물체의 바운딩박스들과 충돌을 검사한다.
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

    ////2. character와의 충돌
    //auto chrs(CS->m_RayArea.GetCharacters());
    //for (auto chr : chrs)
    //{
    //    // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
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
