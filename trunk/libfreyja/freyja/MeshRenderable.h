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

class MeshRenderable : 
		public SceneNode
{
public:	

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MeshRenderable( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	MeshRenderable( const Mesh& mesh );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy constructor.
	 *
	 ------------------------------------------------------*/

	~MeshRenderable( );
	/*------------------------------------------------------
	 * Pre  : Mesh object is allocated
	 * Post : Deconstructs an object of Mesh
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected.
	////////////////////////////////////////////////////////////

	freyja::Mesh* mOwner;           /* Mesh that owns this renderable. */

	freyja::Material* mMaterial;    /* All faces in this list use this material. */

	mstl::Vector<uint32> mIndices;  /* Triangle list. */
};

} // namespace freyja

#endif // GUARD__FREYJA_MESHRENDERABLE_H__
