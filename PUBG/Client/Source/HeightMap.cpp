#include "stdafx.h"
#include "HeightMap.h"
#include "EffectMeshRenderer.h"
#include "DirectionalLight.h"

HeightMap::HeightMap()
	: IMap()
    , m_pMesh(NULL)
{
    SetDimension(257);


    D3DXMATRIXA16 matS, matT;
    D3DXMatrixScaling(&matS, 10.0f, 1.0f, 10.0f);
    Load(_T("./Resource/Heightmap/Heightmap.raw"), &matS);
    

    m_matWorld = matS /** matT*/;

    SetSurface();

    Resource()()->GetEffect(
        "./Resource/Heightmap/", 
        "Heightmap.fx")->
        SetTexture(
            "Heightmap_Diffuse_Tex", 
            Resource()()->GetTexture(
                "./Resource/Heightmap/", 
                "Heightmap.jpg"));





    //ray box
    SetRayBox();
    


}


HeightMap::~HeightMap()
{
	SAFE_RELEASE(m_pMesh);
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
			vecVertex[index].p = D3DXVECTOR3(x, static_cast<float>(y), z);
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


    draw(m_RayBox, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}
void HeightMap::draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color)
{
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [this, &color](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m_matWorld);
        pEffect->SetValue("Color", &color, sizeof color);
    },
        [this, &vertices]()
    {
        Device()()->DrawPrimitiveUP(
            D3DPT_LINELIST,
            vertices.size() / 2,
            vertices.data(),
            sizeof vertices.front());
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

void HeightMap::SetRayBox()
{    
    vector<D3DXVECTOR3> vec;
    vec.resize(8);
    //ray Direction box 
    vec[0] = (D3DXVECTOR3(0,        500.0f,      25.7f));	//�»���
    vec[1] = (D3DXVECTOR3(25.7f,    500.0f,     25.7f));	//�����
    vec[2] = (D3DXVECTOR3(0,        500.0f,      0));	//�»���
    vec[3] = (D3DXVECTOR3(25.7f,    500.0f,      0));	//�����
    vec[4] = (D3DXVECTOR3(0,        -1,         25.7f));	//������
    vec[5] = (D3DXVECTOR3(25.7f,    -1,         25.7f));	//������
    vec[6] = (D3DXVECTOR3(0,        -1,         0));	//������
    vec[7] = (D3DXVECTOR3(25.7f,    -1,         0));	//������
    
    
    
    for (int i = 0; i < vec.size(); i++)
    {
        D3DXVec3TransformCoord(&vec[i], &vec[i], &m_matWorld);
    }

    m_RayBox.resize(indices.size());
    for (int i = 0; i < indices.size(); i++)
    {
        m_RayBox[i] = vec[indices[i]];
    }

    vec.clear();
}


//bool HeightMap::CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
//{
//	//Ray		ray = Ray::RayAtWorldSpace(screenX, screenY);
//	//float	minDist = FLT_MAX;
//	//float	intersectionDist;
//	//bool	bIntersect = false;
//
//	//for (int i = 0; i < m_vecSurfaceVertex.size(); i += 3)
//	//{
//	//	if ( ray.CalcIntersectTri(&m_vecSurfaceVertex[i], &intersectionDist) )
//	//	{
//	//		if (intersectionDist < minDist)
//	//		{
//	//			bIntersect = true;
//	//			minDist = intersectionDist;
//	//			vOut = ray.m_pos + ray.m_dir * intersectionDist;
//	//		}
//	//	}
//	//}
//	//return bIntersect;
//    return false;
//}

