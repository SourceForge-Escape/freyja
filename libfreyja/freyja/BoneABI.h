/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the bone data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJABONEABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJABONEABI_H_

#include "freyja.h"

extern "C" {


	///////////////////////////////////////////////////////////////////////
	// BONE
	///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetBoneCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	byte freyjaIsBoneAllocated(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBonePoolClear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaBoneCreate(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new rest bone for skeleton
	 *        Returns valid Index or -1 on Error
	 ------------------------------------------------------*/

	void freyjaBoneDelete(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBoneFlags(index_t boneIndex, byte flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone flags
	 ------------------------------------------------------*/

	void freyjaBoneParent(index_t boneIndex, index_t parentIndex);
	/*------------------------------------------------------
	 * Pre  : Bone exists
	 * Post : Set bone parent
	 *
	 *        This doesn't affect skeleton, a follow up
	 *        call to AddChild is needed after
	 *        all bones in skeleton are allocated
	 ------------------------------------------------------*/

	void freyjaBoneName(index_t boneIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable bone name
	 ------------------------------------------------------*/

	void freyjaBoneRemoveChild(index_t boneIndex, index_t childIndex);
	void freyjaBoneAddChild(index_t boneIndex, index_t childIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Child is added to Bone's child list
	 ------------------------------------------------------*/

	void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z);
	void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone relative position
	 ------------------------------------------------------*/

	void freyjaBoneRotateEuler3f(index_t boneIndex, vec_t p, vec_t h, vec_t r);
	void freyjaBoneRotateEuler3fv(index_t boneIndex, vec3_t phr);
	void freyjaBoneRotateQuat4f(index_t boneIndex,
	                            vec_t w, vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateQuat4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationQuat4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationEuler3fv(index_t boneIndex, vec3_t phr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 ------------------------------------------------------*/

	void freyjaBoneTransform(index_t boneIndex, 
	                         freyja_transform_action_t action, 
	                         vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBoneTransform3fv(index_t bone, 
								freyja_transform_action_t action, vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBoneInverseTransform3fv(index_t bone, 
									   freyja_transform_action_t action, 
									   vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *freyjaGetBoneNameString(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetBoneName(index_t boneIndex, uint32 size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneParent(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 ------------------------------------------------------*/

	const vec_t *freyjaGetBoneBindPose16fv(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s bind pose transform
	 ------------------------------------------------------*/

	void freyjaGetBoneWorldPos3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s world position
	 ------------------------------------------------------*/

	void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 ------------------------------------------------------*/

	index_t freyjaGetBoneSkeletonIndex(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 'parent' skeletal index of this bone
	 ------------------------------------------------------*/

	index_t freyjaGetBoneChild(index_t boneIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Iterator for bone children
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneChildCount(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Count of bone children
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////
	// 0.9.5 ABI Extensions
	///////////////////////////////////////////////////////////////

	uint32 freyjaGetBoneTrackCount(index_t bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of animation tracks for this bone.
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetBonePosKeyframeTime(index_t bone, index_t track, index_t key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetBonePosKeyframe3fv(index_t bone, index_t track, index_t key,
									 vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetBonePosKeyframeCount(index_t bone, index_t track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of keyframes for this track, for this bone.
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetBoneRotKeyframeTime(index_t bone, index_t track, index_t key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetBoneRotKeyframeEuler3fv(index_t bone, index_t track, index_t key,
									 vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneRotKeyframeCount(index_t bone, index_t track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of keyframes for this track, for this bone.
	 *
	 ------------------------------------------------------*/

	void freyjaBoneUpdateBindPose(index_t bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Force bind pose transform update
	 *
	 ------------------------------------------------------*/

	void freyjaBoneGetBindTransform(index_t bone, matrix_t m);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bind pose transform for <bone>
	 *
	 ------------------------------------------------------*/

	void freyjaBoneGetBindTransformInverse(index_t bone, matrix_t m);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bind pose transform for <bone>
	 *
	 ------------------------------------------------------*/

	void freyjaBoneBindTransformVertex(index_t bone, vec3_t p, vec_t w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transform vertex by bind pose for <bone>
	 *
	 ------------------------------------------------------*/

	/*------------------------------------------------------
	 * NOTE 
	 * The new keyframing ABI should only be used now, so
	 * update all plugins for great victory!
	 *  
	 * The implementation will change, but the ABI should be stable.
	 ------------------------------------------------------ */

	index_t freyjaBoneTrackNew(index_t bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation track for mesh
	 *
	 ------------------------------------------------------*/
	
	void freyjaBoneTrackDuration(index_t bone, index_t track, vec_t duration);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	void freyjaBoneTrackRate(index_t bone, index_t track, vec_t fps);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	index_t freyjaBoneKeyFrameNew(index_t bone, index_t track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new bone animation keyframe for track
	 *
	 ------------------------------------------------------*/

	void freyjaBonePosKeyFrame3f(index_t bone, index_t track, index_t key,
								 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update bone position in keyframe
	 *
	 ------------------------------------------------------*/

	void freyjaBoneRotKeyFrameEuler3f(index_t bone, index_t track, index_t key,
									  vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : x,y,z in degrees
	 * Post : Update bone rotation in keyframe
	 *
	 ------------------------------------------------------*/

	void freyjaBoneRotKeyFrameQuat4f(index_t bone, index_t track, index_t key,
									 vec_t w, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : Quaterion is WXYZ order to match 0.9.3 ABIs! 
	 * Post : Update bone rotation in keyframe
	 *
	 ------------------------------------------------------*/

}

#   if defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )
#      include <mstl/SystemIO.h>
#      include <freyja/Bone.h>

    freyja::Bone *freyjaGetBoneClass(index_t boneUID); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Bone with UID or NULL
	 ------------------------------------------------------*/

    int32 freyjaBoneLoadChunkJA(mstl::SystemIO::FileReader &r,
								freyja_file_chunk_t &chunk);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format bone chunk
	 ------------------------------------------------------*/

    int32 freyjaBoneSaveChunkJA(mstl::SystemIO::FileWriter &w, index_t bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format bone chunk
	 ------------------------------------------------------*/

    bool freyjaBoneSaveChunkTextJA(SystemIO::TextFileWriter &w, index_t bone);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format bone chunk
	 ------------------------------------------------------*/

    bool freyjaBoneLoadChunkTextJA(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format bone chunk
	 ------------------------------------------------------*/

#   endif
#endif
