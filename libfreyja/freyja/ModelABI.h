/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the model ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_MODELABI_H_
#define GUARD__FREYJA_MODELABI_H_

#include "freyja.h"

extern "C" {

	index_t freyjaModelCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelAddSkeleton(index_t skeleton);
	/*------------------------------------------------------
	 * Pre  : Only assign one skeleton to model
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelAddMesh(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetModelCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns gobal model count
	 ------------------------------------------------------*/

	uint32 freyjaGetModelFlags(index_t model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets model option flags
	 ------------------------------------------------------*/

	uint32 freyjaGetModelMeshCount(index_t model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh count of the model
	 ------------------------------------------------------*/

	index_t freyjaGetModelMeshIndex(index_t model, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns gobal mesh index of the model
	 ------------------------------------------------------*/

	index_t freyjaGetModelSkeleton(index_t model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns gobal skeleton index of the model
	 ------------------------------------------------------*/

	void freyjaModelTransform(index_t model,
							  freyja_transform_action_t action, 
							  vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transforms all meshes and skeleton in model.
	 ------------------------------------------------------*/

	void freyjaModelClear(index_t model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All data in model is reset/cleared
	 ------------------------------------------------------*/

}

#endif // GUARD__FREYJA_MODELABI_H_
