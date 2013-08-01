//-----------------------------------------------------------------------
// File : AsMath.h
// Desc : Asura Math
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <--> 2010/11/16 [Pocol] 新規作成.
// <End>

#ifndef __ASURA_MATH_H__
#define __ASURA_MATH_H__

//-----------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------
#ifndef ASURA_INLINE
#if (_MSC_VER >= 1200)
#define ASURA_INLINE __forceinline
#else
#define ASURA_INLINE __inline
#endif
#else
#ifdef __cplusplus
#define ASURA_INLINE inline
#else
#define ASURA_INLINE
#endif
#endif

#ifndef ASURA_TEMPLATE
#define ASURA_TEMPLATE template< typename type >
#endif

#ifndef ASURA_TEMP_INLINE
#define ASURA_TEMP_INLINE ASURA_TEMPLATE ASURA_INLINE
#endif

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include <cmath>
#include <cfloat>
#include <cassert>

namespace Asura {
namespace Math {

//-----------------------------------------------------------------------
// Constant Variables
//-----------------------------------------------------------------------
const float Pi        = 3.1415926535897932384626433832795f;
const float TwoPi     = 6.283185307179586476925286766559f;
const float OneOverPi = 0.31830988618379067153776752674503f;
const float PiOver2   = 1.5707963267948966192313216916398f;
const float PiOver3   = 1.0471975511965977461542144610932f;
const float PiOver4   = 0.78539816339744830961566084581988f;
const float PiOver6   = 0.52359877559829887307710723054658f;
const float F_Epsilon = 1e-4f;
const double D_Epsilon = 1e-6;

//-----------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------

///----------------------------------------------------------------------
///<summary>
///ラジアンへ変換する
///</summary>
///<param="degree">変換する角度の値</param>
///<returns>ラジアンへと変換した結果を返却</returns>
///----------------------------------------------------------------------
ASURA_INLINE
float ToRadian( float degree )
{ return degree * ( Pi / 180.0f ); }

///----------------------------------------------------------------------
///<summary>
///度へ変換する
///</summary>
///<param="radian">変換するラジアンの値</param>
///<returns>度へと変換した結果を返却</returns>
///----------------------------------------------------------------------
ASURA_INLINE
float ToDegree( float radian )
{ return radian * ( 180.0f / Pi ); }

///----------------------------------------------------------------------
///<summary>
///ゼロに近いか確認する
///</summary>
///<param="value">確認したい数値</param>
///<returns>
///ゼロに近ければtrueを返却，そうでなければfalseを返却
///</returns>
///----------------------------------------------------------------------
ASURA_INLINE
bool IsNearZero( float value )
{ return ( -F_Epsilon <= value ) && ( value <= F_Epsilon ); }

///----------------------------------------------------------------------
///<summary>
///ゼロに近いか確認する
///</summary>
///<param="value">確認したい数値</param>
///<returns>
///ゼロに近ければtrueを返却，そうでなければfalseを返却
///</returns>
///----------------------------------------------------------------------
ASURA_INLINE
bool IsNearZero( double value )
{ return ( -D_Epsilon <= value ) && ( value <= D_Epsilon ); }

///----------------------------------------------------------------------
///<summary>
///</summary>
///<param="value1"></param>
///<param="value2"></param>
///<returns></returns>
///----------------------------------------------------------------------
ASURA_INLINE
bool IsNearlyEqual( float value1, float value2 )
{ return ( ( value1 - F_Epsilon ) <= value2 ) && ( value2 <= ( value1 + F_Epsilon ) ); }

///----------------------------------------------------------------------
///<summary>
///</summary>
///<param="value1"></param>
///<param="value2"></param>
///<returns></returns>
///----------------------------------------------------------------------
ASURA_INLINE
bool IsNearlyEqual( double value1, double value2 )
{ return ( ( value1 - D_Epsilon ) <= value2 ) && ( value2 <= ( value1 + D_Epsilon ) ); }

///----------------------------------------------------------------------
///<summary>
///最大値を返却する
///</summary>
///<param="a">比較数値a</param>
///<param="b">比較数値b</param>
///<returns>aとbを比較し、数値の大きいものを返却</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Max( type a, type b )
{ return ( a > b ) ? a : b; }

///----------------------------------------------------------------------
///<summary>
///最小値を返却する
///</summary>
///<param="a">比較数値a</param>
///<param="b">比較数値b</param>
///<returns>aとbを比較し、数値の小さいものを返却</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Min( type a, type b )
{ return ( a < b ) ? a : b; }

///----------------------------------------------------------------------
///<summary>
///数値を指定範囲内にクランプする
///</summary>
///<param="value">クランプしたい数値</param>
///<param="mini">最小値</param>
///<param="maxi">最大値</param>
///<returns>
///最小値から最大値の範囲内にクランプした結果を返却
///</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Clamp( type value, type mini, type maxi )
{ return Max( mini, Min( maxi, value ) ); }

///----------------------------------------------------------------------
///<summary>
///数値を0から1の範囲内にクランプする
///</summary>
///<param="value">クランプしたい数値</param>
///<returns>0から1の範囲内にクランプした結果を返却する</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Saturate( type value )
{ return Clamp( value, static_cast< type >( 0 ), static_cast< type >( 1 ) ); }

///----------------------------------------------------------------------
///<summary>
///符号を取得する
///</summary>
///<param="value">符号を確認したい数値</param>
///<returns>
///値がマイナスであれば-1を返却.
///値がプラスまたはゼロである場合は1を返却する.
///</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Sign( type value )
{ return ( value < 0 ) ? -static_cast< type >( 1 ) : static_cast< type >( 1 ); }

///----------------------------------------------------------------------
///<summary>
///階乗計算を行う
///</summary>
///<param="number">階乗計算する数値n</param>
///<returns>nの階乗(n!)を計算した結果を返却する</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type Fact( int number )
{
	int result = 1;
	for( int i = 1; i <= number; ++i )
	{ result *= i; }
	return static_cast< type >( result );
}

///----------------------------------------------------------------------
///<summary>
///2重階乗を行う
///</summary>
///<param="number">2重階乗計算する数値n</param>
///<returns>nの2重階乗(n!!)を計算した結果を返却する</returns>
///----------------------------------------------------------------------
ASURA_TEMP_INLINE
type DoubleFact( int number )
{
	int result = 1;
	int start = ( number % 2 == 0 ) ?  2 : 1;
	for( int i = start; i <= number; i += 2 )
	{ result *= i; }
	return static_cast< type >( result );
}

/////////////////////////////////////////////////////////////////////////
// Vector2 structure
///////////////////////////////////////////////////////////////////////// 
typedef struct Vector2
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	float x;
	float y;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------

