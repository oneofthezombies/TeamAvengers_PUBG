#include "stdafx.h"

void Character::RifleShootingTest() //bullet ��ü�� ����
{
    //Screen���� �� Ray �������� cell���� ����.
    IScene* CS = CurrentScene()();
    Ray ray = CS->m_RayArea.CreateRayArea();


    //1.terrain���� �浹
    auto tfs(CS->m_RayArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        TerrainFeature* p = tf;
       
        // ���� terrain features�� �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
        //BoundingSphere bs = tf->GetBoundingSphere();



    }



    //2. character���� �浹
    auto chrs(CS->m_RayArea.GetCharacters());
    for (auto chr : chrs)
    {
        // ���� ĳ������ �ٿ�����Ǿ�� �浹�� �˻��Ѵ�.
        BoundingSphere bs = chr->GetBoundingSphere();

        if (!D3DXSphereBoundProbe(
            &(bs.center + bs.position),
            bs.radius,
            &ray.m_pos,
            &ray.m_dir)) continue;

        int i = 0;

    }





}