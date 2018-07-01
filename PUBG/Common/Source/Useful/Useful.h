#pragma once

#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = nullptr; }}
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; }}

template<typename T>
type_index GetTypeIndex()
{
	return type_index(typeid(T));
}

ostream& operator<<(ostream& os, const D3DXVECTOR3& val);
ostream& operator<<(ostream& os, const D3DXMATRIX& val);
ostream& operator<<(ostream& os, const D3DXQUATERNION& val);

struct Vector3
{
    static const D3DXVECTOR3& ZERO;
    static const D3DXVECTOR3& ONE;
    static const D3DXVECTOR3& UP;
    static const D3DXVECTOR3& DOWN;
    static const D3DXVECTOR3& RIGHT;
    static const D3DXVECTOR3& LEFT;
    static const D3DXVECTOR3& FORWARD;
    static const D3DXVECTOR3& BACKWARD;

    static const D3DXVECTOR3& FORWARD_RIGNT;
    static const D3DXVECTOR3& FORWARD_LEFT;
    static const D3DXVECTOR3& BACKWARD_RIGHT;
    static const D3DXVECTOR3& BACKWARD_LEFT;


    static D3DXVECTOR3 Rotate(const D3DXVECTOR3& v, const D3DXQUATERNION& q);
};

struct Quaternion
{
    static const D3DXQUATERNION& ZERO;
};

struct Matrix
{
    static const D3DXMATRIX& IDENTITY;

    static D3DXVECTOR3 GetTranslation(const D3DXMATRIX& val);
    static void GetScaleAndRotation(const D3DXMATRIX& val, D3DXVECTOR3* OutScale, D3DXQUATERNION* OutRotation);
};

struct MaterialTemplate
{
    static D3DMATERIAL9 GetWhite();
};