	// constructors
	Vector2();
	Vector2( const float* );
	Vector2( float x, float y );

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector2& operator += ( const Vector2& );
	Vector2& operator -= ( const Vector2& );
	Vector2& operator *= ( float );
	Vector2& operator /= ( float );

	// unary operators
	Vector2  operator + () const;
	Vector2  operator - () const;

	// binary operators
	Vector2  operator + ( Vector2 ) const;
	Vector2  operator - ( Vector2 ) const;
	Vector2  operator * ( float ) const;
	Vector2  operator / ( float ) const;
	bool     operator == ( const Vector2& ) const;
	bool     operator != ( const Vector2& ) const;
	friend Vector2 operator* ( float, const Vector2& );

	// Length and Normalize Methods
	float   Length() const;
	float   LengthSquared() const;
	Vector2& Normalize();

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
	/* NOTHING */
} Vector2;

/////////////////////////////////////////////////////////////////////////
// Vector3 structure
/////////////////////////////////////////////////////////////////////////
typedef struct Vector3
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	float x;
	float y;
	float z;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	
	// constructors
	Vector3();
	Vector3( const float * );
	Vector3( Vector2& vec, float z );
	Vector3( float x, float y, float z );

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector3& operator += ( const Vector3& );
	Vector3& operator -= ( const Vector3& );
	Vector3& operator *= ( float );
	Vector3& operator /= ( float );

	// unary operators
	Vector3  operator + () const;
	Vector3  operator - () const;

	// binary operators
	Vector3  operator + ( Vector3 ) const;
	Vector3  operator - ( Vector3 ) const;
	Vector3  operator * ( float ) const;
	Vector3  operator / ( float ) const;
	bool    operator == ( const Vector3& ) const;
	bool    operator != ( const Vector3& ) const;
	friend Vector3 operator * ( float, const Vector3& );

