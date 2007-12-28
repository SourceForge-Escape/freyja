/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : Vertex
 * Comments: 
 *           Owner: Mesh
 *           References: Faces
 *           Provides: Vertex, Texcoord, and Normal indices.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_VERTEX_H_
#define GUARD__FREYJA_VERTEX_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/list.h>

#include "XMLSerializer.h"
#include "freyja.h"


namespace freyja {


class Vertex
{
public:
	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16,
		fSelected2   = 32,   // Only used internally per method in Mesh
		fMuted       = 64

	} Flags;

	Vertex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Vertex( const Vertex& v );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~Vertex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize( XMLSerializerNode container );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize( XMLSerializerNode container );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Meld( Vertex& v );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	const byte& GetFlags( ) 
	{ return mFlags; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option flag bitmap.
	 *
	 ------------------------------------------------------*/

	void ClearFlag( Flags flag ) 
	{ mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag if not mute.
	 *
	 ------------------------------------------------------*/

	void SetFlag( Flags flag )
	{ mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag if not mute.
	 *
	 ------------------------------------------------------*/
	
	mstl::list<index_t>& GetFaceRefs() 
	{ return mFaceRefs; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	//void WeldTexCoords(index_t replace, index_t texcoord)
	//{ if (mTexCoordIndex == replace) mTexCoordIndex = texcoord;	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Attempt to replace texcoord reference.
	 *
	 ------------------------------------------------------*/


	byte mFlags;                /* State flags */

	// mSharedVertexIndex

	// mBufferIndex


protected:

	mstl::list<index_t> mIndices;   /* Buffer indices this Vertex object controls. */

	mstl::list<index_t> mFaceRefs;  /* Face references */
};

} // End namespace freyja

#endif // GUARD__FREYJA_VERTEX_H_
