//-----------------------------------------------------------------------
// File : Vector.inl
// Desc : Vector Inline Functions
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/16 [Pocol] êVãKçÏê¨.
// <End>

namespace Asura {
namespace Math {

/////////////////////////////////////////////////////////////////////////
// Vector2 structure
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Vector2::Vector2()
{
	x = y = 0;
}

ASURA_INLINE
Vector2::Vector2( const float* pf )
{
	assert( pf != NULL );
	x = pf[ 0 ];
	y = pf[ 1 ];	
}

ASURA_INLINE
Vector2::Vector2( float nx, float ny )
{
	x = nx;
	y = ny;
}

ASURA_INLINE
Vector2::operator float *()
{ return (float*)&x; }

ASURA_INLINE
Vector2::operator const float *() const
{ return (const float*)&x; }

ASURA_INLINE
Vector2& Vector2::operator += ( const Vector2& v )
{
	x += v.x;
	y += v.y;
	return (*this);
}

ASURA_INLINE
Vector2& Vector2::operator -= ( const Vector2& v )
{
	x -= v.x;
	y -= v.y;
	return (*this);
}

ASURA_INLINE
Vector2& Vector2::operator *= ( float f )
{
	x *= f;
	y *= f;
	return (*this);
}

ASURA_INLINE
Vector2& Vector2::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	return (*this);
}

ASURA_INLINE
Vector2 Vector2::operator + () const
{ return (*this); }

ASURA_INLINE
Vector2 Vector2::operator - () const
{ return Vector2( -x, -y ); }

ASURA_INLINE
Vector2 Vector2::operator + ( Vector2 v ) const
{ return Vector2( x + v.x, y + v.y ); }

ASURA_INLINE
Vector2 Vector2::operator - ( Vector2 v ) const
{ return Vector2( x - v.x, y - v.y ); }

ASURA_INLINE
Vector2 Vector2::operator * ( float f ) const
{ return Vector2( x * f, y * f ); }

ASURA_INLINE
Vector2 Vector2::operator / ( float f ) const
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	return Vector2( x * fInv, y * fInv );
}

ASURA_INLINE
Vector2 operator * ( float f, const Vector2& v )
{ return Vector2( f * v.x, f * v.y ); }

ASURA_INLINE
bool Vector2::operator == ( const Vector2& v ) const
{ return ( IsNearlyEqual( x, v.x ) && IsNearlyEqual( y, v.y ) ); }

ASURA_INLINE
bool Vector2::operator != ( const Vector2& v ) const
{ return ( !IsNearlyEqual( x, v.x ) || !IsNearlyEqual( y, v.y ) ); }

ASURA_INLINE
float Vector2::Length() const
{ return sqrtf( x * x + y * y ); }

ASURA_INLINE
float Vector2::LengthSquared() const
{ return ( x * x + y * y ); }

ASURA_INLINE
Vector2& Vector2::Normalize()
{
    float mag = Length();
    //assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	x *= invMag;
	y *= invMag;
	return (*this);
}


/////////////////////////////////////////////////////////////////////////
// Vector3 structure
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Vector3::Vector3()
{
	x = y = z= 0;
}

ASURA_INLINE
Vector3::Vector3( const float* pf )
{
	assert( pf != NULL );
	x = pf[ 0 ];
	y = pf[ 1 ];
	z = pf[ 2 ];
}

ASURA_INLINE
Vector3::Vector3( Vector2& v, float nz )
{
	x = v.x;
	y = v.y;
	z = nz;
}

ASURA_INLINE
Vector3::Vector3( float nx, float ny, float nz )
{
	x = nx;
	y = ny;
	z = nz;
}

ASURA_INLINE
Vector3::operator float *()
{ return (float*)&x; }

ASURA_INLINE
Vector3::operator const float *() const
{ return (const float*)&x; }

ASURA_INLINE
Vector3& Vector3::operator += ( const Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	return (*this);
}

ASURA_INLINE
Vector3& Vector3::operator -= ( const Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return (*this);
}

