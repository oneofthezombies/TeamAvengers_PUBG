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