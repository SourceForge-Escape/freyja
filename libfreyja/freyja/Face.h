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
#include "MeshRenderable.h"


namespace freyja {

class Mesh;

class Face
{
public:

	Face( freyja::Mesh* owner, freyja::MeshRenderable* renderable );
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

	freyja::MeshRenderable* mRenderable; /* Index array, Material, etc storage. */

	hel::Vec3 mNormal;              /* Assumes co-planar vertices. */

	mstl::list<index_t> mTriangles; /* Indices of Index Triangles. */

	mstl::list<Vertex*> mVertices;  /* Vertices that comprise this face. */

	mstl::list<Edge*> mEdges;       /* Edges that comprise this face. */

	// Just generate this from mEdges.
	//mstl::list<Face*> mNeighbours;  /* Faces that share an edge with this face. */

	mstl::list<Plane> mPlanes;     /* Planes are now Face composite members. */
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
Face::Face( freyja::Mesh* owner, freyja::MeshRenderable* renderable ) :
	mOwner( owner ),
	mRenderable( renderable ),
	mNormal( 0.0f, 1.0f, 0.0f ),
	mTriangles( ),
	mVertices( ),
	mEdges( ),
	mPlanes( )
{
	mTriangles.push_back( renderable->ReserveIndexTriangle( ) );
}


inline
Face::~Face()
{ }


inline
void Face::AddVertex( freyja::Vertex* vertex )
{
	if ( !ContainsVertex( vertex ) ) 
	{
		mVertices.push_back( vertex );

		/* Append this Face to the Vertex face reference list. */
		vertex->AddFaceReference( this );

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
