/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the SceneNode C ABI.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_SCENENODEABI_H_
#define GUARD__FREYJA_SCENENODEABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// SceneNode 0.10.0 ABI
	//
	// Implements: Node, XMLSerializer
	///////////////////////////////////////////////////////////////////////

	void freyjaSceneNodeGetBoundingBox( freyja_ptr scenenode, vec3_t min, vec3_t max );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets min and max of axis aligned bounding volume.
	 ------------------------------------------------------*/

	void freyjaSceneNodeGetBoundingSphere( freyja_ptr scenenode, vec3_t origin, vec_t* radius );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets origin and radius of spherical bounding volume.
	 ------------------------------------------------------*/


} // extern "C"

#endif // GUARD__FREYJA_SCENENODEABI_H_
