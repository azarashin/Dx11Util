//-----------------------------------------------------------------------
// File : Bounding.inl
// Desc : Bounding Inline Files
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
// BoundingBox structure
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
BoundingBox::BoundingBox()
{ /* DO_NOTHING */ }

ASURA_INLINE
BoundingBox::BoundingBox( Vector3 _minimum, Vector3 _maximum )
{
	minimum = _minimum;
	maximum = _maximum;
}

ASURA_INLINE
bool BoundingBox::operator == ( BoundingBox &box ) const
{ return ( maximum == box.maximum && minimum == box.minimum ); }

ASURA_INLINE
bool BoundingBox::operator != ( BoundingBox &box ) const
{ return ( maximum != box.maximum || minimum != box.minimum ); }

ASURA_INLINE
void BoundingBox::Merge( Vector3 value )
{
	maximum = Max( maximum, value );
	minimum = Min( minimum, value );
}

ASURA_INLINE
void BoundingBox::Merge( int size, Vector3 value[] )
{
    assert( value != 0 );
	for( int i = 0; i < size; ++i )
	{ Merge( value[ i ] ); }
}

/////////////////////////////////////////////////////////////////////////
// BoundingSphere structure
/////////////////////////////////////////////////////////////////////////

ASURA_INLINE
BoundingSphere::BoundingSphere()
{ /* DO_NOTING */ }

ASURA_INLINE
BoundingSphere::BoundingSphere( Vector3 _position, float _radius )
{
	position = _position;
	radius = _radius;
}

ASURA_INLINE
BoundingSphere::BoundingSphere( BoundingBox box )
{
	position = ( box.maximum + box.minimum ) * 0.5f;
	radius = Distance( box.maximum, box.minimum ) * 0.5f;
}

ASURA_INLINE
bool BoundingSphere::operator == ( BoundingSphere &sphere ) const
{ return ( position == sphere.position && radius == sphere.radius ); }

ASURA_INLINE
bool BoundingSphere::operator != ( BoundingSphere &sphere ) const
{ return ( position != sphere.position || radius != sphere.radius ); }

ASURA_INLINE
void BoundingSphere::Merge( Vector3 value )
{
	Vector3 tmp = value - position;
	float r = tmp.Length();
	if ( r > radius ) 
	{ 
		radius = r; 
		position = ( value + position ) * 0.5f;
	}
}

ASURA_INLINE
void BoundingSphere::Merge( int size, Vector3* value )
{
    assert( value != 0 );
	for( int i = 0; i < size; ++i )
	{ Merge( value[ i ] ); }
}

} // namespace Math
} // namespace Asura


// End Of File.