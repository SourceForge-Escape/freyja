/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Vertex
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Vertex class.
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
#include <mstl/SystemIO.h>
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

	Vertex(index_t vertex, index_t texcoord, index_t normal);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Vertex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Vertex(const Vertex &v);
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

	static size_t SerializedSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(mstl::SystemIO::FileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(mstl::SystemIO::TextFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(mstl::SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Meld(Vertex &v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	const byte &GetFlags() { return mFlags; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option flag bitmap.
	 *
	 ------------------------------------------------------*/

	void ClearFlag(Flags flag) { if (!(mFlags & fMuted)) mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag if not mute.
	 *
	 ------------------------------------------------------*/

	void SetFlag(Flags flag) { if (!(mFlags & fMuted)) mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag if not mute.
	 *
	 ------------------------------------------------------*/

	bool IsMuted() { return (mFlags & fMuted); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 'Unlocks' current option flags.
	 *
	 ------------------------------------------------------*/

	void ClearMuted() { mFlags &= ~fMuted; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 'Unlocks' current option flags.
	 *
	 ------------------------------------------------------*/

	void SetMuted() { mFlags |= fMuted; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 'Locks' current option flags.
	 *
	 ------------------------------------------------------*/
	
	mstl::Vector<index_t> &GetFaceRefs() { return mFaceRefs; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	mstl::Vector<index_t> &GetTmpRefs() { return mTmpRefs; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Temp references.
	 *
	 ------------------------------------------------------*/


	byte mFlags;                /* State flags */

	index_t mVertexIndex; 		/* Pool storage of XYZ position */

	index_t mTexCoordIndex;     /* Pool storage of UV[W] coordinate */

	index_t mNormalIndex; 		/* Pool storage of XYZ normal */

	index_t mMaterial;          /* Material index */

private:

	mstl::Vector<index_t> mFaceRefs;  /* Face references */

	mstl::Vector<index_t> mTmpRefs;   /* Used for special methods */
};

} // End namespace freyja

#endif // GUARD__FREYJA_VERTEX_H_
