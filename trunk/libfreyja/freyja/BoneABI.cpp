/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ABI to expose Bone class and utils to C.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Bone.cpp
 ==========================================================================*/

#include "freyja.h"
#include "Bone.h"
#include "Skeleton.h"
#include "BoneABI.h"

using namespace freyja;


///////////////////////////////////////////////////////////////////////
// freyjaBone 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaBoneCreate( freyja_ptr skeleton_ptr, const char* name )
{
	freyja::Skeleton* skeleton = Skeleton::Cast( skeleton_ptr );
	freyja::Bone* bone = ( skeleton ) ? skeleton->CreateBone( name ) : NULL;
	return bone;
}


void freyjaBoneDelete( freyja_ptr bone_ptr )
{
	freyja::Bone* bone = Bone::Cast( bone_ptr );

	if ( bone )
	{
		delete bone;
	}
}


#if ABI_0_9_5

	///////////////////////////////////////////////////////////////////////
	// BONE 0.9.3 - 0.9.5
	///////////////////////////////////////////////////////////////////////

	const vec_t *freyjaGetBoneBindPose16fv(freyja_ptr boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s bind pose transform
	 ------------------------------------------------------*/

	const vec_t *freyjaGetBoneLocalBindTransform16fv(freyja_ptr boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s bind pose local transform
	 ------------------------------------------------------*/

	void freyjaGetBoneWorldPos3fv(freyja_ptr boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s world position
	 ------------------------------------------------------*/

	void freyjaGetBoneTranslation3fv(freyja_ptr boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetBoneSkeletonIndex(freyja_ptr boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 'parent' skeletal index of this bone
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetBoneChild(freyja_ptr boneIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Iterator for bone children
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneChildCount(freyja_ptr boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Count of bone children
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////
	// 0.9.5 ABI Extensions
	///////////////////////////////////////////////////////////////

	uint32 freyjaGetBoneTrackCount(freyja_ptr bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of animation tracks for this bone.
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetBonePosKeyframeTime(freyja_ptr bone, freyja_ptr track, freyja_ptr key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaBoneKeyframeMetadata(freyja_ptr bone, freyja_ptr track, freyja_ptr key, 
									const char *metadata);
	const char *freyjaGetBoneKeyframeMetadata(freyja_ptr bone, freyja_ptr track, freyja_ptr key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetBonePosKeyframe3fv(freyja_ptr bone, freyja_ptr track, freyja_ptr key,
									 vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetBonePosKeyframeCount(freyja_ptr bone, freyja_ptr track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of keyframes for this track, for this bone.
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetBoneRotKeyframeTime(freyja_ptr bone, freyja_ptr track, freyja_ptr key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetBoneRotKeyframeEuler3fv(freyja_ptr bone, freyja_ptr track, freyja_ptr key,
									 vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneRotKeyframeCount(freyja_ptr bone, freyja_ptr track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of keyframes for this track, for this bone.
	 *
	 ------------------------------------------------------*/

	void freyjaBoneUpdateBindPose(freyja_ptr bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Force bind pose transform update
	 *
	 ------------------------------------------------------*/

	void freyjaBoneGetBindTransform(freyja_ptr bone, matrix_t m);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bind pose transform for <bone>
	 *
	 ------------------------------------------------------*/

	void freyjaBoneGetBindTransformInverse(freyja_ptr bone, matrix_t m);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bind pose transform for <bone>
	 *
	 ------------------------------------------------------*/

	void freyjaBoneBindTransformVertex(freyja_ptr bone, vec3_t p, vec_t w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transform vertex by bind pose for <bone>.
	 *
	 ------------------------------------------------------*/


	/*------------------------------------------------------
	 * NOTE 
	 * The new keyframing ABI should only be used now, so
	 * update all plugins for great victory!
	 *  
	 * The implementation will change, but the ABI should be stable.
	 ------------------------------------------------------ */

	freyja_ptr freyjaBoneTrackNew(freyja_ptr bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation track for mesh
	 *
	 ------------------------------------------------------*/
	
	void freyjaBoneTrackDuration(freyja_ptr bone, freyja_ptr track, vec_t duration);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	void freyjaBoneTrackRate(freyja_ptr bone, freyja_ptr track, vec_t fps);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaBoneKeyFrameNew(freyja_ptr bone, freyja_ptr track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	void freyjaBonePosKeyFrame3f(freyja_ptr bone, freyja_ptr track, freyja_ptr key,
								 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update bone position in keyframe
	 *
	 ------------------------------------------------------*/

	void freyjaBoneRotKeyFrameEuler3f(freyja_ptr bone, freyja_ptr track, freyja_ptr key,
									  vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : x,y,z in degrees
	 * Post : Update bone rotation in keyframe
	 *
	 ------------------------------------------------------*/

	void freyjaBoneRotKeyFrameQuat4f(freyja_ptr bone, freyja_ptr track, freyja_ptr key,
									 vec_t w, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : Quaterion is WXYZ order to match 0.9.3 ABIs! 
	 * Post : Update bone rotation in keyframe
	 *
	 ------------------------------------------------------*/




#endif
