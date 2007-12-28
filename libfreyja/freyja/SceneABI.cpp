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

#include "freyja.h"
#include "Scene.h"
#include "SceneABI.h"

using namespace freyja;


///////////////////////////////////////////////////////////////////////
// Scene 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

void freyjaSceneAddNode( freyja_ptr scene, freyja_ptr node )
{
#warning FIXME
}


freyja_ptr freyjaSceneGetNode( freyja_ptr scene, freyja_id node )
{
#warning FIXME
	return NULL;
}


uint32 freyjaSceneGetSkeletonCount( freyja_ptr scene )
{
#warning FIXME
	return 0;
}


uint32 freyjaSceneGetMeshCount( freyja_ptr scene )
{
#warning FIXME
	return 0;
}


uint32 freyjaSceneGetPixelbufferCount( freyja_ptr scene )
{
#warning FIXME
	return 0;
}





