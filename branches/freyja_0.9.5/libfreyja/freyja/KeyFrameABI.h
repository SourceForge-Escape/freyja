/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: KeyFrame ABI
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_KEYFRAMEABI_H_
#define GUARD__FREYJA_MONGOOSE_KEYFRAMEABI_H_

#include "freyja.h"

extern "C" {

	index_t freyjaKeyFrameGroupCreate(index_t start, index_t end);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaKeyFrameGroupDelete(index_t groupIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// KeyFrame
	///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetKeyFrameCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get number of allocated keyframes
	 ------------------------------------------------------*/

	byte freyjaIsKeyFrameAllocated(index_t keyframeIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Check if keyframe exists
	 ------------------------------------------------------*/

	void freyjaKeyFramePoolClear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destroy all keyframes
	 ------------------------------------------------------*/

	index_t freyjaKeyFrameGetIndexFromId(uint32 id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get keyframe index from UI keyframe id
	 ------------------------------------------------------*/

	index_t freyjaKeyFrameCreate(int32 id);
	/*------------------------------------------------------
	 * Pre  : <id> is the keyframe id in the UI
	 *
	 * Post : Returns the keyframe's gobal index_t, and
	 *        if keyframe doesn't exist it's created
	 ------------------------------------------------------*/

	void freyjaKeyFrameDelete(index_t keyframeIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete given keyframe
	 ------------------------------------------------------*/

	void freyjaKeyFrameTime(index_t keyframeIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set frame time
	 ------------------------------------------------------*/

	vec_t freyjaGetKeyFrameTime(index_t keyframeIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get frame time
	 ------------------------------------------------------*/

	void freyjaKeyFrameFlags(index_t keyframeIndex, byte flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set flags
	 ------------------------------------------------------*/

	byte freyjaGetKeyFrameFlags(index_t keyframeIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get flags
	 ------------------------------------------------------*/

	void freyjaKeyFrameTransform(index_t keyframeIndex, 
								 freyja_transform_action_t action, 
								 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
}

#endif
