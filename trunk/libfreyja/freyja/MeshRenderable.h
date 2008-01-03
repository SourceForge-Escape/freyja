/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : MeshRenderable
 * Comments: 
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.25:
 * Mongoose - Created, expanded from Mesh.
 ==========================================================================*/

#ifndef GUARD__FREYJA_MESHRENDERABLE_H__
#define GUARD__FREYJA_MESHRENDERABLE_H__

#include <hel/Ray.h>
#include "Weight.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Plane.h"

#include "SceneNode.h"
#include "FloatArray.h"

namespace freyja {

class Mesh;

class MeshRenderable : 
		public Renderable
{
public:	

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MeshRenderable( freyja::Mesh* mesh, freyja::Material* material ) :
		mOwner( mesh ),
		mMaterial( material ),
		mIndices( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~MeshRenderable( )
	{ }
	/*------------------------------------------------------
	 * Pre  : Mesh object is allocated
	 * Post : Deconstructs an object of Mesh
	 *
	 ------------------------------------------------------*/

	index_t ReserveIndexTriangle( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns offset in Index array.
	 *
	 ------------------------------------------------------*/

	//void SwapIndexTriangles( index_t a, index_t b );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//void TruncateIndexTriangles( uint32_t count );
	/*------------------------------------------------------
	 * Pre  : count is number of triangles to effectively 'remove'.
	 * Post : 
	 *
	 ------------------------------------------------------*/

	FREYJA_RENDERABLE_INTERFACE
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renderable implementation.
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected.
	////////////////////////////////////////////////////////////

	freyja::Mesh* mOwner;           /* Mesh that owns this renderable. */

	freyja::Material* mMaterial;    /* All faces in this list use this material. */

	mstl::Vector<index_t> mIndices; /* Triangle list. */

	hel::Vec3 mPosition;            /* Offset of this renderable in world coordinates. */

	hel::Quat mOrientation;         /* Orientation relative to world. */
};


inline
index_t MeshRenderable::ReserveIndexTriangle( )
{
	index_t off = mIndices.size();
	mIndices.push_back( 0 );
	mIndices.push_back( 0 );
	mIndices.push_back( 0 );
	return off;
}


inline
const hel::Quat& MeshRenderable::GetWorldOrientation() const
{
	return mOrientation;
}


inline
const hel::Vec3& MeshRenderable::GetWorldPosition() const
{
	return mPosition;
}


} // namespace freyja

#endif // GUARD__FREYJA_MESHRENDERABLE_H__
