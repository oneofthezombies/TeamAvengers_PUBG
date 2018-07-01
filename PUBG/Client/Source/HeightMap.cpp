#include "stdafx.h"
#include "HeightMap.h"
#include "EffectMeshRenderer.h"
#include "DirectionalLight.h"

////#include "Ray.h"
////#include "AStar.h"

//D3DVERTEXELEMENT9 decl[] =
//{
//    { 0, 0, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
//    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//    D3DDECL_END()
//};

HeightMap::HeightMap()
	: IMap()
    , m_pMesh(NULL)
{
    D3DXMATRIXA16 matS;
    D3DXMatrixScaling(&matS, 1.0f, 0.03f, 1.0f);

    SetDimension(257);
    Load(_T("./Resource/Heightmap/Heightmap.raw"), &matS);
    
    //<<색을 입히던데 어떻게 할꺼냐?

    SetSurface();

    Resource()()->GetEffect(
        "./Resource/Heightmap/", 
        "Heightmap.fx")->
        SetTexture(
            "Heightmap_Diffuse_Tex", 
            Resource()()->GetTexture(
                "./Resource/Heightmap/", 
                "Heightmap.jpg"));
}


HeightMap::~HeightMap()
{
	SAFE_RELEASE(m_pMesh);
	//SAFE_DELETE(m_pMtlTex);
	//SAFE_RELEASE(m_pAStar);
}





void HeightMap::Load(LPCTSTR fullPath, D3DXMATRIXA16 * pMat)
{
	vector<VERTEX_PNT> vecVertex;
	vecVertex.resize(m_dimension * m_dimension);
	m_vecVertex.resize(m_dimension * m_dimension);

	std::ifstream fin(fullPath, std::ios::binary);
    if (!fin.is_open())
        assert(false && "std::ifstream fin(fullPath, std::ios::binary); load fail");

	for (int z = 0; z < m_dimension; ++z)
	{
		for (int x = 0; x < m_dimension; ++x)
		{
			int index = z * (m_dimension)+x;
			int y = fin.get();
			vecVertex[index].p = D3DXVECTOR3(x, y, z);
			vecVertex[index].n = D3DXVECTOR3(0, 1, 0);
			vecVertex[index].t = D3DXVECTOR2(x / (float)m_numTile, z / (float)m_numTile);

			if (pMat)
				D3DXVec3TransformCoord(&vecVertex[index].p, &vecVertex[index].p, pMat);

			m_vecVertex[index] = vecVertex[index].p;
		}
	}

	fin.close();

	if (pMat)
	{
		m_scale.x = pMat->_11;
		m_scale.z = pMat->_33;
	}
	m_size.x = m_numTile * m_scale.x;
	m_size.z = m_numTile * m_scale.z;
	
	vector<DWORD> vecIndex;
	vecIndex.reserve(m_numTile * m_numTile * 3 * 2);

	for (int z = 0; z < m_numTile; ++z)
	{
		for (int x = 0; x < m_numTile; ++x)
		{
			// 1--3
			// |\ |
			// | \|
			// 0--2
			int _0 = (z + 0) * m_dimension + x + 0;
			int _1 = (z + 1) * m_dimension + x + 0;
			int _2 = (z + 0) * m_dimension + x + 1;
			int _3 = (z + 1) * m_dimension + x + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);
		}
	}

	for (int z = 1; z < m_numTile; ++z)
	{
		for (int x = 1; x < m_numTile; ++x)
		{
			// ---u---
			// |\ |\ |
			// | \| \|
			// l--n--r
			// |\ |\ |
			// | \| \|
			// ---d---
			int index = z * m_dimension + x;

			int l = index - 1;
			int r = index + 1;
			int u = index + m_dimension;
			int d = index - m_dimension;

			D3DXVECTOR3 du = m_vecVertex[u] - m_vecVertex[d];
			D3DXVECTOR3 lr = m_vecVertex[r] - m_vecVertex[l];
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);

			vecVertex[index].n = n;
		}
	}

	HRESULT hr = D3DXCreateMeshFVF(vecIndex.size() / 3, vecVertex.size(), 
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_PNT::FVF, Device()(), &m_pMesh);

    assert(!FAILED(hr) && "HeightMap::Load(), D3DXCreateMesh() failed.");
	
	VERTEX_PNT* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PNT));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(FLT_EPSILON, &vecAdjBuf[0]);
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &vecAdjBuf[0], 0, 0, 0);
}


void HeightMap::OnUpdate()
{
	//if (Mouse::Get()->ButtonDown(Mouse::RBUTTON))
	//{
	//	D3DXVECTOR3 pos = Mouse::Get()->GetPosition();
	//		
	//	if (CalcPickedPosition(pos, pos.x, pos.y) == true)
	//	{
	//		//GetHeight(pos.y, pos);
	//		static_cast<IUnitObject*>(ObjectManager::Get()->FindObjectByTag(TAG_PLAYER))->SetDestination(pos);
	//	}
	//}
}


