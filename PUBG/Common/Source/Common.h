#pragma once

#include "targetver.h"

// windows headers
// exclude rarely used content from the Windows header.
#define WIN32_LEAN_AND_MEAN
// for std::numeric_limits<T>::max() and min(), windows MIN and MAX macro is ambiguous with this methods
#define NOMINMAX    
#include <windows.h>
// end windows headers

// c runtime headers
#include <tchar.h>
#include <atlstr.h>
#include <cstdlib>
#include <cassert>
#include <ctime>
// end c runtime headers

// STLs
#include <thread>
#include <future>

#include <fstream>
using std::fstream;

#include <chrono>
using std::chrono::system_clock;
using std::chrono::duration;

#include <iterator>
using std::advance;
using std::next;
using std::prev;

#include <iostream>
using std::cout;
using std::endl;

#include <limits>
using std::numeric_limits;

#include <utility>
using std::enable_shared_from_this;
using std::make_pair;
using std::pair;

#include <type_traits>
using std::is_base_of_v;
using std::is_pointer_v;
using std::enable_if_t;
using std::move;

#include <functional>
using namespace std::placeholders;
using std::function;
using std::hash;
using std::bind;

#include <typeindex>
using std::type_index;

#include <ostream>
using std::ostream;

#include <memory>
using std::make_shared;
using std::shared_ptr;

#include <sstream>
using std::stringstream;

#include <iomanip>
using std::setw;
using std::setfill;

// STL data structures
#include <unordered_map>
using std::unordered_multimap;
using std::unordered_map;

#include <unordered_set>
using std::unordered_multiset;
using std::unordered_set;

#include <string>
using std::to_string;
using std::string;

#include <bitset>
using std::bitset;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <array>
using std::array;

#include <list>
using std::list;

#include <map>
using std::map;

#include <set>
using std::set;
// end STL data structures
// end STLs

// boost
#include "boost/asio.hpp"
using boost::asio::ip::tcp;
// end boost

// directx9
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
// end directx9

// math and usefuls
#include "MemoryAllocator.h"
#include "Useful.h"
// math and usefuls
