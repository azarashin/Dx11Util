//-----------------------------------------------------------------------
// File : Matrix.inl
// Desc : Matrix Inline Files
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
// Matrix structure
/////////////////////////////////////////////////////////////////////////
ASURA_INLINE
Matrix::Matrix()
{ /* DO_NOTHING */ }

ASURA_INLINE
Matrix::Matrix( const float* pf )
{
	assert( pf != NULL );
	memcpy( &_11, pf, sizeof(Matrix) );
}

ASURA_INLINE
Matrix::Matrix( float f11, float f12, float f13, float f14,
			    float f21, float f22, float f23, float f24,
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44 )
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

ASURA_INLINE float&
Matrix::operator () ( unsigned int iRow, unsigned int iCol )
{ return m[iRow][iCol]; }

ASURA_INLINE float
Matrix::operator () ( unsigned int iRow, unsigned int iCol ) const
{ return m[iRow][iCol]; }

ASURA_INLINE
Matrix::operator float* ()
{ return (float*)&_11; }

ASURA_INLINE
Matrix::operator const float* () const 
{ return (const float*)&_11; }

ASURA_INLINE Matrix&
Matrix::operator *= ( Matrix mat )
{
	Matrix tmp = (*this);
	(*this) = Multiply( tmp, mat );
	return (*this);
}

ASURA_INLINE Matrix&
Matrix::operator += ( const Matrix& mat )
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return (*this);
}

ASURA_INLINE Matrix&
Matrix::operator -= ( const Matrix& mat )
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return (*this);
}

ASURA_INLINE Matrix&
Matrix::operator *= ( float f )
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return (*this);
}

ASURA_INLINE Matrix&
Matrix::operator /= ( float f )
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f/f;
	_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
	_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
	_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
	_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
	return (*this);
}

ASURA_INLINE Matrix
Matrix::operator + () const
{ 
    return Matrix( _11, _12, _13, _14,
                   _21, _22, _23, _24,
                   _31, _32, _33, _34,
                   _41, _42, _43, _44 );
}

ASURA_INLINE Matrix
Matrix::operator - () const
{
	return Matrix( -_11, -_12, -_13, -_14,
				   -_21, -_22, -_23, -_24,
				   -_31, -_32, -_33, -_34,
				   -_41, -_42, -_43, -_44 );
}

ASURA_INLINE Matrix
Matrix::operator * ( Matrix mat ) const
{
    Matrix tmp = (*this);
	Matrix result = Multiply( tmp, mat );
	return result;
}

ASURA_INLINE Matrix
Matrix::operator + ( Matrix mat ) const
{
	return Matrix( _11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
				   _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
				   _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
				   _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44 );
}

ASURA_INLINE Matrix
Matrix::operator - ( Matrix mat ) const
{
	return Matrix( _11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
				   _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
				   _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
				   _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44 );
}

ASURA_INLINE Matrix
Matrix::operator * ( float f ) const
{
	return Matrix( _11 * f, _12 * f, _13 * f, _14 * f,
			       _21 * f, _22 * f, _23 * f, _24 * f,
				   _31 * f, _32 * f, _33 * f, _34 * f,
				   _41 * f, _42 * f, _43 * f, _44 * f );
}

ASURA_INLINE Matrix
Matrix::operator / ( float f ) const
{
    assert( !IsNearZero( f ) );
	float fInv = 1.0f/f;
	return Matrix( _11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
				   _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
				   _31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
				   _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv );
}

ASURA_INLINE Matrix
operator * ( float f, const Matrix& mat )
{
	return Matrix( f * mat._11, f * mat._12, f * mat._13, f * mat._14,
				   f * mat._21, f * mat._22, f * mat._23, f * mat._24,
				   f * mat._31, f * mat._32, f * mat._33, f * mat._34,
				   f * mat._41, f * mat._42, f * mat._43, f * mat._44 );
}

ASURA_INLINE bool
Matrix::operator == ( const Matrix& mat ) const
{ return ( 0 == memcmp( this, &mat, sizeof( Matrix ) ) ); }