ASURA_INLINE
Vector3& Vector3::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	return (*this);
}

ASURA_INLINE
Vector3& Vector3::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	return (*this);
}

ASURA_INLINE
Vector3 Vector3::operator + () const
{ return (*this); }

ASURA_INLINE
Vector3 Vector3::operator - () const
{ return Vector3( -x, -y, -z ); }

ASURA_INLINE
Vector3 Vector3::operator + ( Vector3 v ) const
{ return Vector3( x + v.x, y + v.y, z + v.z ); }

ASURA_INLINE
Vector3 Vector3::operator - ( Vector3 v ) const
{ return Vector3( x - v.x, y - v.y, z - v.z ); }

ASURA_INLINE
Vector3 Vector3::operator * ( float f ) const
{ return Vector3( x * f, y * f, z * f ); }

ASURA_INLINE
Vector3 Vector3::operator / ( float f ) const
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	return Vector3( x * fInv, y * fInv, z * fInv );
}

ASURA_INLINE
Vector3 operator * ( float f, const Vector3& v )
{ return Vector3( f * v.x, f * v.y, f * v.z ); }

ASURA_INLINE
bool Vector3::operator == ( const Vector3& v ) const
{
	return ( IsNearlyEqual( x, v.x ) 
		  && IsNearlyEqual( y, v.y ) 
		  && IsNearlyEqual( z, v.z ) );
}

ASURA_INLINE
bool Vector3::operator != ( const Vector3& v ) const
{ 
	return ( !IsNearlyEqual( x, v.x ) 
		  || !IsNearlyEqual( y, v.y ) 
		  || !IsNearlyEqual( z, v.z ) ); 
}

ASURA_INLINE
float Vector3::Length() const
{ return sqrtf( x * x + y * y + z * z); }

ASURA_INLINE
float Vector3::LengthSquared() const
{ return ( x * x + y * y + z * z); }

ASURA_INLINE
Vector3& Vector3::Normalize()
{
    float mag = Length();
    //assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	x *= invMag;
	y *= invMag;
	z *= invMag;
	return (*this);
}

/////////////////////////////////////////////////////////////////////////
// Vector4 structure
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Vector4::Vector4()
{
	x = y = z= 0;
}

ASURA_INLINE
Vector4::Vector4( const float* pf )
{
	assert( pf != NULL );
	x = pf[ 0 ];
	y = pf[ 1 ];
	z = pf[ 2 ];
	w = pf[ 3 ];
}

ASURA_INLINE
Vector4::Vector4( Vector2& v, float nz, float nw )
{
	x = v.x;
	y = v.y;
	z = nz;
	w = nw;
}

ASURA_INLINE
Vector4::Vector4( Vector3& v, float nw )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = nw;
}

ASURA_INLINE
Vector4::Vector4( float nx, float ny, float nz, float nw )
{
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

ASURA_INLINE
Vector4::operator float *()
{ return (float*)&x; }

ASURA_INLINE
Vector4::operator const float *() const
{ return (const float*)&x; }

ASURA_INLINE
Vector4& Vector4::operator += ( const Vector4& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return (*this);
}

ASURA_INLINE
Vector4& Vector4::operator -= ( const Vector4& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return (*this);
}

ASURA_INLINE
Vector4& Vector4::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return (*this);
}

ASURA_INLINE
Vector4& Vector4::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return (*this);
}

ASURA_INLINE
Vector4 Vector4::operator + () const
{ return (*this); }

ASURA_INLINE
Vector4 Vector4::operator - () const
{ return Vector4( -x, -y, -z, -w ); }

ASURA_INLINE
Vector4 Vector4::operator + ( Vector4 v ) const
{ return Vector4( x + v.x, y + v.y, z + v.z, w + v.w ); }

ASURA_INLINE
Vector4 Vector4::operator - ( Vector4 v ) const
{ return Vector4( x - v.x, y - v.y, z - v.z, w - v.w ); }

