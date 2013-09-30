//-----------------------------------------------------------------------
// File : AsuraTokenizer.cpp
// Desc : Tokenizer
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] �V�K�쐬.
// <End>


//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asTokenizer.h"


namespace Asura {
namespace Util {

///-----------------------------------------------------------------------
///<summary>
///�R���X�g���N�^
///</summary>
///<param name="size">�g�[�N���̃T�C�Y</param>
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
///�f�X�g���N�^
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
///��؂蕶����ݒ肷��
///</summary>
///<param name="separator">��؂蕶���Ƃ��Đݒ肷�镶��</param>
///----------------------------------------------------------------------
void Tokenizer::SetSeparator( const char *separator )
{
	mSeparator = std::string( separator );
}

///-----------------------------------------------------------------------
///<summary>
///�؂�o��������ݒ肷��
///</summary>
///<param name="cutfoff">�P�̃g�[�N���Ƃ��Đ؂�o������</param>
///-----------------------------------------------------------------------
void Tokenizer::SetCutOff( const char *cutoff )
{
	mCutOff = std::string( cutoff );
}

///----------------------------------------------------------------------
///<summary>
///�o�b�t�@�ւ̃|�C���^��ݒ肷��
///</summary>
///<param name="buffer">������o�b�t�@�ւ̃|�C���^</param>
///----------------------------------------------------------------------
void Tokenizer::SetBuffer( char *buffer )
{
	mBuffer = buffer;
}

///----------------------------------------------------------------------
///<summary>
///�������������
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
///���̃g�[�N�����擾����
///</summary>
///----------------------------------------------------------------------
void Tokenizer::Next()
{
	char *p = mBuffer;
	char *q = mToken;

	// ��؂蕶���̓X�L�b�v����
	while ( (*p) != '\0' && strchr( mSeparator.c_str(), (*p) ) )
	{
		p++;
	}

	// �؂�o�������ƃq�b�g���邩����
	if ( strchr( mCutOff.c_str(), (*p) ) )
	{
		//�؂�o�������ƃq�b�g������C�P�̃g�[�N���Ƃ���
		(*(q++)) = (*(p++));
	}
	else
	{
		//��؂蕶���܂��͐؂�o�������ȊO�Ȃ�g�[�N���Ƃ���
		std::string split = mSeparator + mCutOff;
		while( (*p) != '\0' && !strchr( split.c_str(), (*p) ) )
		{
			(*(q++)) = (*(p++));
		}
	}

	//�����o�����������o�b�t�@��i�߂�
	mBuffer = p;

	//������Ƃ��ĕԂ����߂�NULL�I�[������������
	(*q) = '\0';
}

///-----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ������ƃg�[�N������v���Ă��邩���肷��
///</summary>
///<param name="token">�g�[�N���Ɣ�r���镶����</param>
///<returns>
///�g�[�N���ƈ�v���Ă�����true��ԋp�A�����łȂ��Ȃ��false��ԋp
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
///�I������
///</summary>
///<returns>
///�o�b�t�@��NULL�I�[�����ɂȂ����ꍇ�Atrue��ԋp.
///�o�b�t�@�ւ̃|�C���^��NULL�̏ꍇ�Ctrue��ԋp.
///��L�ȊO�̏ꍇ�́Cfalse��ԋp.
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
///�g�[�N����char�^�Ƃ��ĕԋp����
///</summary>
///<returns>�g�[�N��</returns>
///-----------------------------------------------------------------------
char* Tokenizer::GetAsChar()
{
	return mToken;
}

///-----------------------------------------------------------------------
///<summary>
///�g�[�N����double�^�Ƃ��ĕԋp����
///</summary>
///<returns>double�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
double Tokenizer::GetAsDouble()
{
	return atof( mToken );
}

///-----------------------------------------------------------------------
///<summary>
///�g�[�N����float�^�Ƃ��ĕԋp����
///</summary>
///<returns>float�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
float Tokenizer::GetAsFloat()
{
	return static_cast<float>( atof( mToken) );
}

///-----------------------------------------------------------------------
///<summary>
///�g�[�N����int�^�Ƃ��ĕԋp����
///</summary>
///<returns>int�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
int32_t Tokenizer::GetAsInt()
{
	return atoi( mToken );
}

///-----------------------------------------------------------------------
///<summary>
///���̃g�[�N�����擾���Cchar�^�Ƃ��ĕԋp����
///</summary>
///<returns>�g�[�N��</returns>
///-----------------------------------------------------------------------
char* Tokenizer::GetNextAsChar()
{
	Next();
	return GetAsChar();
}

///-----------------------------------------------------------------------
///<summary>
///���̃g�[�N�����擾���Cdouble�^�Ƃ��ĕԋp����.
///</summary>
///<returns>double�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
double Tokenizer::GetNextAsDouble()
{
	Next();
	return GetAsDouble();
}

///-----------------------------------------------------------------------
///<summary>
///���̃g�[�N�����擾���Cfloat�^�Ƃ��ĕԋp����.
///</summary>
///<returns>float�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
float Tokenizer::GetNextAsFloat()
{
	Next();
	return GetAsFloat();
}

///-----------------------------------------------------------------------
///<summary>
///���̃g�[�N�����擾���Cint�^�Ƃ��ĕԋp����.
///</summary>
///<returns>int�^�ɕϊ������g�[�N��</returns>
///-----------------------------------------------------------------------
int32_t Tokenizer::GetNextAsInt()
{
	Next();
	return GetAsInt();
}

} // namespace Util
} // namespace Asura

// End Of File.