ASURA_INLINE bool
Matrix::operator != ( const Matrix& mat ) const
{ return ( 0 != memcmp( this, &mat, sizeof( Matrix ) ) ); }

ASURA_INLINE float
Matrix::Determinant() const
{
	float det =
		_11*_22*_33*_44 + _11*_23*_34*_42 +
		_11*_24*_32*_43	+ _12*_21*_34*_43 +
		_12*_23*_31*_44	+ _12*_24*_33*_41 +
		_13*_21*_32*_44	+ _13*_22*_34*_41 +
		_13*_24*_31*_42 + _14*_21*_33*_42 +
		_14*_22*_31*_43 + _14*_23*_32*_41 -
		_11*_22*_34*_43 - _11*_23*_32*_44 -
		_11*_24*_33*_42 - _12*_21*_33*_44 -
		_12*_23*_34*_41 - _12*_24*_31*_43 -
		_13*_21*_34*_42 - _13*_22*_31*_44 -
		_13*_24*_32*_41 - _14*_21*_32*_43 -
		_14*_22*_33*_41 - _14*_23*_31*_42;
	return det;
}

ASURA_INLINE void
Matrix::Identity()
{
	_11 = _22 = _33 = _44 = 1.0f;
	_12 = _13 = _14 =
	_21 = _23 = _24 =
	_31 = _32 = _34 =
	_41 = _42 = _43 = 0.0f;
}

/////////////////////////////////////////////////////////////////////////
// Matrix Methods
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
Matrix& Identity( Matrix &value )
{
	value.m[0][1] = value.m[0][2] = value.m[0][3] =
	value.m[1][0] = value.m[1][2] = value.m[1][3] =
	value.m[2][0] = value.m[2][1] = value.m[2][3] =
	value.m[3][0] = value.m[3][1] = value.m[3][2] = 0.0f;

	value.m[0][0] = value.m[1][1] = value.m[2][2] = value.m[3][3] = 1.0f;
	return value;
}

ASURA_INLINE
bool IsIdentity( const Matrix &value )
{
	return (
		IsNearlyEqual( value.m[0][0], 1.0f ) && IsNearZero( value.m[0][1] ) && IsNearZero( value.m[0][2] ) && IsNearZero( value.m[0][3] ) &&
		IsNearZero( value.m[1][0] ) && IsNearlyEqual( value.m[1][1], 1.0f ) && IsNearZero( value.m[1][2] ) && IsNearZero( value.m[1][3] ) &&
		IsNearZero( value.m[2][0] ) && IsNearZero( value.m[2][1] ) && IsNearlyEqual( value.m[2][2] , 1.0f ) && IsNearZero( value.m[2][3] ) &&
		IsNearZero( value.m[3][0] ) && IsNearZero( value.m[3][1] ) && IsNearZero( value.m[3][2] ) && IsNearlyEqual( value.m[3][3], 1.0f ) );
}

ASURA_INLINE
Matrix Transpose( Matrix value )
{
	return Matrix(
		value._11, value._21, value._31, value._41,
		value._12, value._22, value._32, value._42,
		value._13, value._23, value._33, value._43,
		value._14, value._24, value._34, value._44 );
}

ASURA_INLINE
void Transpose( Matrix &value, Matrix &result )
{ result = Transpose( value ); }

ASURA_INLINE
Matrix Multiply( Matrix value1, Matrix value2 )
{
	Matrix tmp;
	for ( int i=0; i<4; i++ )
		for ( int j=0; j<4; j++ )
			tmp.m[i][j] = value1.m[i][0] * value2.m[0][j] +
						  value1.m[i][1] * value2.m[1][j] +
						  value1.m[i][2] * value2.m[2][j] +
						  value1.m[i][3] * value2.m[3][j];
	return tmp;
}

ASURA_INLINE
void Multiply( Matrix &value1, Matrix &value2, Matrix &result )
{ result = Multiply( value1, value2 ); }

