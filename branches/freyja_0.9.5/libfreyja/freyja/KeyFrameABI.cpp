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

#include <mstl/Vector.h>

#include "KeyFrame.h"
#include "KeyFrameABI.h"

using namespace mstl;
using namespace freyja;

Vector<KeyFrame *> gKeyFramePool;


KeyFrame *freyjaGetKeyframeClass(index_t idx)
{
	KeyFrame **array = gKeyFramePool.getVectorArray();
	
	if ( idx < gKeyFramePool.size() )
	{
		return array[idx];
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////
// KeyFrame ABI
///////////////////////////////////////////////////////////////////////


uint32 freyjaGetKeyFrameCount()
{
	return gKeyFramePool.size();
}


byte freyjaIsKeyFrameAllocated(index_t keyframeIndex)
{
	return (freyjaGetKeyframeClass(keyframeIndex) != NULL);
}


void freyjaKeyFramePoolClear()
{
	gKeyFramePool.erase();
}


index_t freyjaKeyFrameGetIndexFromId(uint32 id)
{
	KeyFrame **array = gKeyFramePool.getVectorArray();

	for ( uint32 i = 0, count = gKeyFramePool.size(); i < count; ++i )
	{
		if (array[i] == NULL && array[i]->mIndex == id)
			return i;
	}
	
	return INDEX_INVALID;
}


index_t freyjaKeyFrameCreate(int32 id)
{
	index_t idx = freyjaKeyFrameGetIndexFromId(id);
	if (idx != INDEX_INVALID)
		return idx;

	KeyFrame **array = gKeyFramePool.getVectorArray();
	KeyFrame *obj = new KeyFrame();

	for ( uint32 i = 0, count = gKeyFramePool.size(); i < count; ++i )
	{
		if (array[i] == NULL)
		{
			obj->mIndex = i;
			array[i] = obj;
			return i;
		}
	}
	
	gKeyFramePool.pushBack(obj);
	return gKeyFramePool.size() - 1;
}


void freyjaKeyFrameDelete(index_t idx)
{
	KeyFrame **array = gKeyFramePool.getVectorArray();
	
	if ( idx < gKeyFramePool.size() && array[idx] )
	{
		delete array[idx];
		array[idx] = NULL;
	}
}


void freyjaKeyFrameTime(index_t keyframeIndex, vec_t time)
{
	KeyFrame *obj = freyjaGetKeyframeClass(keyframeIndex);
	if (obj) obj->mTime = time;
}


vec_t freyjaGetKeyFrameTime(index_t keyframeIndex)
{
	KeyFrame *obj = freyjaGetKeyframeClass(keyframeIndex);
	return (obj) ? obj->mTime : 0.0f;
}


void freyjaKeyFrameFlags(index_t keyframeIndex, byte flags)
{
	KeyFrame *obj = freyjaGetKeyframeClass(keyframeIndex);
	if (obj) obj->mFlags = flags;
}


byte freyjaGetKeyFrameFlags(index_t keyframeIndex)
{
	KeyFrame *obj = freyjaGetKeyframeClass(keyframeIndex);
	return (obj) ? obj->mFlags : 0x0;
}


void freyjaKeyFrameTransform(index_t keyframeIndex, 
							 freyja_transform_action_t action, 
							 vec_t x, vec_t y, vec_t z)
{
}


