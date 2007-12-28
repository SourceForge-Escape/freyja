/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the Scene C ABI.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created, moved from other headers.
 ==========================================================================*/

#ifndef GUARD__FREYJA_SCENEABI_H_
#define GUARD__FREYJA_SCENEABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// freyjaScene 0.10 ABI
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaSceneAddNode( freyja_ptr scene, freyja_ptr node ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends node to scene.
	 ------------------------------------------------------*/

	freyja_ptr freyjaSceneGetNode( freyja_ptr scene, freyja_id node ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Node or NULL.
	 ------------------------------------------------------*/

	uint32 freyjaSceneGetSkeletonCount( freyja_ptr scene );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of skeletons in the scene.
	 ------------------------------------------------------*/

	uint32 freyjaSceneGetMeshCount( freyja_ptr scene );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns number of meshes in scene.
	 ------------------------------------------------------*/

	uint32 freyjaSceneGetPixelbufferCount( freyja_ptr scene );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of pixelbuffers in scene.
	 ------------------------------------------------------*/

} // extern "C"

#endif // GUARD__FREYJA_SCENEABI_H_