ASURA_INLINE
Matrix Multiply( Matrix value, float scaleFactor )
{
	return Matrix(
		value._11*scaleFactor, value._12*scaleFactor, value._13*scaleFactor, value._14*scaleFactor,
		value._21*scaleFactor, value._22*scaleFactor, value._23*scaleFactor, value._24*scaleFactor,
		value._31*scaleFactor, value._32*scaleFactor, value._33*scaleFactor, value._34*scaleFactor,
		value._41*scaleFactor, value._42*scaleFactor, value._43*scaleFactor, value._44*scaleFactor
		);
}

ASURA_INLINE
void Multiply( Matrix &value, float scaleFactor, Matrix &result )
{ result = Multiply( value, scaleFactor ); }

ASURA_INLINE
Matrix MultiplyTranspose( Matrix value1, Matrix value2 )
{
	Matrix result;
	result = Multiply( value1, value2 );
	result = Transpose( result );
	return result;
}

ASURA_INLINE
void MultiplyTranspose( Matrix &value1, Matrix &value2, Matrix &result )
{ result = MultiplyTranspose( value1, value2 ); }

ASURA_INLINE 
Matrix Invert( Matrix value )
{
	Matrix result;
    float det = value.Determinant();
    assert( !IsNearZero( det ) );
	float invDet = 1.0f / det;

	result._11 = value._22*value._33*value._44 + value._23*value._34*value._42 + value._24*value._32*value._43 - value._22*value._34*value._43 - value._23*value._32*value._44 - value._24*value._33*value._42;
	result._12 = value._12*value._34*value._43 + value._13*value._32*value._44 + value._14*value._33*value._42 - value._12*value._33*value._44 - value._13*value._34*value._42 - value._14*value._32*value._43;
	result._13 = value._12*value._23*value._44 + value._13*value._24*value._42 + value._14*value._22*value._43 - value._12*value._24*value._43 - value._13*value._22*value._44 - value._14*value._23*value._42;
	result._14 = value._12*value._24*value._33 + value._13*value._22*value._34 + value._14*value._23*value._32 - value._12*value._23*value._34 - value._13*value._24*value._32 - value._14*value._22*value._33;

	result._21 = value._21*value._34*value._43 + value._23*value._31*value._44 + value._24*value._33*value._41 - value._21*value._33*value._44 - value._23*value._34*value._41 - value._24*value._31*value._43;
	result._22 = value._11*value._33*value._44 + value._13*value._34*value._41 + value._14*value._31*value._43 - value._11*value._34*value._43 - value._13*value._31*value._44 - value._14*value._33*value._41;
	result._23 = value._11*value._24*value._43 + value._13*value._21*value._44 + value._14*value._23*value._41 - value._11*value._23*value._44 - value._13*value._24*value._41 - value._14*value._21*value._43;
	result._24 = value._11*value._23*value._34 + value._13*value._24*value._31 + value._14*value._21*value._33 - value._11*value._24*value._33 - value._13*value._21*value._34 - value._14*value._23*value._31;

	result._31 = value._21*value._32*value._44 + value._22*value._34*value._41 + value._24*value._31*value._42 - value._21*value._34*value._42 - value._22*value._31*value._44 - value._24*value._32*value._41;
	result._32 = value._11*value._34*value._42 + value._12*value._31*value._44 + value._14*value._32*value._41 - value._11*value._32*value._44 - value._12*value._34*value._41 - value._14*value._31*value._42;
	result._33 = value._11*value._22*value._44 + value._12*value._24*value._41 + value._14*value._21*value._42 - value._11*value._24*value._42 - value._12*value._21*value._44 - value._14*value._22*value._41;
	result._34 = value._11*value._24*value._32 + value._12*value._21*value._34 + value._14*value._22*value._31 - value._11*value._22*value._34 - value._12*value._24*value._31 - value._14*value._21*value._32;

	result._41 = value._21*value._33*value._42 + value._22*value._31*value._43 + value._23*value._32*value._41 - value._21*value._32*value._43 - value._22*value._33*value._41 - value._23*value._31*value._42;
	result._42 = value._11*value._32*value._43 + value._12*value._33*value._41 + value._13*value._31*value._42 - value._11*value._33*value._42 - value._12*value._31*value._43 - value._13*value._32*value._41;
	result._43 = value._11*value._23*value._42 + value._12*value._21*value._43 + value._13*value._22*value._41 - value._11*value._22*value._43 - value._12*value._23*value._41 - value._13*value._21*value._42;
	result._44 = value._11*value._22*value._33 + value._12*value._23*value._31 + value._13*value._21*value._32 - value._11*value._23*value._32 - value._12*value._21*value._33 - value._13*value._22*value._31;

	result = invDet * result;
	return result;
}