ASURA_INLINE
Vector4 Vector4::operator * ( float f ) const
{ return Vector4( x * f, y * f, z * f, w * f ); }

ASURA_INLINE
Vector4 Vector4::operator / ( float f ) const
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	return Vector4( x * fInv, y * fInv, z * fInv, w * fInv );
}

ASURA_INLINE
Vector4 operator * ( float f, const Vector4& v )
{ return Vector4( f * v.x, f * v.y, f * v.z, f * v.w ); }

ASURA_INLINE
bool Vector4::operator == ( const Vector4& v ) const
{
	return ( IsNearlyEqual( x, v.x ) 
		  && IsNearlyEqual( y, v.y ) 
		  && IsNearlyEqual( z, v.z )
		  && IsNearlyEqual( w, v.w ) );
}

ASURA_INLINE
bool Vector4::operator != ( const Vector4& v ) const
{ 
	return ( !IsNearlyEqual( x, v.x ) 
		  || !IsNearlyEqual( y, v.y ) 
		  || !IsNearlyEqual( z, v.z )
		  || !IsNearlyEqual( w, v.w ) ); 
}

ASURA_INLINE
float Vector4::Length() const
{ return sqrtf( x * x + y * y + z * z + w * w ); }

ASURA_INLINE
float Vector4::LengthSquared() const
{ return ( x * x + y * y + z * z + w * w ); }

ASURA_INLINE
Vector4& Vector4::Normalize()
{
    float mag = Length();
    //assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	x *= invMag;
	y *= invMag;
	z *= invMag;
	w *= invMag;
	return (*this);
}

/////////////////////////////////////////////////////////////////////////
// Vector Methods
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Vector2 Abs( Vector2 v )
{ return Vector2( fabs( v.x ), fabs( v.y ) ); }

ASURA_INLINE
Vector3 Abs( Vector3 v )
{ return Vector3( fabs( v.x ), fabs( v.y ), fabs( v.z ) ); }

ASURA_INLINE
Vector4 Abs( Vector4 v )
{ return Vector4( fabs( v.x ), fabs( v.y ), fabs( v.z ), fabs( v.w ) ); }

ASURA_INLINE
void Abs( Vector2 &v, Vector2 &result )
{ result = Abs( v ); }

ASURA_INLINE
void Abs( Vector3 &v, Vector3 &result )
{ result = Abs( v ); }

ASURA_INLINE
void Abs( Vector4 &v, Vector4 &result )
{ result = Abs( v ); }

ASURA_INLINE
Vector2 Clamp( Vector2 v, Vector2 mini, Vector3 maxi )
{
	return Vector2( Clamp< float >( v.x, mini.x, maxi.x ),
				    Clamp< float >( v.y, mini.y, maxi.y ) );
}

ASURA_INLINE
Vector3 Clamp( Vector3 v, Vector3 mini, Vector3 maxi )
{
	return Vector3( Clamp< float >( v.x, mini.x, maxi.x ),
		            Clamp< float >( v.y, mini.y, maxi.y ),
					Clamp< float >( v.z, mini.z, maxi.z ) );
}

ASURA_INLINE
Vector4 Clamp( Vector4 v, Vector4 mini, Vector4 maxi )
{
	return Vector4( Clamp< float >( v.x, mini.x, maxi.x ),
		            Clamp< float >( v.y, mini.y, maxi.y ),
					Clamp< float >( v.z, mini.z, maxi.z ),
					Clamp< float >( v.w, mini.w, maxi.w ) );
}

ASURA_INLINE
void Clamp( Vector2 &v, Vector2 &mini, Vector2 &maxi, Vector2 &result )
{ result = Clamp( v, mini, maxi ); }

ASURA_INLINE
void Clamp( Vector3 &v, Vector3 &mini, Vector3 &maxi, Vector3 &result )
{ result = Clamp( v, mini, maxi ); }

