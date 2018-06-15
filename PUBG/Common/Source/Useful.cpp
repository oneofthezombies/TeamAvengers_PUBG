#include "stdafx.h"
#include "Useful.h"

ostream& operator<<(ostream& os, const D3DXVECTOR3& val)
{
	os << "[ " << val.x << ", " << val.y << ", " << val.z << " ]";
	return os;
}
