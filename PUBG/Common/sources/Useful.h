#pragma once

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = nullptr; }}

template<typename T>
type_index GetTypeIndex()
{
	return type_index(typeid(T));
}