ASURA_INLINE
void Invert( Matrix &value, Matrix &result )
{ result = Invert( value ); }

ASURA_INLINE
Matrix CreateScale( float scale )
{
	return Matrix(
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

ASURA_INLINE 
void CreateScale( float scale, Matrix &result )
{ result = CreateScale( scale ); }

ASURA_INLINE 
Matrix CreateScale( float xScale, float yScale, float zScale )
{
	return Matrix(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zScale, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

ASURA_INLINE
void CreateScale( float xScale, float yScale, float zScale, Matrix &result )
{ result = CreateScale( xScale, yScale, zScale ); }

ASURA_INLINE
Matrix CreateScale( Vector3 scales )
{
	return Matrix(
		scales.x, 0.0f, 0.0f, 0.0f,
		0.0f, scales.y, 0.0f, 0.0f,
		0.0f, 0.0f, scales.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f ); 
}

ASURA_INLINE
void CreateScale( Vector3 &scales, Matrix &result )
{ result = CreateScale( scales ); }

ASURA_INLINE 
Matrix CreateTranslation( float xPos, float yPos, float zPos )
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		xPos, yPos, zPos, 1.0f );
}

ASURA_INLINE
void CreateTranslation( float xPos, float yPos, float zPos, Matrix &result )
{ result = CreateTranslation( xPos, yPos, zPos ); }

ASURA_INLINE
Matrix CreateTranslation( Vector3 pos )
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f );
}

ASURA_INLINE
void CreateTranslation( Vector3 &pos, Matrix &result )
{ result = CreateTranslation( pos ); }
 

