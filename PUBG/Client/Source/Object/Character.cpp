#include "stdafx.h"

void Character::RifleShootingTest() //bullet 객체에 대한
{
    //Screen에서 쏜 Ray 방향으로 cell들이 담긴다.
    IScene* CS = CurrentScene()();
    Ray ray = CS->m_RayArea.CreateRayArea();


    //1.terrain과의 충돌
    auto tfs(CS->m_RayArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        TerrainFeature* p = tf;
       
        // 먼저 terrain features의 바운딩스피어와 충돌을 검사한다.
        //BoundingSphere bs = tf->GetBoundingSphere();



    }



    //2. character와의 충돌
    auto chrs(CS->m_RayArea.GetCharacters());
    for (auto chr : chrs)
    {
        // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
        BoundingSphere bs = chr->GetBoundingSphere();

        if (!D3DXSphereBoundProbe(
            &(bs.center + bs.position),
            bs.radius,
            &ray.m_pos,
            &ray.m_dir)) continue;

        int i = 0;

    }





}