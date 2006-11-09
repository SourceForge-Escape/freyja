/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: KeyFrame class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/

//#include <mstl/Map.h>

#include "KeyFrame.h"

using namespace mstl;
using namespace freyja;

//Map<int32, index_t> gKeyFrameMap;
//Vector<KeyFrame *> gKeyFrames;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

//KeyFrame::KeyFrame() :
//	mFlags(0x0),
//	mIndex(INDEX_INVALID),
//	mTime(0.0f),
//	mObjects()
//{
//}


//KeyFrame::~KeyFrame()
//{
//}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_KEYFRAME
int runKeyFrameUnitTest(int argc, char *argv[])
{
	KeyFrame test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[KeyFrame class test]\n");

	return runKeyFrameUnitTest(argc, argv);
}
#endif


// NOTE: This assumes 'forward playback' and computes from last keyframe
void TransformTrack::GetTransform(vec_t time,
								  Vec3 &pos, Vec3 &rot, Vec3 &scale) 
{
	// Ran off the rails here... could wrap around, but just return for now
	if (time > GetDuration() || time < 0.0f)
	{
		pos   = Vec3(0,0,0);
		rot   = Vec3(0,0,0);
		scale = Vec3(1,1,1);
		return;
	}
	
	// Given time assertion this should be a 'valid' frame here...
	int32 frame = GetKeyfameIndexFromTime(time);
	Vec3x3KeyFrame *key = GetKeyframe(frame);
	
	if (key)
	{
		pos   = key->mData[2];
		rot   = key->mData[0];
		scale = key->mData[1];
		
		// NOTE: This should interpolate for PLAYBACK, but this is the
		//       editor interface for KEYFRAMES so we need 1.0f weight 
		//       to correctly edit existing keys.
		//
		// This basically means we'll 'pull' to this keyframe very easy,
		// which later will be tweakable from this API.
	}
	else
	{
		pos   = Vec3(0,0,0);
		rot   = Vec3(0,0,0);
		scale = Vec3(1,1,1);
		
		// Find last keyframe ( not last played, that's for a game engine )
		KeyFrame **array = mKeyFrames.getVectorArray();
		int32 last;
		vec_t start = 0.0f;
		
		key = NULL;
		for ( last = frame; last > -1; --last )
		{
			if (array[last])
			{
				key = (Vec3x3KeyFrame *)array[last];
				break;
			}
		}
		
		// Found a 'start' keyframe, or if you don't you'll use a
		// dummy keyframe with reflexive transforms @ 0.0 seconds
		if (key)
		{
			pos   = key->mData[2];
			rot   = key->mData[0];
			scale = key->mData[1];
			start = (vec_t)last / GetRate();
		}
		
		key = NULL;
		int32 next = 0, count = mKeyFrames.size();
		for ( next = frame; next < count; ++next )
		{
			if (array[next])
			{
				key = (Vec3x3KeyFrame *)array[next];
				break;
			}
		}
		
		// Found an 'end' keyframe, or if you don't you'll use a
		// dummy keyframe with reflexive transforms @ 0.0 seconds
		if (key)
		{
			vec_t end = (vec_t)next / GetRate();
			Vec3 posNext   = key->mData[2];
			Vec3 rotNext   = key->mData[0];
			Vec3 scaleNext = key->mData[1];
			
			vec_t w = (time - start) / (end - start);
			
			//freyjaPrintMessage(" w = %f = %f / (%f - %f)", 
			//				   w, time, end, start);
			
			// Just testing pos for now
			pos.mVec[0] = pos.mVec[0] + w * (posNext.mVec[0] - pos.mVec[0]);
			pos.mVec[1] = pos.mVec[1] + w * (posNext.mVec[1] - pos.mVec[1]);
			pos.mVec[2] = pos.mVec[2] + w * (posNext.mVec[2] - pos.mVec[2]);



			//freyjaPrintMessage("   => %f = %f + %f * (%f - %f)\n",
			//				   pos.mVec[1], pos.mVec[1], 
			//				   w, posNext.mVec[1], pos.mVec[1]);
		}
	}
}