	// Length and Normalize methods
	float   Length() const;
	float   LengthSquared() const;
	Vector3& Normalize();

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
	/* NOTHING */
} Vector3;

/////////////////////////////////////////////////////////////////////////
// Vector3 structure
/////////////////////////////////////////////////////////////////////////
typedef struct Vector4
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	float x;
	float y;
	float z;
	float w;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	
	// costructors
	Vector4();
	Vector4( const float* );
	Vector4( Vector2& vec, float z, float w );
	Vector4( Vector3& vec, float w );
	Vector4( float x, float y, float z, float w );

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector4& operator += ( const Vector4& );
	Vector4& operator -= ( const Vector4& );
	Vector4& operator *= ( float );
	Vector4& operator /= ( float );

	// unary operators
	Vector4  operator + () const;
	Vector4  operator - () const;

	// binary operators
	Vector4  operator + ( Vector4 ) const;
	Vector4  operator - ( Vector4 ) const;
	Vector4  operator * ( float ) const;
	Vector4  operator / ( float ) const;
	bool     operator == ( const Vector4& ) const;
	bool     operator != ( const Vector4& ) const;
	friend Vector4 operator * ( float, const Vector4& );

	// Length and Normalize methods
	float   Length() const;
	float   LengthSquared() const;
	Vector4& Normalize();

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
	/* NOTHING */
} Vector4;

/////////////////////////////////////////////////////////////////////////
// Matrix structure
/////////////////////////////////////////////////////////////////////////
typedef struct Matrix 
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	
	// constructors
	Matrix();
	Matrix( const float* );
	Matrix( float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44 );

	// access grants
	float& operator () ( unsigned int row, unsigned int col );
	float  operator () ( unsigned int row, unsigned int col ) const;

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Matrix& operator *= ( Matrix );
	Matrix& operator += ( const Matrix& );
	Matrix& operator -= ( const Matrix& );
	Matrix& operator *= ( float );
	Matrix& operator /= ( float );

	// unary operators
	Matrix  operator + () const;
	Matrix  operator - () const;

	// binary operators
	Matrix  operator * ( Matrix ) const;
	Matrix  operator + ( Matrix ) const;
	Matrix  operator - ( Matrix ) const;
	Matrix  operator * ( float ) const;
	Matrix  operator / ( float ) const;
	bool    operator == ( const Matrix& ) const;
	bool    operator != ( const Matrix& ) const;
	friend Matrix operator * ( float, const Matrix& );

	// Determinant and Identity methods
	float   Determinant() const;
	void    Identity();

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
	/* NOTHING */

} Matrix;

/////////////////////////////////////////////////////////////////////////
// Quaternion structure
/////////////////////////////////////////////////////////////////////////
typedef struct Quaternion
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	float x;
	float y;
	float z;
	float w;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------

	// constructors
	Quaternion();
	Quaternion( const float* );
	Quaternion( float x, float y, float z, float w );

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Quaternion& operator += ( const Quaternion& );
	Quaternion& operator -= ( const Quaternion& );
	Quaternion& operator *= ( const Quaternion& );
	Quaternion& operator *= ( float );
	Quaternion& operator /= ( float );

	// unary operators
	Quaternion  operator + () const;
	Quaternion  operator - () const;

	// binary operators
    Quaternion  operator * ( Quaternion ) const;
	Quaternion  operator + ( Quaternion ) const;
	Quaternion  operator - ( Quaternion ) const;
	Quaternion  operator * ( float ) const;
	Quaternion  operator / ( float ) const;
	bool operator == ( const Quaternion& ) const;
	bool operator != ( const Quaternion& ) const;
	friend Quaternion operator * ( float, const Quaternion& );

	// Length and Normalize mathods
	float      Length() const;
	float      LengthSquared() const;
	void       Conjugate();
	Quaternion& Normalize();

} Quaternion;