ASURA_INLINE 
Matrix CreateRotationX( float radian )
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(radian), sinf(radian), 0.0f,
		0.0f, -sinf(radian), cosf(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

ASURA_INLINE 
void CreateRotationX( float radian, Matrix &result )
{ result = CreateRotationX( radian ); }

ASURA_INLINE 
Matrix CreateRotationY( float radian )
{
	return Matrix(
		cosf(radian), 0.0f, -sinf(radian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(radian), 0.0f, cosf(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

ASURA_INLINE
void CreateRotationY( float radian, Matrix &result )
{ result = CreateRotationY( radian ); }

ASURA_INLINE
Matrix CreateRotationZ( float radian )
{
	return Matrix( 
		cosf(radian), sinf(radian), 0.0f, 0.0f,
		-sinf(radian), cosf(radian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

ASURA_INLINE
void CreateRotationZ( float radian, Matrix &result )
{ result = CreateRotationZ( radian ); }

ASURA_INLINE
Matrix CreateFromQuaternion( Quaternion qua )
{
	Matrix result;
	float xx = qua.x*qua.x;
	float yy = qua.y*qua.y;
	float zz = qua.z*qua.z;
	float xy = qua.x*qua.y;
	float yw = qua.y*qua.w;
	float yz = qua.y*qua.z;
	float xw = qua.x*qua.w;
	float zx = qua.z*qua.x;
	float zw = qua.z*qua.w;

	result._11 = 1.0f - (2.0f * (yy + zz));
	result._12 = 2.0f * (xy + zw);
	result._13 = 2.0f * (zx - yw);
	result._14 = 0.0f;

	result._21 = 2.0f * (xy - zw);
	result._22 = 1.0f - (2.0f * (zz + xx)); 
	result._23 = 2.0f * (yz + xw);
	result._24 = 0.0f;

	result._31 = 2.0f * (zx + yw); 
	result._32 = 2.0f * (yz - xw);
	result._33 = 1.0f - (2.0f * (yy + xx));
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
	
	return result;
}

ASURA_INLINE
void CreateFromQuaternion( Quaternion &qua, Matrix &result )
{ result = CreateFromQuaternion( qua ); }

ASURA_INLINE
Matrix CreateFromAxisAngle( Vector3 axis, float radian )
{
	Matrix result;
	float sine = sinf(radian);
	float cosine = cosf(radian);
	float ab, bc, ca, tx, ty, tz, a = 1.0f -cosine;
	ab = axis.x * axis.y * a;
	bc = axis.y * axis.z * a;
	ca = axis.z * axis.x * a;
	tx = axis.x * axis.x;
	ty = axis.y * axis.y;
	tz = axis.z * axis.z;

	result._11 = tx + cosine * (1.0f - tx);
	result._12 = ab + axis.z * sine;
	result._13 = ca - axis.y * sine;
	result._14 = 0.0f;

	result._21 = ab - axis.z * sine;
	result._22 = ty + cosine * (1.0f - ty);
	result._23 = bc + axis.x * sine;
	result._24 = 0.0f;

	result._31 = ca + axis.y * sine;
	result._32 = bc - axis.x * sine;
	result._33 = tz + cosine * (1.0f - tz);
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;

	return result;
}

ASURA_INLINE
void CreateFromAxisAngle( Vector3 &axis, float radian, Matrix &result )
{ result = CreateFromAxisAngle( axis, radian ); }

ASURA_INLINE
Matrix CreateLookAt( Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector )
{
	Matrix result;
	Vector3 zaxis = cameraPosition - cameraTarget;
	zaxis.Normalize();

	Vector3 xaxis = Cross(cameraUpVector, zaxis);
	xaxis.Normalize();

	Vector3 yaxis = Cross(zaxis, xaxis);
	yaxis.Normalize();

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -Dot(xaxis, cameraPosition);
	result._42 = -Dot(yaxis, cameraPosition);
	result._43 = -Dot(zaxis, cameraPosition);
	result._44 = 1.0f;

	return result;
}

ASURA_INLINE
void CreateLookAt( Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector, Matrix &result )
{ result = CreateLookAt( cameraPosition, cameraTarget, cameraUpVector ); }

ASURA_INLINE 
Matrix CreatePerspective( float width, float height, float nearPlaneDistance, float farPlaneDistance )
{
    assert( !IsNearZero( width ) );
    assert( !IsNearZero( height ) );
    assert( !IsNearZero( nearPlaneDistance - farPlaneDistance ) );
	Matrix result;
	result._11 = 2.0f*nearPlaneDistance/width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f*nearPlaneDistance/height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = farPlaneDistance/(nearPlaneDistance - farPlaneDistance);
	result._34 = (nearPlaneDistance*farPlaneDistance)/(nearPlaneDistance -farPlaneDistance);
	
	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = -1.0f;
	result._44 = 0.0f;

	return result;
}

ASURA_INLINE
void CreatePerspective( float width, float height, float nearPlaneDistance, float farPlaneDistance, Matrix &result )
{ result = CreatePerspective( width, height, nearPlaneDistance, farPlaneDistance ); }

ASURA_INLINE
Matrix CreatePerspectiveFieldOfView( float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance )
{
    assert( !IsNearZero( aspectRatio ) );
    assert( !IsNearZero( nearPlaneDistance - farPlaneDistance ) );
	Matrix result;
	float yScale = 1.0f/tanf(fieldOfView/2.0f);
	float xScale = yScale/aspectRatio;
	result._11 = xScale;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = yScale;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = farPlaneDistance/(nearPlaneDistance - farPlaneDistance);
	result._34 = -1.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = (nearPlaneDistance*farPlaneDistance)/(nearPlaneDistance - farPlaneDistance);
	result._44 = 0.0f;

	return result;
}

ASURA_INLINE
void CreatePerspectiveFieldOfView( float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance, Matrix &result )
{ result = CreatePerspectiveFieldOfView( fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance ); }

ASURA_INLINE
Matrix CreatePerspectiveOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance )
{
    assert( !IsNearZero( right - left ) );
    assert( !IsNearZero( top - bottom ) );
    assert( !IsNearZero( nearPlaneDistance - farPlaneDistance ) );
	Matrix result;
	result._11 = 2.0f*nearPlaneDistance/(right - left);
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f*nearPlaneDistance/(top - bottom);
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = (left + right)/(right - left);
	result._32 = (top + bottom)/(top -bottom);
	result._33 = farPlaneDistance/(nearPlaneDistance - farPlaneDistance);
	result._34 = -1.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = nearPlaneDistance*farPlaneDistance/(nearPlaneDistance - farPlaneDistance);
	result._44 = 0.0f;

	return result;
}

ASURA_INLINE
void CreatePerspectiveOffcenter( float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance, Matrix &result )
{ result = CreatePerspectiveOffcenter( left, right, bottom, top, nearPlaneDistance, farPlaneDistance ); }

ASURA_INLINE
Matrix CreateOrthographic( float width, float height, float nearPlaneDistance, float farPlaneDistance )
{
    assert( !IsNearZero( width ) );
    assert( !IsNearZero( height ) );
    assert( !IsNearZero( nearPlaneDistance - farPlaneDistance ) );

	Matrix result;
	result._11 = 2.0f/width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;
	
	result._21 = 0.0f;
	result._22 = 2.0f/height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f/(nearPlaneDistance - farPlaneDistance);
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = nearPlaneDistance/(nearPlaneDistance - farPlaneDistance);
	result._44 = 1.0f;

	return result;
}

ASURA_INLINE
void CreateOrthographic( float width, float height, float nearPlaneDistance, float farPlaneDistance, Matrix &result )
{ result = CreateOrthographic( width, height, nearPlaneDistance, farPlaneDistance ); }

ASURA_INLINE
Matrix CreateShadow( Vector4 lightDirection, Plane plane )
{
	Matrix result;
	Plane p = Normalize( plane );
	Vector4 l = Normalize( lightDirection );
	float d = Dot( p, l );

	result._11 = p.a * l.x + d;
	result._12 = p.a * l.y;
	result._13 = p.a * l.z;
	result._14 = p.a * l.w;

	result._21 = p.b * l.x;
	result._22 = p.b * l.y + d;
	result._23 = p.b * l.z;
	result._24 = p.b * l.w;

	result._31 = p.c * l.x;
	result._32 = p.c + l.y;
	result._33 = p.c * l.z + d;
	result._34 = p.c * l.w;

	result._41 = p.d * l.x;
	result._42 = p.d * l.y;
	result._43 = p.d * l.z;
	result._44 = p.d * l.w + d;

	return result;
}

ASURA_INLINE
void CreateShadow( Vector4 &lightDirection, Plane &plane, Matrix &result )
{ result = CreateShadow( lightDirection, plane ); }

ASURA_INLINE
Matrix CreateReflect( Plane plane )
{
	Matrix result;
	result._11 = -2.0f * plane.a * plane.a + 1.0f;
	result._12 = -2.0f * plane.b * plane.a;
	result._13 = -2.0f * plane.c * plane.a;
	result._14 = 0.0f;

	result._21 = -2.0f * plane.a * plane.b;
	result._22 = -2.0f * plane.b * plane.b + 1.0f;
	result._23 = -2.0f * plane.c * plane.b;
	result._24 = 0.0f;

	result._31 = -2.0f * plane.a * plane.c;
	result._32 = -2.0f * plane.b * plane.c;
	result._33 = -2.0f * plane.c * plane.c + 1.0f;
	result._34 = 0.0f;

	result._41 = -2.0f * plane.a * plane.d;
	result._42 = -2.0f * plane.b * plane.d;
	result._43 = -2.0f * plane.c * plane.d;
	result._44 = 1.0f;
	return result;
}

ASURA_INLINE
void CreateReflect( Plane &plane, Matrix &result )
{ result = CreateReflect( plane ); }

ASURA_INLINE
Matrix Lerp( Matrix value1, Matrix value2, float amount )
{
	Matrix result;
	result = (1.0f - amount) * value1 + amount * value2;
	return result;
}

ASURA_INLINE
void Lerp( Matrix &value1, Matrix &value2, float amount, Matrix &result )
{ result = Lerp( value1, value2, amount ); }

} // namespace Math
} // namespace Asura


// End Of File.
