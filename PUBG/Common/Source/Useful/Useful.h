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

struct Matrix
{
    static const D3DXMATRIX& IDENTITY;

    static D3DXVECTOR3 GetTranslation(const D3DXMATRIX& val);
};

struct MaterialTemplate
{
    static D3DMATERIAL9 GetWhite();
};