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

#include "SceneNode.h"
#include "SceneNodeABI.h"


///////////////////////////////////////////////////////////////////////
// SceneNode 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

void
freyjaSceneNodeGetBoundingBox( freyja_ptr scenenode, 
							   vec3_t min, 
							   vec3_t max )
{
#warning FIXME
}


void
freyjaSceneNodeGetBoundingSphereCenter( freyja_ptr scenenode,
										vec3_t center )
{
#warning FIXME
}


vec_t
freyjaSceneNodeGetBoundingSphereRadius( freyja_ptr scenenode )
{
#warning FIXME
}