/////////////////////////////////////////////////////////////////////////
// Plane structure
/////////////////////////////////////////////////////////////////////////
typedef struct Plane
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	float a;
	float b;
	float c;
	float d;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	
	// constructors
	Plane();
	Plane( const float* );
	Plane( float a, float b, float c, float d );

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Plane& operator *= ( float );
	Plane& operator /= ( float );

	// unary operators
	Plane  operator + () const;
	Plane  operator - () const;

	// binary operators
	Plane  operator * ( float ) const;
	Plane  operator / ( float ) const;
	bool   operator == ( const Plane& ) const;
	bool   operator != ( const Plane& ) const;
	friend Plane operator * ( float, Plane& );

	// Normalize method
	Plane& Normalize();
    Plane& FromPointNormal( const Vector3 &point, const Vector3 &normal );
    Plane& FromPoints( const Vector3 &p0, const Vector3 &p1, const Vector3 &p2 );
    bool   Inside( const Vector3 &point ) const;
    bool   Inside( const Vector3 &poont, const float radius ) const;

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
	/* NOTHING */
} Plane;

/////////////////////////////////////////////////////////////////////////
// BoundingBox structure
/////////////////////////////////////////////////////////////////////////
typedef struct BoundingBox
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	Vector3 minimum;
	Vector3 maximum;
	
	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	BoundingBox();
	BoundingBox( Vector3 minmum, Vector3 maximum );
	bool operator == ( BoundingBox& ) const;
	bool operator != ( BoundingBox& ) const;
	void Merge( Vector3 value );
	void Merge( int size, Vector3 value[] );

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
	/* NOTHING */
	
} BoundingBox;

/////////////////////////////////////////////////////////////////////////
// BoundingSphere strcture
/////////////////////////////////////////////////////////////////////////
typedef struct BoundingSphere
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	/* NOTHING */

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	Vector3 position;
	float   radius;

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	BoundingSphere();
	BoundingSphere( Vector3 position, float radius );
	BoundingSphere( BoundingBox box );
	bool operator == ( BoundingSphere& ) const;
	bool operator != ( BoundingSphere& ) const;
	void Merge( Vector3 value );
	void Merge( int size, Vector3* value );

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
	/* NOTHING */

} BoundingSphere;

/////////////////////////////////////////////////////////////////////////
// Vector Methods
/////////////////////////////////////////////////////////////////////////

