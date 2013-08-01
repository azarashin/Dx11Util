//-----------------------------------------------------------------------
// File : Quaternion.inl
// Desc : Quaternion Inline Files
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
// Quaternion
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Quaternion::Quaternion()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

ASURA_INLINE
Quaternion::Quaternion( const float* pf )
{
	assert( pf != NULL );
	x = pf[ 0 ];
	y = pf[ 1 ];
	z = pf[ 2 ];
	w = pf[ 3 ];
}

ASURA_INLINE
Quaternion::Quaternion( float fx, float fy, float fz, float fw )
{ 
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

ASURA_INLINE
Quaternion::operator float* ()
{ return (float*)&x; }

ASURA_INLINE
Quaternion::operator const float* () const
{ return (const float*)&x; }

ASURA_INLINE Quaternion&
Quaternion::operator += ( const Quaternion& q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return (*this);
}

ASURA_INLINE Quaternion&
Quaternion::operator -= ( const Quaternion& q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return (*this);
}

ASURA_INLINE Quaternion&
Quaternion::operator *= ( const Quaternion& q )
{
	Quaternion tmp = (*this);
	(*this) = Multiply( tmp, q );
	return (*this);
}

ASURA_INLINE Quaternion&
Quaternion::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return (*this);
}

ASURA_INLINE Quaternion&
Quaternion::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return (*this);
}

ASURA_INLINE Quaternion
Quaternion::operator + () const
{ return (*this); }

ASURA_INLINE Quaternion
Quaternion::operator - () const
{ return Quaternion( -x, -y, -z, -w ); }

ASURA_INLINE Quaternion
Quaternion::operator + ( Quaternion q ) const
{ return Quaternion( x + q.x, y + q.y, z + q.z, w + q.z ); }

ASURA_INLINE Quaternion
Quaternion::operator - ( Quaternion q ) const
{ return Quaternion( x - q.x, y - q.y, z - q.z, w - q.z ); }

ASURA_INLINE Quaternion
Quaternion::operator * ( Quaternion q ) const
{ 
	Quaternion qT;
	qT = Multiply( (*this), q );
	return qT;
}

ASURA_INLINE Quaternion
Quaternion::operator * ( float f ) const
{ return Quaternion( x * f, y * f, z * f, w *f ); }

ASURA_INLINE Quaternion
Quaternion::operator / ( float f ) const
{ 
    assert( !IsNearZero( f ) );
	float fInv = 1.0f/f;
	return Quaternion( x * fInv, y * fInv, z * fInv, w * fInv ); 
}

ASURA_INLINE Quaternion
operator * ( float f, const Quaternion& q )
{ return Quaternion( f * q.x, f * q.y, f * q.z, f * q.w ); }

ASURA_INLINE bool
Quaternion::operator == ( const Quaternion& q ) const
{ return ( (x==q.x) && (y==q.y) && (z==q.z) && (w==q.w) ); }

ASURA_INLINE bool
Quaternion::operator != ( const Quaternion& q ) const
{ return ( (x!=q.x) || (y!=q.y) || (z!=q.z) || (w!=q.w) ); }

ASURA_INLINE float
Quaternion::Length() const
{ return sqrtf( x * x + y * y + z * z + w * w ); }

ASURA_INLINE float
Quaternion::LengthSquared() const
{ return ( x * x + y * y + z * z + w * w ); }

ASURA_INLINE void
Quaternion::Conjugate()
{ 
	x = -x;
	y = -y;
	z = -z;
}

ASURA_INLINE Quaternion&
Quaternion::Normalize()
{
    float mag = Length();
    assert( !IsNearZero( mag ) );
	float invMag = 1.0f/mag;
	x *= invMag;
	y *= invMag;
	z *= invMag;
	w *= invMag;
	return (*this);
}

/////////////////////////////////////////////////////////////////////////
// Quaternion Methods
//////////////////////////////////////////////////////////////////////////

ASURA_INLINE
float Dot( Quaternion q1, Quaternion q2 )
{ return (q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w); }