ASURA_INLINE
void Clamp( Vector4 &v, Vector4 &mini, Vector4 &maxi, Vector4 &result )
{ result = Clamp( v, mini, maxi ); }

ASURA_INLINE
float Distance( Vector2 v1, Vector2 v2 )
{
	Vector2 v = v2 - v1;
	return v.Length();
}

ASURA_INLINE
float Distance( Vector3 v1, Vector3 v2 )
{
	Vector3 v = v2 - v1;
	return v.Length();
}

ASURA_INLINE
float Distance( Vector4 v1, Vector4 v2 )
{
	Vector4 v = v2 - v1;
	return v.Length();
}

ASURA_INLINE
void Distance( Vector2 &v1, Vector2 &v2, float &result )
{ result = Distance( v1, v2 ); }

ASURA_INLINE
void Distance( Vector3 &v1, Vector3 &v2, float &result )
{ result = Distance( v1, v2 ); }

ASURA_INLINE
void Distance( Vector4 &v1, Vector4 &v2, float &result )
{ result = Distance( v1, v2 ); }

ASURA_INLINE
float DistanceSquared( Vector2 v1, Vector2 v2 )
{
	Vector2 v = v2 - v1;
	return v.LengthSquared();
}

ASURA_INLINE
float DistanceSquared( Vector3 v1, Vector3 v2 )
{
	Vector3 v = v2 - v1;
	return v.LengthSquared();
}

ASURA_INLINE
float DistanceSquared( Vector4 v1, Vector4 v2 )
{
	Vector4 v = v2 - v1;
	return v.LengthSquared();
}

ASURA_INLINE
void DistanceSquared( Vector2 &v1, Vector2 &v2, float &result )
{ result = DistanceSquared( v1, v2 ); }

ASURA_INLINE
void DistanceSquared( Vector3 &v1, Vector3 &v2, float &result )
{ result = DistanceSquared( v1, v2 ); }

ASURA_INLINE
void DistanceSquared( Vector4 &v1, Vector4 &v2, float &result )
{ result = DistanceSquared( v1, v2 ); }

ASURA_INLINE
float Dot( Vector2 v1, Vector2 v2 )
{ return ( v1.x * v2.x + v1.y * v2.y ); }

ASURA_INLINE
float Dot( Vector3 v1, Vector3 v2 )
{ return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z ); }

ASURA_INLINE
float Dot( Vector4 v1, Vector4 v2 )
{ return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w ); }

ASURA_INLINE
void Dot( Vector2 &v1, Vector2 &v2, float &result )
{ result = Dot( v1, v2 ); }

ASURA_INLINE
void Dot( Vector3 &v1, Vector3 &v2, float &result )
{ result = Dot( v1, v2 ); }

ASURA_INLINE
void Dot( Vector4 &v1, Vector4 &v2, float &result )
{ result = Dot( v1, v2 ); }

ASURA_INLINE
Vector3 Cross( Vector3 v1, Vector3 v2 )
{
	return Vector3(	( v1.y * v2.z ) - ( v1.z * v2.y ),
		            ( v1.z * v2.x ) - ( v1.x * v2.z ),
		            ( v1.x * v2.y ) - ( v1.y * v2.x ) );
}

ASURA_INLINE
void Cross( Vector3 &v1, Vector3 &v2, Vector3 &result )
{ result = Cross( v1, v2 ); }

ASURA_INLINE
Vector2 Normalize( Vector2 v )
{
    float mag = v.Length();
    assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	return Vector2( v.x * invMag, v.y * invMag );
}

ASURA_INLINE
Vector3 Normalize( Vector3 v )
{
    float mag = v.Length();
    assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	return Vector3( v.x * invMag, v.y * invMag, v.z * invMag ); 
}

ASURA_INLINE
Vector4 Normalize( Vector4 v )
{
    float mag = v.Length();
    assert( !IsNearZero( mag ) );
	float invMag = 1.0f / mag;
	return Vector4( v.x * invMag, v.y * invMag, v.z * invMag, v.w * invMag );
}

