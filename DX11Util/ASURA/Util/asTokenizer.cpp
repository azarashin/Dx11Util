//-----------------------------------------------------------------------
// File : AsuraTokenizer.cpp
// Desc : Tokenizer
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] 新規作成.
// <End>


//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asTokenizer.h"


namespace Asura {
namespace Util {

///-----------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///<param name="size">トークンのサイズ</param>
///-----------------------------------------------------------------------
Tokenizer::Tokenizer( uint32_t size )
{
	mBuffer = null;
	mSeparator.clear();
	mCutOff.clear();
	try 
	{ 
		mToken = new char [size];
	} 
	catch( std::bad_alloc ba )
	{
		std::cerr << "Error : " << ba.what() << "\n";
	}
}

///-----------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///-----------------------------------------------------------------------
Tokenizer::~Tokenizer()
{
	mSeparator.clear();
	mCutOff.clear();
	SAFE_DELETE( mToken );
}

///----------------------------------------------------------------------
///<summary>
///区切り文字を設定する
///</summary>
///<param name="separator">区切り文字として設定する文字</param>
///----------------------------------------------------------------------
void Tokenizer::SetSeparator( const char *separator )
{
	mSeparator = std::string( separator );
}

///-----------------------------------------------------------------------
///<summary>
///切り出し文字を設定する
///</summary>
///<param name="cutfoff">単体トークンとして切り出す文字</param>
///-----------------------------------------------------------------------
void Tokenizer::SetCutOff( const char *cutoff )
{
	mCutOff = std::string( cutoff );
}

///----------------------------------------------------------------------
///<summary>
///バッファへのポインタを設定する
///</summary>
///<param name="buffer">文字列バッファへのポインタ</param>
///----------------------------------------------------------------------
void Tokenizer::SetBuffer( char *buffer )
{
	mBuffer = buffer;
}

///----------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///----------------------------------------------------------------------
void Tokenizer::Release()
{
	mSeparator.clear();
	mCutOff.clear();
	SAFE_DELETE( mToken );
	mBuffer = null;
}

///----------------------------------------------------------------------
///<summary>
///次のトークンを取得する
///</summary>
///----------------------------------------------------------------------
void Tokenizer::Next()
{
	char *p = mBuffer;
	char *q = mToken;

	// 区切り文字はスキップする
	while ( (*p) != '\0' && strchr( mSeparator.c_str(), (*p) ) )
	{
		p++;
	}

	// 切り出し文字とヒットするか判定
	if ( strchr( mCutOff.c_str(), (*p) ) )
	{
		//切り出し文字とヒットしたら，単体トークンとする
		(*(q++)) = (*(p++));
	}
	else
	{
		//区切り文字または切り出し文字以外ならトークンとする
		std::string split = mSeparator + mCutOff;
		while( (*p) != '\0' && !strchr( split.c_str(), (*p) ) )
		{
			(*(q++)) = (*(p++));
		}
	}

	//抜き出した分だけバッファを進める
	mBuffer = p;

	//文字列として返すためにNULL終端文字を加える
	(*q) = '\0';
}

///-----------------------------------------------------------------------
///<summary>
///指定された文字列とトークンが一致しているか判定する
///</summary>
///<param name="token">トークンと比較する文字列</param>
///<returns>
///トークンと一致していたらtrueを返却、そうでないならばfalseを返却
///</returns>
///-----------------------------------------------------------------------
bool Tokenizer::Check( const char *token )
{
	bool result = false;
	int ret = strcmp( mToken, token ); 
	if ( ret == 0 )
	{
		result = true;
	}
	return result;
}

///-----------------------------------------------------------------------
///<summary>
///終了判定
///</summary>
///<returns>
///バッファがNULL終端文字になった場合、trueを返却.
///バッファへのポインタがNULLの場合，trueを返却.
///上記以外の場合は，falseを返却.
///</returns>
///-----------------------------------------------------------------------
bool Tokenizer::IsEnd()
{
	bool result = false;
	if ( (*mBuffer) == '\0' || mBuffer == null )
	{
		result = true;
	}
	return result;
}

///-----------------------------------------------------------------------
///<summary>
///トークンをchar型として返却する
///</summary>
///<returns>トークン</returns>
///-----------------------------------------------------------------------
char* Tokenizer::GetAsChar()
{
	return mToken;
}

///-----------------------------------------------------------------------
///<summary>
///トークンをdouble型として返却する
///</summary>
///<returns>double型に変換したトークン</returns>
///-----------------------------------------------------------------------
double Tokenizer::GetAsDouble()
{
	return atof( mToken );
}

///-----------------------------------------------------------------------
///<summary>
///トークンをfloat型として返却する
///</summary>
///<returns>float型に変換したトークン</returns>
///-----------------------------------------------------------------------
float Tokenizer::GetAsFloat()
{
	return static_cast<float>( atof( mToken) );
}

///-----------------------------------------------------------------------
///<summary>
///トークンをint型として返却する
///</summary>
///<returns>int型に変換したトークン</returns>
///-----------------------------------------------------------------------
int32_t Tokenizer::GetAsInt()
{
	return atoi( mToken );
}

///-----------------------------------------------------------------------
///<summary>
///次のトークンを取得し，char型として返却する
///</summary>
///<returns>トークン</returns>
///-----------------------------------------------------------------------
char* Tokenizer::GetNextAsChar()
{
	Next();
	return GetAsChar();
}

///-----------------------------------------------------------------------
///<summary>
///次のトークンを取得し，double型として返却する.
///</summary>
///<returns>double型に変換したトークン</returns>
///-----------------------------------------------------------------------
double Tokenizer::GetNextAsDouble()
{
	Next();
	return GetAsDouble();
}

///-----------------------------------------------------------------------
///<summary>
///次のトークンを取得し，float型として返却する.
///</summary>
///<returns>float型に変換したトークン</returns>
///-----------------------------------------------------------------------
float Tokenizer::GetNextAsFloat()
{
	Next();
	return GetAsFloat();
}

///-----------------------------------------------------------------------
///<summary>
///次のトークンを取得し，int型として返却する.
///</summary>
///<returns>int型に変換したトークン</returns>
///-----------------------------------------------------------------------
int32_t Tokenizer::GetNextAsInt()
{
	Next();
	return GetAsInt();
}

} // namespace Util
} // namespace Asura

// End Of File.
