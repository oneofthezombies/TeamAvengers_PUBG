#include "stdafx.h"
#include "HeightMap.h"
#include "EffectMeshRenderer.h"
#include "Light.h"
#include "Collider.h"

const float HeightMap::RAY_OFFSET_Y = 20.0f;

HeightMap::HeightMap()
	: IMap()
    , m_pMesh(NULL)
{
    //////------------------HeightMap------------------
    //////old map
    //D3DXMATRIXA16 matS, matT;
    //D3DXMatrixScaling(&matS, 30.0f, 1.0f, 30.0f);
    //D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
    //m_matWorld = matS * matT;

    //SetDimension(257);
    //Load(_T("./Resource/Heightmap/Heightmap.raw"), &m_matWorld);

    //SetSurface();

    //Resource()()->GetEffect(
    //    "./Resource/Heightmap/", 
    //    "Heightmap.fx")->
    //    SetTexture(
    //        "Heightmap_Diffuse_Tex", 
    //        Resource()()->GetTexture(
    //            "./Resource/Heightmap/", 
    //            "Heightmap.jpg"));
    //////---------------------------------------------------------



    ////new map-------------------------------------------
    D3DXMATRIXA16 matS, matT;
    D3DXMatrixScaling(&matS, 100.0f, 50.0f, 100.0f);
    D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
    m_matWorld = matS * matT;

    SetDimension(256);
    Load(_T("./Resource/Heightmap/testing/HeightMap_128.raw"), &m_matWorld);
    SetMoreHigherFloor();




    Resource()()->GetEffect(
        "./Resource/Heightmap/",
        "Heightmap.fx")->
        SetTexture(
            "Heightmap_Diffuse_Tex",
            //Resource()()->GetTexture(
            //    "./Resource/Heightmap/",
            //    "Heightmap.jpg"));
            //Resource()()->GetTexture(
            //    "./Resource/Heightmap/",
            //    "ground.png"));
            Resource()()->GetTexture(
                "./Resource/Heightmap/",
                "groundtest_d_8192.png"));
    ////---------------------------------------------------





    //ray box
    SetRayBox();

}


HeightMap::~HeightMap()
{
	SAFE_RELEASE(m_pMesh);
    //SAFE_RELEASE(m_testSphereMesh);
}