ASURA_INLINE
void Normalize( Vector2 &v, Vector2 &result )
{ result = Normalize( v ); }

ASURA_INLINE
void Normalize( Vector3 &v, Vector3 &result )
{ result = Normalize( v ); }

ASURA_INLINE
void Normalize( Vector4 &v, Vector4 &result )
{ result = Normalize( v ); }

ASURA_INLINE
Vector3 ComputeNormal( Vector3 p1, Vector3 p2, Vector3 p3 )
{
	Vector3 v1 = p2 - p1;
	Vector3 v2 = p3 - p1;
	Vector3 result = Cross( v1, v2 );
	return result.Normalize();
}

ASURA_INLINE
void ComputeNormal( Vector3 &p1, Vector3 &p2, Vector3 &p3, Vector3 &result )
{ result = ComputeNormal( p1, p2, p3 ); }

ASURA_INLINE
Vector3 ComputeQuadNormal( Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4 )
{
	Vector3 result;
	Vector3 n1a = ComputeNormal( p1, p2, p3 );
	Vector3 n1b = ComputeNormal( p1, p3, p4 );
	Vector3 n2a = ComputeNormal( p2, p3, p4 );
	Vector3 n2b = ComputeNormal( p2, p4, p1 );
	if ( Dot( n1a, n1b ) > Dot( n2a, n2b ) )
	{
		result = n1a + n1b;
		result.Normalize();
	}
	else
	{
		result = n2a + n2b;
		result.Normalize();
	}
	return result;
}

ASURA_INLINE
void ComputeQuadNormal( Vector3 &p1, Vector3 &p2, Vector3 &p3, Vector3 &p4, Vector3 &result )
{ result = ComputeQuadNormal( p1, p2, p3, p4 ); }

ASURA_INLINE
Vector3 CompuateTangent( Vector3 p1, Vector3 p2, Vector3 p3,
						 Vector2 t1, Vector2 t2, Vector2 t3 )
{
	Vector3 result = -( t3.x - t1.x ) * ( p2 - p1 ) + ( t2.x - t1.x ) * ( p3 - p1 );
	return result;
}

ASURA_INLINE
void ComputeTangent( Vector3 &p1, Vector3 &p2, Vector3 &p3,
					 Vector2 &t1, Vector2 &t2, Vector2 &t3, Vector3 &result )
{ result = ComputeTangent( p1, p2, p3, t1, t2, t3 ); }

ASURA_INLINE
Vector3 ComputeBinormal( Vector3 normal, Vector3 tangent )
{ return Cross( normal, tangent ); }

ASURA_INLINE
void ComputeBinormal( Vector3 &normal, Vector3 &tangent, Vector3 &result )
{ result = ComputeBinormal( normal, tangent ); }

ASURA_INLINE
float ComputeCrossingAngle( Vector2 v1, Vector2 v2 )
{
	float d = v1.Length() * v2.Length();
	if ( IsNearZero( d ) ) { return 0; }
	float c = Dot( v1, v2 ) / d;
	if ( c >= 1.0f )  { return 0; }
	if ( c <= -1.0f ) { return Pi; }
	return acosf( c );
}

ASURA_INLINE
float ComputeCrossingAngle( Vector3 v1, Vector3 v2 )
{
	float d = v1.Length() * v2.Length();
	if ( IsNearZero( d ) ) { return 0; }
	float c = Dot( v1, v2 ) / d;
	if ( c >= 1.0f )  { return 0; }
	if ( c <= -1.0f ) { return Pi; }
	return acosf( c );
}

ASURA_INLINE
float ComputeCrossingAngle( Vector4 v1, Vector4 v2 )
{
	float d = v1.Length() * v2.Length();
	if ( IsNearZero( d ) ) { return 0; }
	float c = Dot( v1, v2 ) / d;
	if ( c >= 1.0f )  { return 0; }
	if ( c <= -1.0f ) { return Pi; }
	return acosf( c );
}

