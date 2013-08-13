//-----------------------------------------------------------------------
// File : AsDef.h
// Desc : Asura Data Type Definitions
// Copyright(c) 2010 Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] êVãKçÏê¨.
// <End>

#ifndef __ASURA_DEF_H__
#define __ASURA_DEF_H__

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include <new>
#include <memory>

//-----------------------------------------------------------------------
// Data Definition
//-----------------------------------------------------------------------
#ifndef _STDINT
typedef unsigned char          uchar_t;
typedef unsigned char          uint8_t;
typedef unsigned short         uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long     uint64_t;
typedef char                   int8_t;
typedef short                  int16_t;
typedef int                    int32_t;
typedef long long              int64_t;
typedef float                  fp32_t;
typedef double                 fp64_t;
#endif

#ifndef AS_TCHAR_DEFINE
#define AS_TCHAR_DEFINE 1
#ifdef UNICODE
typedef wchar_t				   tchar;
#else
typedef char				   tchar;
#endif
#endif//AS_TCHAR_DEFINE

//-----------------------------------------------------------------------
// null Definition
//-----------------------------------------------------------------------
const class 
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	/* NOTHING */

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	template<class T>            operator T* () const { return 0; }
	template<class T0, class T1> operator T1 T0::*() const { return 0; }

protected:
	//-------------------------------------------------------------------
	// protected variables
	//-------------------------------------------------------------------
	/* NOTHING */

	//-------------------------------------------------------------------
	// protected methods
	//-------------------------------------------------------------------
	/* NOTHING */

private:
	//-------------------------------------------------------------------
	// private variables
	//-------------------------------------------------------------------
	/* NOTHING */

	//-------------------------------------------------------------------
	// private methods
	//-------------------------------------------------------------------
	void operator& () const {}
} null;


//-----------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) { delete (x); (x)=null; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete[] (x); (x)=null; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) { (x)->Release(); (x)=null; } }
#endif

#ifndef DLOG
#if defined(DEBUG) || defined(_DEBUG)
#define DLOG(x, ...) std::printf( "[File: %s, Line: %d] "x"\n", __FILE__, __LINE__, ##__VA_ARGS__ );
#else
#define DLOG(x, ...)
#endif
#endif

#ifndef ILOG
#define ILOG(x, ...) std::printf( x"\n", ##__VA_ARGS__ );
#endif

#ifndef ELOG
#define ELOG(x, ...) std::printf( "[File: %s, Line: %d] "x"\n", __FILE__, __LINE__, ##__VA_ARGS__ );
#endif

#endif //__ASURA_DEF_H__