vector<WORD> indices = {
    4,0,1,
    4,1,5,
    5,1,3,
    5,3,7,
    6,2,0,
    6,0,4,
    7,3,2,
    7,2,6,
    0,2,3,
    0,3,1,
    6,4,5,
    6,5,7
};

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
            float fX = static_cast<float>(x);
            float fZ = static_cast<float>(z);
			vecVertex[index].p = D3DXVECTOR3(fX, static_cast<float>(y), fZ);
			vecVertex[index].n = D3DXVECTOR3(0, 1, 0);
			vecVertex[index].t = D3DXVECTOR2(fX / (float)m_numTile, fZ / (float)m_numTile);

            //static const float d = 64.0f;
            //const float newNumTile = static_cast<float>(m_numTile) / d;
            //float newX = fX;
            //while (newX > newNumTile)
            //    newX -= newNumTile;
            //float newZ = fZ;
            //while (newZ > newNumTile)
            //    newZ -= newNumTile;
            //vecVertex[index].t = D3DXVECTOR2(newX / newNumTile, newZ / newNumTile);

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

	HRESULT hr = D3DXCreateMeshFVF(
        static_cast<DWORD>(vecIndex.size() / 3), 
        static_cast<DWORD>(vecVertex.size()), 
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
            DWORD index = 
                static_cast<DWORD>(
                    z / (float)numSurfaceTile * m_numTile * m_dimension
                + x / (float)numSurfaceTile * m_numTile);
            vecPos.push_back(m_vecVertex[index]);
        }
    }

    vecIndex.reserve(numSurfaceTile * numSurfaceTile * 2 * 3);
    for (size_t z = 0; z < numSurfaceTile; ++z)
    {
        for (size_t x = 0; x < numSurfaceTile; ++x)
        {
            DWORD _0 = static_cast<DWORD>((z + 0) * surfaceDim + x + 0);
            DWORD _1 = static_cast<DWORD>((z + 1) * surfaceDim + x + 0);
            DWORD _2 = static_cast<DWORD>((z + 0) * surfaceDim + x + 1);
            DWORD _3 = static_cast<DWORD>((z + 1) * surfaceDim + x + 1);

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

//AddHeightVertex(
//    D3DXVECTOR3(,,),
//    D3DXVECTOR3(,,),
//    D3DXVECTOR3(,,),
//    D3DXVECTOR3(,,);
void HeightMap::SetMoreHigherFloor()
{
    // Town Houses 
    //House1 
    {
        //low stairs
        AddHeightVertex(D3DXVECTOR3(7208.211f, 4551.991f, 4935.474f), D3DXVECTOR3(7204.964f, 4649.274f, 4839.107f),
            D3DXVECTOR3(7059.049f, 4649.274f, 4839.107f), D3DXVECTOR3(7062.781f, 4551.991f, 4935.474f));
        //middle flat
        AddHeightVertex(D3DXVECTOR3(7224.780f, 4649.274f, 4839.107f), D3DXVECTOR3(7224.780f, 4649.274f, 4698.931f),
            D3DXVECTOR3(7059.049f, 4649.274f, 4698.297f), D3DXVECTOR3(7059.049f, 4649.274f, 4839.107f));
        //up stairs
        AddHeightVertex(D3DXVECTOR3(7217.125f, 4649.274f, 4841.938f), D3DXVECTOR3(7528.707f, 4950.964f, 4841.938f),
            D3DXVECTOR3(7528.707f, 4950.964f, 4701.871f), D3DXVECTOR3(7217.125f, 4649.274f, 4699.952f));
        //second floor + balcony
        AddHeightVertex(D3DXVECTOR3(7528.285f, 4950.964f, 5258.534f), D3DXVECTOR3(8955.188f, 4950.964f, 5245.071f),
            D3DXVECTOR3(8955.188f, 4950.964f, 3911.407f), D3DXVECTOR3(7528.285f, 4950.964f, 3916.068f));
        //second floor back
        AddHeightVertex(D3DXVECTOR3(7551.703f, 4948.814f, 4698.931f), D3DXVECTOR3(7551.703f, 4948.814f, 3905.567f),
            D3DXVECTOR3(7034.993f, 4948.814f, 3905.567f), D3DXVECTOR3(7030.984f, 4948.814f, 4698.931f));

    }

    //House2
    //low stair
    //AddHeightVertex(D3DXVECTOR3(9064.158f, 4551.790f, 6206.691f), D3DXVECTOR3(, , ),
    //    D3DXVECTOR3(, , ), D3DXVECTOR3(9064.158f, 4551.790f, 6061.329f));


    //warehouse_A
    {
        //east stairs
        AddHeightVertex(
            D3DXVECTOR3(9430.240f, 4748.771f, 16836.355f),
            D3DXVECTOR3(9034.330f, 4774.442f, 16836.355f),
            D3DXVECTOR3(9023.568f, 4774.442f, 17664.314f),
            D3DXVECTOR3(9426.085f, 4746.271f, 17659.828f));
        //west stairs
        AddHeightVertex(
            D3DXVECTOR3(5296.136f, 4748.069f, 17661.768f),
            D3DXVECTOR3(5694.028f, 4777.353f, 17661.768f),
            D3DXVECTOR3(5694.028f, 4777.353f, 16832.848f),
            D3DXVECTOR3(5296.136f, 4748.069f, 16836.340f));

        //main floor
        AddHeightVertex(
            D3DXVECTOR3(5690.938f, 4777.353f, 18266.002f),
            D3DXVECTOR3(9044.663f, 4777.353f, 18266.002f),
            D3DXVECTOR3(9044.663f, 4777.353f, 16234.355f),
            D3DXVECTOR3(5690.938f, 4777.353f, 16236.205f));
    }


    // Church
    {
        // first stair (from earth)
        AddHeightVertex(
            D3DXVECTOR3(12393.421f, 5794.864f, 10823.303f),
            D3DXVECTOR3(12393.421f, 6044.047f, 11246.786f),
            D3DXVECTOR3(12582.898f, 6044.047f, 11246.786f),
            D3DXVECTOR3(12573.668f, 5798.094f, 10817.962f));

        // first floor
        AddHeightVertex(
            D3DXVECTOR3(12208.354f, 6035.410f, 11221.958f),
            D3DXVECTOR3(12208.354f, 6042.187f, 11400.729f),
            D3DXVECTOR3(12586.079f, 6042.187f, 11400.729f),
            D3DXVECTOR3(12586.079f, 6042.187f, 11216.418f));

        // second stair
        AddHeightVertex(
            D3DXVECTOR3(12390.408f, 6030.919f, 11241.337f),
            D3DXVECTOR3(12392.792f, 6267.627f, 10860.087f),
            D3DXVECTOR3(12206.996f, 6267.627f, 10860.087f),
            D3DXVECTOR3(12206.996f, 6029.039f, 11250.622f));

        // second floor
        AddHeightVertex(
            D3DXVECTOR3(12212.496f, 6259.917f, 10681.529f),
            D3DXVECTOR3(12212.496f, 6259.917f, 10871.830f),
            D3DXVECTOR3(12590.355f, 6259.917f, 10871.830f),
            D3DXVECTOR3(12586.525f, 6265.406f, 10686.784f));

        // Third stair
        AddHeightVertex(
            D3DXVECTOR3(12383.309, 6259.917f, 10871.830f),
            D3DXVECTOR3(12377.020f, 6476.385f, 11228.501f),
            D3DXVECTOR3(12590.355f, 6476.385f, 11228.501f),
            D3DXVECTOR3(12590.355f, 6259.917f, 10871.830f));

        // Third floor
        AddHeightVertex(
            D3DXVECTOR3(12377.020f, 6476.385f, 11228.501f),
            D3DXVECTOR3(12377.020f, 6476.385f, 11569.791f),
            D3DXVECTOR3(12572.840f, 6476.385f, 11569.791f),
            D3DXVECTOR3(12590.355f, 6476.385f, 11228.501f));

        //roof left
        AddHeightVertex(
            D3DXVECTOR3(12203.550f, 6404.667f, 11377.509f),
            D3DXVECTOR3(12203.550f, 6404.667f, 13249.753f),
            D3DXVECTOR3(12972.576f, 6577.479f, 13249.753f),
            D3DXVECTOR3(12972.576f, 6577.479f, 11365.938f));

        //roof right
        AddHeightVertex(
            D3DXVECTOR3(12972.576f, 6577.479f, 11365.938f),
            D3DXVECTOR3(12972.576f, 6577.479f, 13249.753f),
            D3DXVECTOR3(13792.662f, 6396.372f, 13353.286f),
            D3DXVECTOR3(13792.662f, 6396.372f, 11391.755f));

    }


    //AddHeightVertex(D3DXVECTOR3(12209.882f, 6043.665f, 11399.259f),D3DXVECTOR3(12209.882f, 6043.665f, 11399.259f), D3DXVECTOR3(12568.783f, 6043.665f, 11399.259f), D3DXVECTOR3(12568.783f, 6043.665f, 11399.259f));

    

    //testing
    //AddHeightVertex(D3DXVECTOR3(5000.0f, 4650.000, 5000.0f), D3DXVECTOR3(5000.0f, 4650.000, 6000.0f), D3DXVECTOR3(6000.0f, 4650.000, 6000.0f), D3DXVECTOR3(6000.0f, 4650.000, 5000.0f));

}

void HeightMap::AddHeightVertex(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 pos4)
{
    m_vecAddSurfaceVertex.push_back(pos1);
    m_vecAddSurfaceVertex.push_back(pos2);
    m_vecAddSurfaceVertex.push_back(pos3);

    m_vecAddSurfaceVertex.push_back(pos1);
    m_vecAddSurfaceVertex.push_back(pos3);
    m_vecAddSurfaceVertex.push_back(pos4);
}

void HeightMap::OnUpdate()
{
}
void HeightMap::OnRender()
{
    //Draw HeightMap
    {
        Shader::Draw(
            Resource()()->GetEffect("./Resource/Heightmap/", "Heightmap.fx"),
            nullptr,
            m_pMesh,
            0,
            [this](LPD3DXEFFECT pEffect)
        {
            //pEffect->SetTexture("Heightmap_Normal_Tex", 
            //    Resource()()->GetTexture("./Resource/Heightmap/",
            //    "ground_n.png"));
            pEffect->SetTexture("Heightmap_Normal_Tex",
                Resource()()->GetTexture("./Resource/Heightmap/",
                    "groundtest_n_8192.png"));

            pEffect->SetMatrix(
                Shader::World,
                &GetTransform()->GetTransformationMatrix());
        });

    }
    
    //Draw Boundary Box
    if (Collision()()->IsRender())
    {
        drawIndices(BoxCollider::FRUSTUM_INDICES, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void HeightMap::drawIndices(const vector<WORD>& indices, const D3DXCOLOR & color)
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [this, &color](LPD3DXEFFECT pEffect)
    {
        D3DXMATRIX mat;
        D3DXMatrixIdentity(&mat);
        pEffect->SetMatrix(Shader::World, &mat);
        pEffect->SetValue("Color", &color, sizeof color);
    },
        [this, &indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            sizeof m_boundary / sizeof m_boundary[0],
            static_cast<UINT>(indices.size() / 2),
            indices.data(),
            D3DFMT_INDEX16,
            &m_boundary[0],
            sizeof D3DXVECTOR3);
    });

}


bool HeightMap::GetHeight(const D3DXVECTOR3 & pos,OUT float * OutHeight)
{

	if (pos.x < 0 || pos.z < 0 ||
		pos.x > m_size.x || pos.z > m_size.z)
	{
        *OutHeight = 0.0f;
		return false;
	}
	
    float height1 = 0.0f;
    float height2 = 0.0f;

    //1. height map 자리를 구해서 height를 주는 부분 
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
        height1 = m_vecVertex[_0].y + zY * fDeltaZ + xY * fDeltaX;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;

		float xY = m_vecVertex[_1].y - m_vecVertex[_3].y;
		float zY = m_vecVertex[_2].y - m_vecVertex[_3].y;
        height1 = m_vecVertex[_3].y + xY * fDeltaX + zY * fDeltaZ;
	}


    //2. 추가된 map의 자리와 판결해서 height를 주는 부분
    D3DXVECTOR3 rayPos(pos.x, pos.y + RAY_OFFSET_Y, pos.z);
    D3DXVECTOR3 rayDir(0, -1, 0);
    float distance;

    for (size_t i = 0; i < m_vecAddSurfaceVertex.size(); i += 3)
    {
        if (D3DXIntersectTri(&m_vecAddSurfaceVertex[i], &m_vecAddSurfaceVertex[i + 1], &m_vecAddSurfaceVertex[i + 2],
            &rayPos, &rayDir, NULL, NULL, &distance))
        {
            height2 = rayPos.y - distance;
            break;
        }
    }

    if (height2 > 0.0f)
        *OutHeight = height2;
    else
        *OutHeight = height1;


	return true;
}

LPD3DXMESH HeightMap::GetMesh() const
{
    return m_pMesh;
}


void HeightMap::SetRayBox()
{    

    //ray Direction box 
    m_boundary[0] = (D3DXVECTOR3(0,        800.0f,         257.f));	//좌상후
    m_boundary[1] = (D3DXVECTOR3(257.f,    800.0f,         257.f));	//우상후
    m_boundary[2] = (D3DXVECTOR3(0,        800.0f,         0));	//좌상전
    m_boundary[3] = (D3DXVECTOR3(257.f,    800.0f,         0));	//우상전
    m_boundary[4] = (D3DXVECTOR3(0,        -1,             257.f));	//좌하후
    m_boundary[5] = (D3DXVECTOR3(257.f,    -1,             257.f));	//우하후
    m_boundary[6] = (D3DXVECTOR3(0,        -1,             0));	//좌하전
    m_boundary[7] = (D3DXVECTOR3(257.f,    -1,             0));	//우하전
    
    
    for (std::size_t i = 0; i < 8; i++)
    {
        D3DXVec3TransformCoord(&m_boundary[i], &m_boundary[i], &m_matWorld);
    }

    m_boundaryBox.resize(indices.size());
    for (std::size_t i = 0; i < indices.size(); i++)
    {
        m_boundaryBox[i] = m_boundary[indices[i]];
    }

}
