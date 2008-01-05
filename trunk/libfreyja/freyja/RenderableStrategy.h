/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : RenderableStrategy
 * License : No use w/o permission (C) 2006-2007 Mongoose
 * Comments: Mostly an interface for renderable world objects.
 *
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.14:
 * Mongoose - New 0.9.6 design as a generic interface.
 *
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_RENDERABLESTRATEGY_H_
#define GUARD__FREYJA_RENDERABLESTRATEGY_H_

#include "freyja.h"


namespace freyja {

class RenderableStrategy
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	// FIXME: Convert these members to method wrappers!

	RenderableStrategy( ) :	
		mTriangleCount( 0 ),
		mIndexArraySize( 0 ),
		mIndexArrayPtr ( NULL ),
		mVertexWidth( 3 ),
		mVertexArraySize( 0 ),
		mVertexArrayPtr( NULL ),
		mTexcoordWidth( 2 ),
		mTexcoordArraySize( 0 ),
		mTexcoordArrayPtr( NULL ),
		mNormalArraySize( 0 ),
		mNormalArrayPtr( NULL )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~RenderableStrategy( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	virtual void DrawIcon( const char* name  ) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void DrawTriangles(  ) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 mTriangleCount;

	uint32 mIndexArraySize;
	uint32* mIndexArrayPtr;

	uint16 mVertexWidth;
	uint32 mVertexArraySize;
	vec_t* mVertexArrayPtr;
	
	uint16 mTexcoordWidth;
	uint32 mTexcoordArraySize;
	vec_t* mTexcoordArrayPtr;

	uint32 mNormalArraySize;
	vec_t* mNormalArrayPtr;
};


} // end namespace freyja

#endif // GUARD__FREYJA_RENDERABLESTRATEGY_H_
