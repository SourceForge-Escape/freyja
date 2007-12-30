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

#include <hel/Vec3.h>
#include <mstl/list.h>
#include "freyja.h"
#include "Vertex.h"
#include "Edge.h"
#include "Plane.h"

namespace freyja {

class Mesh;

class Face
{
public:

	Face( freyja::Mesh* owner, index_t offset );
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

	bool ContainsVertex( freyja::Vertex* vertex ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if Face contains vertex.
	 *
	 ------------------------------------------------------*/

	void ReplaceVertex( freyja::Vertex* replace, freyja::Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	freyja::Mesh* mOwner;           /* Mesh that owns this face. ( Mostly for ABI use. ) */

	index_t mListOffset;            /* Offset into the face index array */
	
	hel::Vec3 mNormal;              /* Assumes co-planar vertices. */

	mstl::list<Vertex*> mVertices;  /* Vertices that comprise this face. */

	mstl::list<Edge*> mEdges;       /* Edges that comprise this face. */

	mstl::list<Face*> mNeighbours;  /* Faces that share an edge with this face. */

	mstl::list<Plane*> mPlanes;     /* Planes are now Face members. */
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
Face::Face( Mesh* owner, index_t offset ) :
	mOwner( owner ),
	mListOffset( offset ),
	mNormal(0.0f, 1.0f, 0.0f),
	mVertices(),
	mEdges(),
	mNeighbours(),
	mPlanes()
{ }


inline
Face::~Face()
{
	// FIXME: Delete planes.
}


inline
void Face::AddVertex( freyja::Vertex* vertex )
{
	if ( !ContainsVertex( vertex ) ) 
	{
		mVertices.push_back( vertex );

		// FIXME: Now you should append this Face's id to the vertex's polyref
		// FIXME: Might want to cache edge map too
	}
}


inline
bool Face::ContainsVertex( freyja::Vertex* vertex ) const
{
	bool found = false;
	for ( VertexIterator it = mVertices.begin(); *it; it++ )
	{
		if ( (*it) == vertex )
		{
			found = true;
			break;
		}
	}	

	return found;
}


inline
void Face::ReplaceVertex( freyja::Vertex* replace, freyja::Vertex* vertex )
{
	for ( VertexIterator it = mVertices.begin(); *it; it++ )
	{
		if ( (*it) == vertex )
		{
			// FIXME: Could have duplicaties now... 
			it = vertex;
			break;
		}
	}
}

} // namespace freyja

#endif // GUARD__FREYJA_FACE_H_
