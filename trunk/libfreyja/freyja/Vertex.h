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

#include <mstl/list.h>
#include "freyja.h"


namespace freyja {

class Vertex;
typedef mstl::list<freyja::Vertex*> VertexList;
typedef mstl::list<freyja::Vertex*>::iterator VertexIterator;
typedef mstl::list<index_t>::iterator VertexIndexIterator;

class Face;

class Vertex
{
public:

	Vertex( index_t idx ) : 
		mMainIndex( idx ),
		mIndices( ),
		mFaces( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~Vertex( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool AddFaceReference( freyja::Face* face );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face References list.
	 *
	 ------------------------------------------------------*/

	VertexIndexIterator GetIndexIterator( ) const
	{ return mIndices.begin(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	mstl::list<index_t>& GetIndices( ) 
	{ return mIndices; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Indices list.
	 *
	 ------------------------------------------------------*/

	mstl::list<Face*>& GetFaces( ) 
	{ return mFaces; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face References list.
	 *
	 ------------------------------------------------------*/
	
	const index_t& GetIndex( ) 
	{ return mMainIndex; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the offset in the FA VertexArray.
	 *
	 ------------------------------------------------------*/


protected:

	Vertex( const Vertex& v );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	index_t mMainIndex;

	mstl::list<index_t> mIndices;   /* Buffer indices this Vertex object controls. */

	mstl::list<Face*> mFaces;       /* Face references */
};


inline
bool Vertex::AddFaceReference( freyja::Face* face )
{
	bool found = false;
	for ( mstl::list<Face*>::iterator it = GetFaces().begin( ); it != it.end(); it++ )
	{
		if ( *it == face )
		{
			found = true;
			break;
		}
	}

	if ( !found )
	{
		mFaces.push_back( face );
	}

	return (!found);
}


} // End namespace freyja

#endif // GUARD__FREYJA_VERTEX_H_