ASURA_INLINE
void ComputeCrossingAngle( Vector2 &v1, Vector2 &v2, float &result )
{ result = ComputeCrossingAngle( v1, v2 ); }

ASURA_INLINE
void ComputeCrossingAngle( Vector3 &v1, Vector3 &v2, float &result )
{ result = ComputeCrossingAngle( v1, v2 ); }

ASURA_INLINE
void ComputeCrossingAngle( Vector4 &v1, Vector4 &v2, float &result )
{ result = ComputeCrossingAngle( v1, v2 ); }

ASURA_INLINE
Vector2 Min( Vector2 v1, Vector2 v2 )
{ 
	return Vector2( Min< float >( v1.x, v2.x ),
				    Min< float >( v1.y, v2.y ) );
}

ASURA_INLINE
Vector3 Min( Vector3 v1, Vector3 v2 )
{ 
	return Vector3( Min< float >( v1.x, v2.x ),
				    Min< float >( v1.y, v2.y ),
					Min< float >( v1.z, v2.z ) );
}

ASURA_INLINE
Vector4 Min( Vector4 v1, Vector4 v2 )
{ 
	return Vector4( Min< float >( v1.x, v2.x ),
				    Min< float >( v1.y, v2.y ),
					Min< float >( v1.z, v2.z ),
					Min< float >( v1.w, v2.w ) );
}

ASURA_INLINE
void Min( Vector2 &v1, Vector2 &v2, Vector2 &result )
{ result = Min( v1, v2 ); }

ASURA_INLINE
void Min( Vector3 &v1, Vector3 &v2, Vector3 &result )
{ result = Min( v1, v2 ); }

ASURA_INLINE
void Min( Vector4 &v1, Vector4 &v2, Vector4 &result )
{ result = Min( v1, v2 ); }

ASURA_INLINE
Vector2 Max( Vector2 v1, Vector2 v2 )
{
	return Vector2( Max< float >( v1.x, v2.x ),
		            Max< float >( v1.y, v2.y ) );
}

ASURA_INLINE
Vector3 Max( Vector3 v1, Vector3 v2 )
{
	return Vector3( Max< float >( v1.x, v2.x ),
		            Max< float >( v1.y, v2.y ),
					Max< float >( v1.z, v2.z ) );
}

ASURA_INLINE
Vector4 Max( Vector4 v1, Vector4 v2 )
{
	return Vector4( Max< float >( v1.x, v2.x ),
		            Max< float >( v1.y, v2.y ),
					Max< float >( v1.z, v2.z ),
					Max< float >( v1.w, v2.w ) );
}

ASURA_INLINE
void Max( Vector2 &v1, Vector2 &v2, Vector2 &result )
{ result = Max( v1, v2 ); }

ASURA_INLINE
void Max( Vector3 &v1, Vector3 &v2, Vector3 &result )
{ result = Max( v1, v2 ); }

ASURA_INLINE
void Max( Vector4 &v1, Vector4 &v2, Vector4 &result )
{ result = Max( v1, v2 ); }

ASURA_INLINE
Vector2 Reflect( Vector2 i, Vector2 n )
{
	Vector2 result;
    float dot = Dot( n, i );
    result.x = i.x - ( 2.0f * n.x ) * dot;
    result.y = i.y - ( 2.0f * n.y ) * dot;
	return result;
}

ASURA_INLINE
Vector3 Reflect( Vector3 i, Vector3 n )
{
	Vector3 result;
    float dot = Dot( n, i );
    result.x = i.x - ( 2.0f * n.x ) * dot;
    result.y = i.y - ( 2.0f * n.y ) * dot;
    result.z = i.z - ( 2.0f * n.z ) * dot;
    return result;
}

ASURA_INLINE
void Reflect( Vector2 &i, Vector2 &n, Vector2 &result )
{ result = Reflect( i, n ); }

ASURA_INLINE
void Reflect( Vector3 &i, Vector3 &n, Vector3 &result )
{ result = Reflect( i, n ); }

