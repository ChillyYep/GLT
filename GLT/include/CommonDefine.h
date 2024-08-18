#pragma once
#include <assert.h>

using GLTBool = bool;
using GLTUByte = unsigned char;
using GLTSByte = signed char;
using GLTInt16 = signed short;
using GLTUInt16 = unsigned short;
using GLTInt32 = signed int;
using GLTUInt32 = unsigned int;
using GLTInt64 = signed long long int;
using GLTUInt64 = unsigned long long int;
using GLTSize = size_t;
using GLTSizei = GLTInt32;

#define ENUM_BINDING_CLASS(CLASSTYPE,BASECLASSTYPE,ENUMTYPE,CLASSTYPEENUM,TRAITSCLASS) \
class CLASSTYPE; \
template<>	\
struct TRAITSCLASS<CLASSTYPE> \
{	\
	static const ENUMTYPE value = CLASSTYPEENUM;	\
};	\
class CLASSTYPE :public BASECLASSTYPE	\


#define __GET_SET_PROPERTY__(PROPERTYNAME,PROPERTYTYPE,VARIABLE) \
inline void Set##PROPERTYNAME(PROPERTYTYPE params) \
{ \
	VARIABLE = params; \
} \
inline PROPERTYTYPE Get##PROPERTYNAME() const \
{ \
	return VARIABLE; \
} 

#define __GET_SET_BOOLEANPROPERTY__(PROPERTYNAME,VARIABLE) \
inline void Set##PROPERTYNAME(bool params) \
{ \
	VARIABLE = params; \
} \
inline bool Is##PROPERTYNAME() const \
{ \
	return VARIABLE; \
} 

#define __GET_SET_ARRAYPROPERTY__(PROPERTYNAME,PROPERTYTYPE,VARIABLE) \
inline void Set##PROPERTYNAME(PROPERTYTYPE params[]) { VARIABLE = params; } \
inline PROPERTYTYPE* Get##PROPERTYNAME() { return VARIABLE; } \
\

#define BUFFER_OFFSET(offset) ((void*)(offset))

#define MyAssert(condition,msg) \
	if(!condition){ \
		_wassert(msg, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));	\
	}

//#define USE_SPIRV

#ifndef SPRIV_USING
#define SPRIV_USING

#ifdef USE_SPIRV
#define SHADER_ROOT "Shader/SPIRV/"
#else
#define SHADER_ROOT "Shader/"
#endif // USE_SPIRV

#endif //SPRIV_USING
