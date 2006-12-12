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

using namespace mstl;


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

	Vertex(index_t vertex, index_t texcoord, index_t normal) :
		mFlags(fNone),
		mVertexIndex(vertex),
		mTexCoordIndex(texcoord),    
		mNormalIndex(normal),
		mMaterial(INDEX_INVALID),
		mFaceRefs(),
		mTmpRefs()
	{
	}

	Vertex() :
		mFlags(fNone),
		mVertexIndex(INDEX_INVALID),
		mTexCoordIndex(INDEX_INVALID),    
		mNormalIndex(INDEX_INVALID),
		mMaterial(INDEX_INVALID),
		mFaceRefs(),
		mTmpRefs()
	{
	}

	Vertex(const Vertex &v) :
		mFlags(v.mFlags),
		mVertexIndex(v.mVertexIndex),
		mTexCoordIndex(v.mTexCoordIndex),    
		mNormalIndex(v.mNormalIndex),
		mMaterial(v.mMaterial),
		mFaceRefs(v.mFaceRefs),
		mTmpRefs(v.mTmpRefs)
	{
	}

	static size_t SerializedSize() 
	{
		return ( 1 + 4 * 6); 
	}

	bool Serialize(SystemIO::FileWriter &w) 
	{ 
		freyja_file_chunk_t chunk;

		chunk.type = FREYJA_CHUNK_VERTEX;
		chunk.size = SerializedSize();
		chunk.flags = 0x0;
		chunk.version = 10;

		w.WriteInt8U(mFlags);
		w.WriteLong(mVertexIndex);
		w.WriteLong(mTexCoordIndex);
		w.WriteLong(mNormalIndex);
		w.WriteLong(mMaterial);

		// No use in storing face references to disk!

		return true; 
	}

	bool Serialize(SystemIO::TextFileWriter &w) 
	{ 
		w.Print("\t\tv %u %u %u %u %u\n", 
				mFlags, mVertexIndex, mTexCoordIndex, mNormalIndex, mMaterial);
		return true;
	}

	bool Serialize(SystemIO::TextFileReader &r) 
	{ 
		const char *symbol = r.ParseSymbol();
		if (symbol[0] != 'v')
			return false;

		mFlags = r.ParseInteger();
		mVertexIndex = r.ParseInteger();
		mTexCoordIndex = r.ParseInteger();
		mNormalIndex = r.ParseInteger();
		mMaterial = r.ParseInteger();

		return true;
	}

	
	void Meld(Vertex &v)
	{
		// We don't want transform operations on this anymore, so
		// we make it unselectable and hidden.
		mFlags = fHidden | fMuted;
		mVertexIndex = v.mVertexIndex;
		mTexCoordIndex = v.mTexCoordIndex;
		mNormalIndex = v.mNormalIndex;
		mMaterial = v.mMaterial;
	}
	
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
	
	Vector<index_t> &GetFaceRefs() { return mFaceRefs; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	Vector<index_t> &GetTmpRefs() { return mTmpRefs; }
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

	Vector<index_t> mFaceRefs;  /* Face references */

	Vector<index_t> mTmpRefs;   /* Face references used for special methods */
};

} // End namespace freyja

#endif // GUARD__FREYJA_VERTEX_H_
