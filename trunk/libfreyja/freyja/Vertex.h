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

class Vertex
{
public:

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

	VertexIndexIterator GetIndexIterator() const
	{ return mIndices.begin(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	mstl::list<index_t>& GetFaceRefs() 
	{ return mFaceRefs; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Access the Face references.
	 *
	 ------------------------------------------------------*/

	

protected:

	mstl::list<index_t> mIndices;   /* Buffer indices this Vertex object controls. */

	mstl::list<index_t> mFaceRefs;  /* Face references */
};

} // End namespace freyja

#endif // GUARD__FREYJA_VERTEX_H_
