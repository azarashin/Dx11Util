//-----------------------------------------------------------------------
// File : AsuraTokenizer.h
// Desc : Asura Tokenizer
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] êVãKçÏê¨.
// <End>

#ifndef __ASURA_TOKENIZER_H__
#define __ASURA_TOKENIZER_H__

//-----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------- 
#include "asDef.h"
#include <iostream>
#include <cstdlib>
#include <string>


namespace Asura {
namespace Util {

//-----------------------------------------------------------------------
// Tokenizer class
//-----------------------------------------------------------------------
class Tokenizer
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
	Tokenizer( uint32_t size );
	virtual ~Tokenizer();
	void           SetSeparator( const char *separator );
	void           SetCutOff( const char *cutoff );
	void           SetBuffer( char *buffer );
	void           Release();
	void           Next();
	bool           Check( const char *token );
	bool           IsEnd();
	char*          GetAsChar();
	double         GetAsDouble();
	float          GetAsFloat();
	int32_t        GetAsInt();
	char*          GetNextAsChar();
	double         GetNextAsDouble();
	float          GetNextAsFloat();
	int32_t        GetNextAsInt();

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
	char *mBuffer;
	char *mToken;
	std::string mSeparator;
	std::string mCutOff;

	//-------------------------------------------------------------------
	// private methods
	//-------------------------------------------------------------------
	/* NOTHING */
};

} // namespace Util
} // namespace Asura


#endif //__ASURA_TOKENIZER_H__

// End Of File.