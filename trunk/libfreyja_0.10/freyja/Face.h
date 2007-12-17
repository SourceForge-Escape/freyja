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
#include <hel/Vec3.h>
#include <mstl/Vector.h>
#include "freyja.h"

namespace freyja {

class Edge;
class Vertex;

class Face
{
public:

	typedef enum {
		fNone      =  0,
		fRayHit    =  1,
		fAlpha     =  2
	} Flags;


	Face();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~Face();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void AddVertex( freyja::Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const int16 FindVertex( freyja::Vertex* vertex ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns element index or -1 if not found.
	 *
	 ------------------------------------------------------*/

	void ReplaceVertex( freyja::Vertex* replace, freyja::Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const byte GetSmoothingGroup( ) const 
	{ return mSmoothingGroup; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetSmoothingGroup( const byte group ) 
	{ mSmoothingGroup = group; }
	/*------------------------------------------------------
	 * Pre  : Groups:  1-24,    Normal smoothing groups
	 *                25-32,    Reserved for special use
	 * Post : 
	 * Note : Groups may later be used to autoassign edge
	 *        flags like crease, dart, hard/corner, etc.
	 *
	 ------------------------------------------------------*/

	void SetLightVisible( uint16 light_id )
	{ mVisible &= (1 << light_id); }
	/*------------------------------------------------------
	 * Pre  : 0 < light_id < 7
	 * Post : Visbility test result cached in this face.
	 *
	 ------------------------------------------------------*/
	
	void SetLightInvisible( uint16 light_id )
	{ mVisible &= ~(1 << light_id); }
	/*------------------------------------------------------
	 * Pre  : 0 < light_id < 7
	 * Post : Visbility test result cached in this face.
	 *
	 ------------------------------------------------------*/

	byte mFlags;                      /* Option flags. */

	byte mSmoothingGroup;             /* Smoothing group. */

	byte mVisible;                    /* Light visiblity test 2^(light_id) cached. */

	// FIXME: This is handled by the Mesh now using material face lists/arrays.
	byte mMaterial;                   /* Material id reference. */

	hel::Vec3 mNormal;                /* Assumes co-planar. */

	mstl::Vector<Vertex*> mVertices;  /* Vertices that comprise this face. */

	mstl::Vector<Edge*> mEdges;       /* Edges that comprise this face. */

	mstl::Vector<Face*> mNeighbours;  /* Faces that share an edge with this face. */
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
Face::Face() :
	mFlags(fNone),
	mSmoothingGroup(0),
	mVisible(0),
	mMaterial(0),
	mNormal(0.0f, 1.0f, 0.0f),
	mVertices(),
	mEdges(),
	mNeighbours()
{ }


inline
Face::~Face()
{ }


inline
void Face::AddVertex( freyja::Vertex* vertex )
{
	if ( !FindVertex( vertex ) == -1 ) 
	{
		mVertices.push_back( vertex );

		// FIXME: Now you should append this Face's id to the vertex's polyref
		// FIXME: Might want to cache edge map too
	}
}


inline
const int16 Face::FindVertex( freyja::Vertex* vertex ) const
{
	int16 idx = -1;
	for ( uint32 i = 0, n = mVertices.size(); i < n; ++i )
	{
		if ( mVertices[i] == vertex )
		{
			idx = i;
			break;
		}
	}	

	return idx;
}


inline
void Face::ReplaceVertex( freyja::Vertex* replace, freyja::Vertex* vertex )
{
	int16 idx = FindVertex( vertex );

	if ( idx > -1 )
	{
		mVertices[idx] = vertex;
	}
}

} // namespace freyja

#endif // GUARD__FREYJA_FACE_H_
