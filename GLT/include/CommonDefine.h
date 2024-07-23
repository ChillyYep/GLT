#pragma once
#include <GL/glcorearb.h>
#include <GL/gl3w.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Enums.h"

#define COMPONENT_CLASS(CLASSTYPE,CLASSTYPEENUM)\
class CLASSTYPE; \
template<>	\
struct component_traits<CLASSTYPE> \
{	\
	static const ComponentType value = CLASSTYPEENUM;	\
};	\
class CLASSTYPE :public Component	\


#define DERIVED_COMPONENT_CLASS(CLASSTYPE,BASECLASSTYPE,CLASSTYPEENUM)\
class CLASSTYPE; \
template<>	\
struct component_traits<CLASSTYPE> \
{	\
	static const ComponentType value = CLASSTYPEENUM;	\
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


size_t OpenFile(const char* directory, const char* filename, char*& content);

std::string OpenFile(const char* directory, const char* filename);

inline size_t OpenShaderFile(const char* shaderName, char*& content) { return OpenFile(SHADER_ROOT, shaderName, content); }