// Abs
Vector2 Abs( Vector2 v );
Vector3 Abs( Vector3 v );
Vector4 Abs( Vector4 v );
void    Abs( Vector2& v, Vector2& result );
void    Abs( Vector3& v, Vector3& result );
void    Abs( Vector4& v, Vector4& result );
// Clamp
Vector2 Clamp( Vector2 v, Vector2 mini, Vector2 maxi );
Vector3 Clamp( Vector3 v, Vector3 mini, Vector3 maxi );
Vector4 Clamp( Vector4 v, Vector4 mini, Vector4 maxi );
void    Clamp( Vector2& v, Vector2& mini, Vector2& maxi, Vector2& result );
void    Clamp( Vector3& v, Vector3& mini, Vector3& maxi, Vector3& result );
void    Clamp( Vector4& v, Vector4& mini, Vector4& maxi, Vector4& result );
// Distance
float Distance( Vector2 v1, Vector2 v2 );
float Distance( Vector3 v1, Vector3 v2 );
float Distance( Vector4 v1, Vector4 v2 );
void  Distance( Vector2& v1, Vector2& v2, float& result );
void  Distance( Vector3& v1, Vector3& v2, float& result );
void  Distance( Vector4& v1, Vector4& v2, float& result );
// DistanceSquared
float DistanceSquared( Vector2 v1, Vector2 v2 );
float DistanceSquared( Vector3 v1, Vector3 v2 );
float DistanceSquared( Vector4 v1, Vector4 v2 );
void  DistanceSquared( Vector2& v1, Vector2& v2, float& result );
void  DistanceSquared( Vector3& v1, Vector3& v2, float& reuslt );
void  DistanceSquared( Vector4& v1, Vector4& v2, float& result );
// Dot
float Dot( Vector2 v1, Vector2 v2 );
float Dot( Vector3 v1, Vector3 v2 );
float Dot( Vector4 v1, Vector4 v2 );
void  Dot( Vector2& v1, Vector2& v2, float& result );
void  Dot( Vector3& v1, Vector3& v2, float& result );
void  Dot( Vector4& v1, Vector4& v2, float& result );
// Cross
Vector3 Cross( Vector3 v1, Vector3 v2 );
void    Cross( Vector3& v1, Vector3& v2, Vector3& result );
// Normalize
Vector2 Normalize( Vector2 v );
Vector3 Normalize( Vector3 v );
Vector4 Normalize( Vector4 v );
void    Normalize( Vector2& v, Vector2& result );
void    Normalize( Vector3& v, Vector2& result );
void    Normalize( Vector4& v, Vector4& result );
// ComputeNormal
Vector3 ComputeNormal( Vector3 p1, Vector3 p2, Vector3 p3 );
void    ComputeNormal( Vector3& p1, Vector3& p2, Vector3& p3, Vector3& result );
// ComputeQuadNormal
Vector3 ComputeQuadNormal( Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4 );
void    ComputeQuadNormal( Vector3& p1, Vector3& p2, Vector3& p3, Vector3& p4, Vector3& result );
// ComputeTangent
Vector3 ComputeTangent( Vector3 p1, Vector3 p2, Vector3 p3, Vector2 t1, Vector2 t2, Vector2 t3 );
void    ComputeTangent( Vector3& p1, Vector3& p2, Vector3& p3, Vector2& t1, Vector2& t2, Vector2& t3, Vector3& result );
// CompuateBinormal
Vector3 ComputeBinormal( Vector3 normal, Vector3 tangent );
void    ComputeBinormal( Vector3& normal, Vector3& tangent, Vector3& result );
// ComputeCrossingAngle
float   ComputeCrossingAngle( Vector2 v1, Vector2 v2 );
float   ComputeCrossingAngle( Vector3 v1, Vector3 v2 );
float   ComputeCrossingAngle( Vector4 v1, Vector4 v2 );
void    ComputeCrossingAngle( Vector2& v1, Vector2& v2, float& result );
void    ComputeCrossingAngle( Vector3& v1, Vector3& v2, float& result );
void    ComputeCrossingAngle( Vector4& v1, Vector4& v2, float& result );
// Min
Vector2 Min( Vector2 v1, Vector2 v2 );
Vector3 Min( Vector3 v1, Vector3 v2 );
Vector4 Min( Vector4 v1, Vector4 v2 );
void    Min( Vector2& v1, Vector2& v2, Vector2& result );
void    Min( Vector3& v1, Vector3& v2, Vector3& result );
void    Min( Vector4& v1, Vector4& v2, Vector4& result );
// Max
Vector2 Max( Vector2 v1, Vector2 v2 );
Vector3 Max( Vector3 v1, Vector3 v2 );
Vector4 Max( Vector4 v1, Vector4 v2 );
void    Max( Vector2& v1, Vector2& v2, Vector2& result );
void    Max( Vector3& v1, Vector3& v2, Vector3& result );
void    Max( Vector4& v1, Vector4& v2, Vector4& result );
// Reflect
Vector2 Reflect( Vector2 i, Vector2 n );
Vector3 Reflect( Vector3 i, Vector3 n );
void    Reflect( Vector2& i, Vector2& n, Vector2& result );
void    Reflect( Vector3& i, Vector3& n, Vector3& result );
// Refract
Vector3 Refract( Vector3 i, Vector3 n, float eta );
void    Refract( Vector3& i, Vector3& n, float eta, Vector3& result );
// Barycentric
Vector2 Barycentric( Vector2 v1, Vector2 v2, Vector2 v3, float f, float g );
Vector3 Barycentric( Vector3 v1, Vector3 v2, Vector3 v3, float f, float g );
Vector4 Barycentric( Vector4 v1, Vector4 v2, Vector4 v3, float f, float g );
void    Barycentric( Vector2 &v1, Vector2 &v2, Vector2 &v3, float f, float g, Vector2 &result );
void    Barycentric( Vector3 &v1, Vector3 &v2, Vector3 &v3, float f, float g, Vector3 &result );
void    Barycentric( Vector4 &v1, Vector4 &v2, Vector4 &v3, float f, float g, Vector4 &result );
// Lerp
Vector2 Lerp( Vector2 v1, Vector2 v2, float t );
Vector3 Lerp( Vector3 v1, Vector3 v2, float t );
Vector4 Lerp( Vector4 v1, Vector4 v2, float t );
void    Lerp( Vector2 &v1, Vector2 &v2, float t, Vector2 &result );
void    Lerp( Vector3 &v1, Vector3 &v2, float t, Vector3 &reuslt );
void    Lerp( Vector4 &v1, Vector4 &v2, float r, Vector4 &result );
// SmoothStep
Vector2 SmoothStep( Vector2 v1, Vector2 v2, float t );
Vector3 SmoothStep( Vector3 v1, Vector3 v2, float t );
Vector4 SmoothStep( Vector4 v1, Vector4 v2, float t );
void    SmoothStep( Vector2 &v1, Vector2 &v2, float t, Vector2 &result );
void    SmoothStep( Vector3 &v1, Vector3 &v2, float t, Vector3 &result );
void    SmoothStep( Vector4 &v1, Vector4 &v2, float t, Vector4 &result );
// Transform
Vector2 Transform( Vector2 position, Matrix matrix );
Vector3 Transform( Vector3 position, Matrix matrix );
Vector4 Transform( Vector4 position, Matrix matrix );
void    Transform( Vector2 &position, Matrix &matrix, Vector2 &result );
void    Transform( Vector3 &position, Matrix &matrix, Vector3 &result );
void    Transform( Vector4 &position, Matrix &matrix, Vector4 &result );
// TransformNormal
Vector2 TransformNormal( Vector2 normal, Matrix matrix );
Vector3 TransformNormal( Vector3 normal, Matrix matrix );
void    TransformNormal( Vector2 &normal, Matrix &matrix, Vector2 &result );
void    TransformNormal( Vector3 &normal, Matrix &matrix, Vector3 &result );
// TransformCoordinate
Vector2 TransformCoordinate( Vector2 coords, Matrix matrix );
Vector3 TransformCoordiante( Vector3 coords, Matrix matrix );
void    TransformCoordinate( Vector2 &coords, Matrix &matrix, Vector2 &result );
void    TransformCoordinate( Vector3 &coords, Matrix &matrix, Vector3& result );