ASURA_INLINE
Vector3 Refract( Vector3 i, Vector3 n, float eta )
{
	float cosi = Dot( -i, n );
	float cost2 = 1.0f - eta * eta * ( 1.0f - cosi * cosi );
	Vector3 result = eta * i + ( ( eta * cosi - sqrtf( fabs( cost2 ) ) ) * n );
	return result * Sign< float >( cost2 );
}

ASURA_INLINE
void Refract( Vector3 &i, Vector3 &n, float eta, Vector3 &result )
{ result = Refract( i, n, eta ); }

ASURA_INLINE
Vector2 Barycentric( Vector2 v1, Vector2 v2, Vector2 v3, float f, float g )
{
	Vector2 result = v1 + f * ( v2 - v1 ) + g * ( v3 - v1 );
	return result;
}

ASURA_INLINE
Vector3 Barycentric( Vector3 v1, Vector3 v2, Vector3 v3, float f, float g )
{
	Vector3 result = v1 + f * ( v2 - v1 ) + g * ( v3 - v1 );
	return result;
}

ASURA_INLINE
Vector4 Barycentric( Vector4 v1, Vector4 v2, Vector4 v3, float f, float g )
{
	Vector4 result = v1 + f * ( v2 - v1 ) + g * ( v3 - v1 );
	return result;
}

ASURA_INLINE
void Barycentric( Vector2 &v1, Vector2 &v2, Vector2 &v3, float f, float g, Vector2 &result )
{ result = Barycentric( v1, v2, v3, f, g ); }

ASURA_INLINE
void Barycentric( Vector3 &v1, Vector3 &v2, Vector3 &v3, float f, float g, Vector3 &result )
{ result = Barycentric( v1, v2, v3, f, g ); }

ASURA_INLINE
void Barycentric( Vector4 &v1, Vector4 &v2, Vector4 &v3, float f, float g , Vector4 &result )
{ result = Barycentric( v1, v2, v3, f, g ); }

ASURA_INLINE
Vector2 Lerp( Vector2 v1, Vector2 v2, float t )
{
	Vector2 result = ( 1.0f - t ) * v1 + t * v2;
	return result;
}

ASURA_INLINE
Vector3 Lerp( Vector3 v1, Vector3 v2, float t )
{
	Vector3 result = ( 1.0f - t ) * v1 + t * v2;
	return result;
}

ASURA_INLINE
Vector4 Lerp( Vector4 v1, Vector4 v2, float t )
{
	Vector4 result = ( 1.0f - t ) * v1 + t * v2;
	return result;
}

ASURA_INLINE
void Lerp( Vector2 &v1, Vector2 &v2, float t, Vector2 &result )
{ result = Lerp( v1, v2, t ); }

ASURA_INLINE
void Lerp( Vector3 &v1, Vector3 &v2, float t, Vector3 &result )
{ result = Lerp( v1, v2, t ); }

ASURA_INLINE
void Lerp( Vector4 &v1, Vector4 &v2, float t, Vector4 &result )
{ result = Lerp( v1, v2, t ); }

ASURA_INLINE
Vector2 SmoothStep( Vector2 v1, Vector2 v2, float t )
{
	float s = Clamp< float >( t, 0.0f, 1.0f );
	return Lerp( v1, v2, ( s * s ) + ( 3.0f - ( 2.0f * s ) ) );
}

ASURA_INLINE
Vector3 SmoothStep( Vector3 v1, Vector3 v2, float t )
{
	float s = Clamp< float >( t, 0.0f, 1.0f );
	return Lerp( v1, v2, ( s * s ) + ( 3.0f - ( 2.0f * s ) ) );
}

ASURA_INLINE
Vector4 SmoothStep( Vector4 v1, Vector4 v2, float t )
{
	float s = Clamp< float >( t, 0.0f, 1.0f );
	return Lerp( v1, v2, ( s * s ) + ( 3.0f - ( 2.0f * s ) ) );
}

