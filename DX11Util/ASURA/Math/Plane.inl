//-----------------------------------------------------------------------
// File : Plane.inl
// Desc : Plane Inline Files
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
// Plane
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE 
Plane::Plane( const float* pf )
{
	assert( pf != NULL );
	a = pf[ 0 ];
	b = pf[ 1 ];
	c = pf[ 2 ];
	d = pf[ 3 ];
}

ASURA_INLINE
Plane::Plane( float fa, float fb, float fc, float fd )
{
	a = fa;
	b = fb;
	c = fc;
	d = fd;
}

ASURA_INLINE
Plane::operator float* ()
{ return (float*)&a; }

ASURA_INLINE
Plane::operator const float* () const
{ return (const float*)&a; }

ASURA_INLINE Plane&
Plane::operator *= ( float f )
{ 
	a *= f;
	b *= f;
	c *= f;
	d *= f;
	return (*this);
}

ASURA_INLINE Plane&
Plane::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f/f;
	a *= fInv;
	b *= fInv;
	c *= fInv;
	d *= fInv;
	return (*this);
}

ASURA_INLINE Plane
Plane::operator + () const
{ return (*this); }

ASURA_INLINE Plane
Plane::operator - () const
{ return Plane( -a, -b, -c, -d ); }

ASURA_INLINE Plane
Plane::operator * ( float f ) const
{ return Plane( a * f, b * f, c * f, d * f ); }

ASURA_INLINE Plane
Plane::operator / ( float f ) const
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f/f;
	return Plane( a * fInv, b * fInv, c * fInv, d * fInv ); 
}

ASURA_INLINE Plane
operator * ( float f, const Plane& p )
{ return Plane( f * p.a, f * p.b, f * p.c, f * p.d ); }

ASURA_INLINE bool
Plane::operator == ( const Plane& p ) const
{ return ( (a==p.a) && (b==p.b) && (c==p.c) && (d==p.d) ); }

ASURA_INLINE bool
Plane::operator != ( const Plane& p ) const
{ return ( (a!=p.a) || (b!=p.b) || (c!=p.c) || (d!=p.d) ); }


ASURA_INLINE Plane&
Plane::Normalize()
{
	float mag = sqrtf( a*a + b*b + c*c );
    assert( !IsNearZero( mag ) );
	float magInv = 1.0f / mag;
	a *= magInv;
	b *= magInv;
	c *= magInv;
	d *= magInv;
	return (*this);
}

ASURA_INLINE Plane&
Plane::FromPointNormal( const Vector3 &point, const Vector3 &normal )
{
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -Dot( point, normal );
   
    return (*this);
}

ASURA_INLINE Plane&
Plane::FromPoints( const Vector3 &p0, const Vector3 &p1, const Vector3 &p2 )
{
    Vector3 edge1, edge2, normal, Nnormal;
    
    edge1 = p1 - p0;
    edge2 = p2 - p0;
    normal = Cross( edge1, edge2 );
    Nnormal = normal.Normalize();
    return FromPointNormal( p0, Nnormal );
}

ASURA_INLINE bool
Plane::Inside( const Vector3 &point ) const
{
    float result = DotCoordinate( (*this), point );
    return ( result >= 0.0f );
}

ASURA_INLINE bool
Plane::Inside( const Vector3 &point, const float radius ) const
{
    float dist = DotCoordinate( (*this), point );
    return ( dist >= -radius );
}

/////////////////////////////////////////////////////////////////////////
// Plane Methods
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
float Dot( Plane p, Vector4 v )
{ return (p.a*v.x + p.b*v.y + p.c*v.z + p.d*v.w); }

ASURA_INLINE
void Dot( Plane &p, Vector4 &v, float &result )
{ result = Dot( p, v ); }

ASURA_INLINE
float DotCoordinate( Plane p, Vector3 v )
{ return (p.a*v.x + p.b*v.y + p.c*v.z + p.d); }

ASURA_INLINE
void DotCoordinate( Plane &p, Vector3 &v, float &result )
{ result = DotCoordinate( p, v ); }

ASURA_INLINE 
float DotNormal( Plane p, Vector3 v )
{ return (p.a*v.x + p.b*v.y + p.c*v.z); }

ASURA_INLINE 
void DorNormal( Plane &p, Vector3 &v, float &result )
{ result = DotNormal( p, v ); }

ASURA_INLINE 
Plane Normalize( Plane p )
{
	Plane result = p;
    float mag = ( p.a*p.a + p.b*p.b + p.c*p.c );
    assert( !IsNearZero( mag ) );
	float magInv = 1.0f/mag;
	result.a *= magInv;
	result.b *= magInv;
	result.c *= magInv;
	result.d *= magInv;
	return result;
}

ASURA_INLINE
void Normalize( Plane &p, Plane &result )
{ result = Normalize( p ); }

} // namespace Math
} // namespace Asura

// End Of File.