/////////////////////////////////////////////////////////////////////////
// Matrix Methods
/////////////////////////////////////////////////////////////////////////

// Identity
Matrix& Identity( Matrix &value );
// IsIdentity
bool    IsIdentity( Matrix &value );
// Transpose
Matrix  Transpose( Matrix value );
void    Transpose( Matrix &value, Matrix &result );
// Multiply
Matrix  Multiply( Matrix value1, Matrix value2 );
Matrix  Multiply( Matrix value, float scaleFactor );
void    Multiply( Matrix &value1, Matrix &value2, Matrix &result );
void    Multiply( Matrix &value, float scaleFactor, Matrix &result );
// MultiplyTranspose
Matrix  MultiplyTranspose( Matrix value1, Matrix value2 );
void    MultiplyTranspose( Matrix &value1, Matrix &value2, Matrix &result );
// Invert
Matrix  Invert( Matrix value );
void    Invert( Matrix &value, Matrix &result );
// CreateFromQuaternion
Matrix  CreateFromQuaternion( Quaternion qua );
void    CreateFromQuaternion( Quaternion &qua, Matrix &result );
// CreateScale
Matrix  CreateScale( float scale );
Matrix  CreateScale( float xScale, float yScale, float zScale );
Matrix  CreateScale( Vector3 scales );
void    CreateScale( float scale, Matrix &result );
void    CreateScale( float xScale, float yScale, float zScale, Matrix &result );
void    CreateScale( Vector3 &scales, Matrix &result );
// CreateTranslation
Matrix  CreateTranslation( float xPos, float yPos, float zPos );
Matrix  CreateTranslation( Vector3 position );
void    CreateTranslation( float xPos, float yPos, float zPos, Matrix &result );
void    CreateTranslation( Vector3 &position, Matrix &result );
// CreateRotationX
Matrix  CreateRotationX( float radian );
void    CreateRotationX( float radian, Matrix &result );
// CreateRotationY
Matrix  CreateRotationY( float radian );
void    CreateRotationY( float radian, Matrix &result );
// CreateRotationZ
Matrix  CreateRotationZ( float radian );
void    CreateRotationZ( float radian, Matrix &result );
// CreateFromAxisAngle
Matrix  CreateFromAxisAngle( Vector3 axis, float radian );
void    CreateFromAxisAngle( Vector3 &axis, float radian, Matrix &result );
// CreateLookAt
Matrix  CreateLookAt( Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector );
void    CreateLookAt( Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector, Matrix &result );
// CreatePespective
Matrix  CreatePerspective( float width, float height, float nearPlaneDistance, float farPlaneDistance );
void    CreatePerspective( float width, float height, float nearPlaneDistance, float farPlaneDistance, Matrix &result );
// CreatePerspectiveFieldOfView
Matrix  CreatePerspectiveFieldOfView( float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance );
void    CreatePerspectiveFieldOfView( float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance, Matrix &result );
// CreatePerspectiveOffcenter
//Matrix  CreatePerspectiveOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance );
//void    CreatePerspectiveOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance, Matrix &result );
// CreateOrthographic
Matrix  CreateOrthographic( float width, float height, float nearPlaneDistance, float farPlaneDistance );
void    CreateOrthographic( float width, float height, float nearPlaneDistance, float farPlaneDistance, Matrix &result );
// CreateOrthographicOffcenter
Matrix  CreateOrthographicOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance );
void    CreateOrthographicOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance, Matrix &result );
// CreateShadow
Matrix  CreateShadow( Vector4 lightDirection, Plane plane );
void    CreateShadow( Vector4 &lightDirection, Plane &plane, Matrix &result );
// CreateReflection
Matrix  CreateReflection( Plane plane );
void    CreateReflection( Plane &plane, Matrix &result );
// Lerp
Matrix  Lerp( Matrix v1, Matrix v2, float t );
void    Lerp( Matrix &v1, Matrix &v2, float t, Matrix &result );


