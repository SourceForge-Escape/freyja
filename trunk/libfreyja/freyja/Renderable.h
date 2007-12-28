/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Renderable
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

#ifndef GUARD__FREYJA_RENDERABLE_H_
#define GUARD__FREYJA_RENDERABLE_H_

#include <hel/math.h>
#include <hel/Quat.h>
#include <hel/Vec3.h>

#include "Material.h"
#include "freyja.h"


namespace freyja {

class Renderable
{
public:

	typedef enum {
		fNone =  0,
		fHighlighted =  1,    /* This should be rendered with a highlight.*/
		fHidden = 2,          /* This hidden/not rendered on the client. */
		fShadowCaster = 8,    /* This generates shadow volumes. */   
		fShadowReceiver = 16, /* This receives shadow volumes. */
		fHasLoD = 32          /* This has Levels of Detail. */    

		// Doesn't really fit in this class.
		//fSelected = 64,       /* This is marked as selected. */
		//fRayHit = 128,        /* This was marked in a Ray hit test. */
	} RenderFlag;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Renderable( ) :
		mRenderFlags( fNone )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Renderable
	 *
	 ------------------------------------------------------*/

	virtual ~Renderable( )
	{ }
	/*------------------------------------------------------
	 * Pre  : Renderable object is allocated
	 * Post : Deconstructs an object of Renderable
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Cache API methods.
	////////////////////////////////////////////////////////////

	void ClearRenderFlag( const RenderFlag flag ) 
	{ mRenderFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears render option flag.
	 *
	 ------------------------------------------------------*/

	void SetRenderFlag( const RenderFlag flag ) 
	{ mRenderFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets render option flag.
	 *
	 ------------------------------------------------------*/

	const byte& GetRenderFlags( ) const
	{ return mRenderFlags; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option flag bitmap.
	 *
	 ------------------------------------------------------*/

	bool GetShadowCaster() const
	{ return ( mRenderFlags & fShadowCaster ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used for rendering shadow volumes.
	 *
	 ------------------------------------------------------*/

	bool GetShadowReceiver() const
	{ return ( mRenderFlags & fShadowReceiver ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used for rendering shadow volumes.
	 *
	 ------------------------------------------------------*/

	virtual const hel::Quat& GetWorldOrientation() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the final world orientation of this renderable.
	 *
	 ------------------------------------------------------*/

	virtual const hel::Vec3& GetWorldPosition() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the final world position of this renderable.
	 *
	 ------------------------------------------------------*/

	virtual freyja::Material* GetMaterial() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the Material pointer for this renderable.
	 *        This can be NULL.
	 *
	 ------------------------------------------------------*/


protected:

	byte mRenderFlags;
};


} // end namespace freyja

#   define FREYJA_RENDERABLE_INTERFACE \
	virtual const hel::Quat& GetWorldOrientation() const; \
	virtual const hel::Vec3& GetWorldPosition() const; \
	virtual freyja::Material* GetMaterial() const; 

#endif // GUARD__FREYJA_RENDERABLE_H_
