#include "stdafx.h"
#include "Useful.h"

const D3DXVECTOR3& Vector3::ZERO    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3& Vector3::ONE     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
const D3DXVECTOR3& Vector3::RIGHT   = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
const D3DXVECTOR3& Vector3::UP      = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3& Vector3::FORWARD = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

const D3DXQUATERNION& Quaternion::ZERO = 
    D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

const D3DXMATRIX& Matrix::IDENTITY = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 0.0f, 1.0f);

ostream& operator<<(ostream& os, const D3DXVECTOR3& val)
{
	os << val.x << ", " << val.y << ", " << val.z;
	return os;
}

ostream& operator<<(ostream& os, const D3DXMATRIX& val)
{
    os << val._11 << ", " << val._12 << ", " << val._13 << ", " << val._14 
       << '\n' 
       << val._21 << ", " << val._22 << ", " << val._23 << ", " << val._24 
       << '\n'
       << val._31 << ", " << val._32 << ", " << val._33 << ", " << val._34 
       << '\n'
       << val._41 << ", " << val._42 << ", " << val._43 << ", " << val._44;
    return os;
}

ostream& operator<<(ostream& os, const D3DXQUATERNION& val)
{
    os << val.x << ", " << val.y << ", " << val.z << ", " << val.w;
    return os;
}

D3DXVECTOR3 Matrix::GetTranslation(const D3DXMATRIX& val)
{
    return D3DXVECTOR3(val._41, val._42, val._43);
}

void Matrix::GetScaleAndRotation(const D3DXMATRIX& val, D3DXVECTOR3* OutScale, D3DXQUATERNION* OutRotation)
{
    const D3DXVECTOR3 s(
        D3DXVec3Length(&D3DXVECTOR3(val._11, val._12, val._13)),
        D3DXVec3Length(&D3DXVECTOR3(val._21, val._22, val._23)),
        D3DXVec3Length(&D3DXVECTOR3(val._31, val._32, val._33)));

    if (OutScale)
        *OutScale = s;

    if (OutRotation)
    {
        D3DXQuaternionRotationMatrix(
            OutRotation, 
            &D3DXMATRIX(
                val._11 / s.x, val._12 / s.x, val._13 / s.x, 0.0f,
                val._21 / s.y, val._22 / s.y, val._23 / s.y, 0.0f,
                val._31 / s.z, val._32 / s.z, val._33 / s.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f));
    }
}

D3DMATERIAL9 MaterialTemplate::GetWhite()
{
    D3DMATERIAL9 m;
    m.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m.Power = 20.0f;
    return m;
}


D3DXVECTOR3 Vector3::Rotate(const D3DXVECTOR3& v, const D3DXQUATERNION& q)
{
    D3DXQUATERNION conjugate, result;
    D3DXQuaternionConjugate(&conjugate, &q);
    result = q * D3DXQUATERNION(v.x, v.y, v.z, 1.0f) * conjugate;
    return D3DXVECTOR3(result.x, result.y, result.z);
}