/////////////////////////////////////////////////////////////////////////
// Quaternion Functions
/////////////////////////////////////////////////////////////////////////

// Identity
Quaternion& Identity( Quaternion &value );
// IsIdentity
bool        IsIdentity( Quaternion &value );
// Multiply
Quaternion  Multiply( Quaternion q1, Quaternion q2 );
void        Multiply( Quaternion &q1, Quaternion &q2, Quaternion &result );
// Dot
float       Dot( Quaternion q1, Quaternion q2 );
void        Dot( Quaternion &q1, Quaternion &q2, float &result );
// Conjugate
Quaternion  Conjugate( Quaternion qua );
void        Conjugate( Quaternion &qua, Quaternion &result );
// Normalize
Quaternion  Normalize( Quaternion qua );
void        Normalize( Quaternion &qua, Quaternion &result );
// Inverse
Quaternion  Inverse( Quaternion qua );
void        Inverse( Quaternion &qua, Quaternion &result );
// CreateFromYawPitchRoll
Quaternion  CreateFromYawPitchRoll( float yaw, float pitch, float roll );
void        CreateFromYawPitchRoll( float yaw, float pitch, float roll, Quaternion &result );
// Slerp
Quaternion  Slerp( Quaternion q1, Quaternion q2, float t );
void        Slerp( Quaternion &q1, Quaternion &q2, float t, Quaternion &result );
// Squad
Quaternion  Squad( Quaternion qua, Quaternion a, Quaternion b, Quaternion c, float t );
void        Squad( Quaternion &qua, Quaternion &a, Quaternion &b, Quaternion &c, float t, Quaternion &result );


/////////////////////////////////////////////////////////////////////////
// Plane Functions
/////////////////////////////////////////////////////////////////////////

// Dot
float Dot( Plane p, Vector4 v );
void  Dot( Plane &p, Vector4 &v, float &result );
// DotCoordinate
float DotCoordinate( Plane p, Vector3 v );
void  DotCoordinate( Plane &p, Vector3 &v, float &result );
// DotNormal
float DotNormal( Plane p, Vector3 v );
void  DotNormal( Plane &p, Vector3 &v, float &result );
// Normalize
Plane Normalize( Plane p );
void  Normalize( Plane &p, Plane &result );

} // namespace Math
} // namespace Asura


//-----------------------------------------------------------------------
// Inline Files
//-----------------------------------------------------------------------
#include "Math/Vector.inl"
#include "Math/Matrix.inl"
#include "Math/Quaternion.inl"
#include "Math/Plane.inl"
#include "Math/Bounding.inl"


#endif//__PHANTOM_MATH_H__