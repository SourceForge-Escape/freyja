/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Face
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Face class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_FACE_H_
#define GUARD__FREYJA_FACE_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "freyja.h"
#include "Track.h"

using namespace mstl;


namespace freyja {

class Face
{
public:

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16,
		fPolyMappedTexCoords = 32,
		fPolyMappedNormals = 64
	} Flags;


	Face() :
		mFlags(fNone),
		mSmoothingGroup(0),
		mColor(0),
		mMaterial(0), // Always have a valid material Id
		mIndices(),
		mTexCoordIndices(),
		mNormalsIndices()
	{
	}

	~Face()
	{
	}


	static const char *GetChunkType() { return "Face"; }

	static uint32 GetChunkVersion() { return 9500; /* 0.9.5-00 */ }


	void AppendNormal(index_t idx)
	{
		mFlags |= fPolyMappedNormals;
		mNormalsIndices.pushBack(idx);
	}


	void AppendTexCoord(index_t idx)
	{
		mFlags |= fPolyMappedTexCoords;
		mTexCoordIndices.pushBack(idx);
	}


	void AppendVertex(index_t idx)
	{
		// FIXME: Now you should append this Face's id to the vertex's polyref
		// FIXME: Might want to cache edge map too
		mIndices.pushBack(idx);
	}

	index_t FindCorrespondingTexCoord(index_t vertexIndex)
	{
		for (uint32 i = 0, n = mIndices.size(); i < n; ++i)
		{
			if (mIndices[i] == vertexIndex)
			{
				return mTexCoordIndices[i];
			}
		}
		
		return INDEX_INVALID;
	}

	void PurgePolyMappedTexCoords()
	{
		mFlags |= fPolyMappedTexCoords;
		mFlags ^= fPolyMappedTexCoords;
		mTexCoordIndices.clear();
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Disables polymapped texCoords for this face 
	 *
	 ------------------------------------------------------*/

	void ClearSmoothingGroup() { mSmoothingGroup = 0; }
	void SetSmoothingGroup(uint32 g) { mSmoothingGroup = g; }
	/*------------------------------------------------------
	 * Pre  : -1 < <g> < 33 ( Relates to power of 2, bitmap ) 
	 *        Groups:  1-24,    Normal smoothing groups
	 *                25-32,    Reserved for special use
	 *
	 *        NOTE Groups/Colors may later use crease flags in other
	 *             'geometery types'.
	 *
	 * Post : 
	 *
	 ------------------------------------------------------*/

	size_t SerializedSize() { return 0; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w) 
	{ 
		w.Print("\t\tface %u %u %u %u\n", 
				mFlags, mSmoothingGroup, mColor, mMaterial);

		uint32 i;
		w.Print("\t\t\tfv %u ", mIndices.size()); 
		foreach(mIndices, i)
		{
			w.Print("%u ", mIndices[i]);
		}
		w.Print("\n");

		w.Print("\t\t\tft %u ", mTexCoordIndices.size());
		foreach(mTexCoordIndices, i)
		{
			w.Print("%u ", mTexCoordIndices[i]);
		}
		w.Print("\n");

		w.Print("\t\t\tfn %u ", mNormalsIndices.size());
		foreach(mNormalsIndices, i)
		{
			w.Print("%u ", mNormalsIndices[i]);
		}
		w.Print("\n");

		return true;
	}

	bool Serialize(SystemIO::TextFileReader &r) 
	{ 
		const char *symbol = r.ParseSymbol();
		if (strncmp(symbol, "face", 4))
			return false;

		mFlags = r.ParseInteger();
		mSmoothingGroup = r.ParseInteger();
		mColor = r.ParseInteger();
		mMaterial = r.ParseInteger();

		r.ParseSymbol(); // "fv"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mIndices.push_back(r.ParseInteger());
		}

		r.ParseSymbol(); // "ft"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mTexCoordIndices.push_back(r.ParseInteger());
		}

		r.ParseSymbol(); // "fn"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mNormalsIndices.push_back(r.ParseInteger());
		}

		return true;
	}

	byte mFlags;                      /* Options flags */

	byte mSmoothingGroup;             /* Group bit index */

	byte mColor;                      /* Reserved use for future algorithms */

	index_t mMaterial;                /* Material index */

	Vector<index_t> mIndices;         /* Indices to the Vertex *class */

	Vector<index_t> mTexCoordIndices; /* Only used with fPolyMappedTexCoords */

	Vector<index_t> mNormalsIndices;  /* Only used with fPolyMappedNormals */
};

} // End namespace freyja


#endif // GUARD__FREYJA_FACE_H_