void HeightMap::OnRender()
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/Heightmap/", "Heightmap.fx"), 
        nullptr, 
        m_pMesh, 
        0, 
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });

	//DX::GetDevice()->SetRenderState(D3DRS_FOGENABLE, true);
	//DX::GetDevice()->SetRenderState(D3DRS_FOGCOLOR, 0xffcccccc);
	//DX::GetDevice()->SetRenderState(D3DRS_FOGDENSITY, FtoDw(0.1f));
	//DX::GetDevice()->SetRenderState(D3DRS_FOGSTART, FtoDw(20.0f));
	//DX::GetDevice()->SetRenderState(D3DRS_FOGEND, FtoDw(40.0f));
	//DX::GetDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	//DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	//DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	//DX::GetDevice()->SetMaterial(&m_pMtlTex->GetMaterial());
	//DX::GetDevice()->SetTexture(0, m_pMtlTex->GetTexture());
	//m_pMesh->DrawSubset(0);
	//DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//
	//SAFE_RENDER(m_pAStar);
	//
	////PreRender
	//DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	//DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//

	//DX::GetDevice()->SetMaterial(&DXUtil::BLUE_MTRL);
	////DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecSurfaceVertex.size() / 3,
	////	&m_vecSurfaceVertex[0], sizeof(D3DXVECTOR3));
	//DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecObstacleVertex.size() / 3, 
	//	&m_vecObstacleVertex[0], sizeof(D3DXVECTOR3));

	////PostRender
	//DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//DX::GetDevice()->SetRenderState(D3DRS_FOGENABLE, false);
}


bool HeightMap::GetHeight(const D3DXVECTOR3 & pos,OUT float * OutHeight)
{
	if (pos.x < 0 || pos.z < 0 ||
		pos.x > m_size.x || pos.z > m_size.z)
	{
        *OutHeight = 0.0f;
		return false;
	}
	
	// 1--3
	// |\ |
	// | \|
	// 0--2
	int nX = (int)(pos.x / m_scale.x);
	int nZ = (int)(pos.z / m_scale.z);
	float fDeltaX = (pos.x / m_scale.x) - nX;
	float fDeltaZ = (pos.z / m_scale.z) - nZ;
	
	int _0 = (nZ + 0) * m_dimension + nX + 0;
	int _1 = (nZ + 1) * m_dimension + nX + 0;
	int _2 = (nZ + 0) * m_dimension + nX + 1;
	int _3 = (nZ + 1) * m_dimension + nX + 1;

	if (fDeltaX + fDeltaZ < 1.0f)
	{
		float zY = m_vecVertex[_1].y - m_vecVertex[_0].y;
		float xY = m_vecVertex[_2].y - m_vecVertex[_0].y;
        *OutHeight = m_vecVertex[_0].y + zY * fDeltaZ + xY * fDeltaX;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;

		float xY = m_vecVertex[_1].y - m_vecVertex[_3].y;
		float zY = m_vecVertex[_2].y - m_vecVertex[_3].y;
        *OutHeight = m_vecVertex[_3].y + xY * fDeltaX + zY * fDeltaZ;
	}
	return true;
}


bool HeightMap::CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
{
	//Ray		ray = Ray::RayAtWorldSpace(screenX, screenY);
	//float	minDist = FLT_MAX;
	//float	intersectionDist;
	//bool	bIntersect = false;

	//for (int i = 0; i < m_vecSurfaceVertex.size(); i += 3)
	//{
	//	if ( ray.CalcIntersectTri(&m_vecSurfaceVertex[i], &intersectionDist) )
	//	{
	//		if (intersectionDist < minDist)
	//		{
	//			bIntersect = true;
	//			minDist = intersectionDist;
	//			vOut = ray.m_pos + ray.m_dir * intersectionDist;
	//		}
	//	}
	//}
	//return bIntersect;
    return false;
}


void HeightMap::SetSurface()
{
	vector<D3DXVECTOR3>	vecPos;
	vector<DWORD>		vecIndex;
	size_t surfaceDim = 5;
	size_t numSurfaceTile = surfaceDim - 1;

	vecPos.reserve(surfaceDim * surfaceDim);
	for (size_t z = 0; z < surfaceDim; ++z)
	{
		for (size_t x = 0; x < surfaceDim; ++x)
		{
			DWORD index = z / (float)numSurfaceTile * m_numTile * m_dimension
				+ x / (float)numSurfaceTile * m_numTile;
			vecPos.push_back(m_vecVertex[index]);
		}
	}
	
	vecIndex.reserve(numSurfaceTile * numSurfaceTile * 2 * 3);
	for (size_t z = 0; z < numSurfaceTile; ++z)
	{
		for (size_t x = 0; x < numSurfaceTile; ++x)
		{
			DWORD _0 = (z + 0) * surfaceDim + x + 0;
			DWORD _1 = (z + 1) * surfaceDim + x + 0;
			DWORD _2 = (z + 0) * surfaceDim + x + 1;
			DWORD _3 = (z + 1) * surfaceDim + x + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);
		}
	}

	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		m_vecSurfaceVertex.push_back(vecPos[vecIndex[i]]);
	}
}



//void HeightMap::SetMtlTex(D3DMATERIAL9 &mtl, LPDIRECT3DTEXTURE9 pTex)
//{
//	//m_pMtlTex = new MTLTEX;
//	//m_pMtlTex->SetMaterial(mtl);
//	//m_pMtlTex->SetTexture(pTex);
//}


//void HeightMap::SetObstacle()
//{
//	float posX = m_pAStar->GetNodes()[2]->GetLocation().x - 0.7f;
//	float posZ = m_pAStar->GetNodes()[30 * 4 + 2]->GetLocation().z + 0.9f;
//	float height = 6;
//
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, 0));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, 0));
//
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, 0));
//
//	posX += 1.4f;
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, 0));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, 0));
//
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, 3, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, posZ));
//	m_vecObstacleVertex.push_back(D3DXVECTOR3(posX, height, 0));
//}