ASURA_INLINE
void SmoothStep( Vector2 &v1, Vector2 &v2, float t, Vector2 &result )
{ result = SmoothStep( v1, v2, t ); }

ASURA_INLINE
void SmoothStep( Vector3 &v1, Vector3 &v2, float t, Vector3 &result )
{ result = SmoothStep( v1, v2, t ); }

ASURA_INLINE
void SmoothStep( Vector4 &v1, Vector4 &v2, float t, Vector4 &result )
{ result = SmoothStep( v1, v2, t ); }

ASURA_INLINE
Vector2 Transform( Vector2 position, Matrix matrix )
{
	return Vector2(
		((position.x * matrix._11) + (position.y * matrix._21)) + matrix._41,
		((position.x * matrix._12) + (position.y * matrix._22)) + matrix._42 );
}

ASURA_INLINE
Vector3 Transform( Vector3 position, Matrix matrix )
{
	return Vector3(
		( ((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31)) + matrix._41,
		( ((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32)) + matrix._42,
		( ((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33)) + matrix._43 );
}

ASURA_INLINE
Vector4 Transform( Vector4 position, Matrix matrix )
{
	return Vector4(
		( ( ((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31) ) + (position.w * matrix._41)),
		( ( ((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32) ) + (position.w * matrix._42)),
		( ( ((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33) ) + (position.w * matrix._43)),
		( ( ((position.x * matrix._14) + (position.y * matrix._24)) + (position.z * matrix._43) ) + (position.w * matrix._44)) );
}

ASURA_INLINE
void Transform( Vector2 &position, Matrix &matrix, Vector2 &result )
{ result = Transform( position, matrix ); }

ASURA_INLINE
void Transform( Vector3 &position, Matrix &matrix, Vector3 &result )
{ result = Transform( position, matrix ); }

ASURA_INLINE
void Transform( Vector4 &position, Matrix &matrix, Vector4 &result )
{ result = Transform( position, matrix ); }

ASURA_INLINE
Vector2 TransformNormal( Vector2 normal, Matrix matrix )
{
	return Vector2(
		(normal.x * matrix._11) + (normal.y * matrix._21),
		(normal.x * matrix._12) + (normal.y * matrix._22) );
}

ASURA_INLINE
Vector3 TransformNormal( Vector3 normal, Matrix matrix )
{
	return Vector3(
		((normal.x * matrix._11) + (normal.y * matrix._21)) + (normal.z * matrix._31),
		((normal.y * matrix._12) + (normal.y * matrix._22)) + (normal.z * matrix._32),
		((normal.z * matrix._13) + (normal.y * matrix._23)) + (normal.z * matrix._33) );
}

ASURA_INLINE
void TransformNormal( Vector2 &normal, Matrix &matrix, Vector2 &result )
{ result = TransformNormal( normal, matrix ); }

ASURA_INLINE
void TransformNormal( Vector3 &normal, Matrix &matrix, Vector3 &result )
{ result = TransformNormal( normal, matrix ); }

ASURA_INLINE
Vector2 TransformCoordinate( Vector2 coords, Matrix matrix )
{
	Vector4 tmp( coords.x, coords.y, 0, 1 );
	Vector4 result = Transform( tmp, matrix );
	return Vector2( result.x, result.y );
}

ASURA_INLINE
Vector3 TransformCoordinate( Vector3 coords, Matrix matrix )
{
	Vector4 tmp( coords.x, coords.y, coords.z, 1 );
	Vector4 result = Transform( tmp, matrix );
	return Vector3( result.x, result.y, result.z );
}

ASURA_INLINE
void TransformCoordinate( Vector2 &coords, Matrix &matrix, Vector2 &result )
{ result = TransformCoordinate( coords, matrix ); }

ASURA_INLINE
void TransformCoordinate( Vector3 &coords, Matrix &matrix, Vector3 &result )
{ result = TransformCoordinate( coords, matrix ); }

} // namespace Math
} // namespace Asura

// End Of File.