ASURA_INLINE
void Dot( Quaternion &q1, Quaternion &q2, float &result )
{ result = Dot( q1, q2 ); }

ASURA_INLINE
Quaternion Conjugate( Quaternion qua )
{ return Quaternion( -qua.x, -qua.y, -qua.z, qua.w ); }

ASURA_INLINE
void Conjugate( Quaternion &qua, Quaternion &result )
{ result = Conjugate( qua ); }

ASURA_INLINE
Quaternion Normalize( Quaternion qua )
{
	Quaternion result;
    float mag = qua.Length();
    assert( !IsNearZero( mag ) );
	float magInv = 1.0f/mag;
	result.x *= magInv;
	result.y *= magInv;
	result.z *= magInv;
	result.w *= magInv;
	return result;
}

ASURA_INLINE
void Normalize( Quaternion &qua, Quaternion &result )
{ result = Normalize( qua ); }

ASURA_INLINE
Quaternion Inverse( Quaternion qua )
{
	Quaternion result( -qua.x, -qua.y, -qua.z, qua.w );
	result.Normalize();
	return result;
}

ASURA_INLINE
void Inverse( Quaternion &qua, Quaternion &result )
{ result = Inverse( qua ); }

ASURA_INLINE
Quaternion CreateFromYawPitchRoll( float yaw, float pitch, float roll )
{
	Quaternion result;
	float sr = sinf( roll * 0.5f );
	float cr = cosf( roll * 0.5f );
	float sp = sinf( pitch * 0.5f );
	float cp = cosf( pitch * 0.5f );
	float sy = sinf( yaw * 0.5f );
	float cy = cosf( yaw * 0.5f );

	result.x = -(sy*sp*cr) + (cy*cp*sr);
	result.y = (cy*sp*cr) + (sy*cp*sr);
	result.z = -(cy*sp*sr) + (sy*cp*cr);
	result.w = (cy*cp*cr) + (sy*sp*sr);
	return result;
}

ASURA_INLINE
void CreateFromYawPitchRoll( float yaw, float pitch, float roll, Quaternion &result )
{ result = CreateFromYawPitchRoll( yaw, pitch, roll ); }
	
ASURA_INLINE
Quaternion Slerp( Quaternion q1, Quaternion q2, float t )
{
	if ( t <= 0.0f ) return q1;
	if ( t >= 1.0f ) return q2;

	float cosOmega = Dot(q1, q2);
	float q1x = q1.x;
	float q1y = q1.y;
	float q1z = q1.z;
	float q1w = q1.w;

	if ( cosOmega < 0.0f )
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega -= cosOmega;
	}

	float k1, k2;
	if ( cosOmega > 0.9999f )
	{
		k1 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrtf(1.0f - (cosOmega*cosOmega));
		float omega = atan2f(sinOmega, cosOmega);
		float sinOmegaInv  = 1.0f/sinOmega;
		k1 = sinf((1.0f-t)*omega)*sinOmegaInv;
		k2 = sinf(t*omega)*sinOmegaInv;
	}
	return Quaternion( 
		(k1*q1.x) + (k2*q1.x),
		(k1*q1.y) + (k2*q1.y),
		(k1*q1.z) + (k2*q1.z),
		(k1*q1.w) + (k2*q1.w)
		);
}

ASURA_INLINE
void Slerp( Quaternion &q1, Quaternion &q2, float t, Quaternion &result )
{ result = Slerp( q1, q2, t ); }

ASURA_INLINE
Quaternion Squad( Quaternion qua, Quaternion a, Quaternion b, Quaternion c, float t )
{ return Slerp( Slerp(qua, c, t ), Slerp( a, b, t ), 2.0f * t * ( 1.0f - t ) ); }

ASURA_INLINE
void Squad( Quaternion &qua, Quaternion &a, Quaternion &b, Quaternion &c, float t, Quaternion &result )
{ result = Squad( qua, a, b, c, t ); }

} // namespace Math
} // namespace Asura

// End Of File.