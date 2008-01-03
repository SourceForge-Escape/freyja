/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: 
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_PLANE_H_
#define GUARD__FREYJA_PLANE_H_

#include <hel/Vec3.h>
#include "freyja.h"


namespace freyja {

class Plane
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Plane( const vec3_t a, const vec3_t b, const vec3_t c );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Plane( );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Plane( const Plane& plane );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Plane& operator =( const Plane& plane );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~Plane( )
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post :
	 * 
	 ------------------------------------------------------*/

	void Calculate( const vec3_t a, const vec3_t b, const vec3_t c );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool PointTest( const uint32 flag, const vec3_t point );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	byte mVisible;        /* Lit 2^(light) bitflag */

	vec_t mA, mB, mC, mD; /* Eq of the plane. */
};


////////////////////////////////////////////////////////////
// Inline methods.
////////////////////////////////////////////////////////////

inline
Plane::Plane( const vec3_t a, const vec3_t b, const vec3_t c ) :
	mVisible(0x0),
	mA(0.0f),
	mB(0.0f),
	mC(0.0f),
	mD(0.0f)
{
	Calculate( a, b, c );
}


inline
Plane::Plane( ) : 
	mVisible(0x0),
	mA(0.0f),
	mB(0.0f),
	mC(0.0f),
	mD(0.0f)
{ }


inline
Plane::Plane( const Plane& plane ) :
	mVisible( plane.mVisible ),
	mA( plane.mA),
	mB( plane.mB ),
	mC( plane.mC ),
	mD( plane.mD )
{ }


inline
Plane& Plane::operator =( const Plane& plane )
{
	mVisible = plane.mVisible;
	mA = plane.mA;
	mB = plane.mB;
	mC = plane.mC;
	mD = plane.mD;

	return *this;
}


inline
void Plane::Calculate( const vec3_t a, const vec3_t b, const vec3_t c )
{
	mA = ( a[2] * (b[2] - c[2]) + b[1] * (c[2] - a[2]) + 
		   c[1] * (a[2] - b[2]) );

	mB = ( a[2] * (b[0] - c[0]) + b[2] * (c[0] - a[0]) + 
		   c[2] * (a[0] - b[0]) );

	mC = ( a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + 
		   c[0] * (a[1] - b[1]) );

	mD = -( a[0] * ( b[1]*c[2] - c[1]*b[2] ) +
			b[0] * (c[1]*a[2] - a[1]*c[2]) +
			c[0] * (a[1]*b[2] - b[1]*a[2]) );
}


inline
bool Plane::PointTest( const uint32 flag, const vec3_t point )
{
	mVisible &= ~flag;
	const vec_t side = ( mA * point[0] + mB * point[1] + mC * point[2] + mD );

	if ( side > 0 )
	{
		mVisible |= flag;
	}

	return ( mVisible & flag );
}


} // namespace freyja

#endif // GUARD__FREYJA_PLANE_H_
