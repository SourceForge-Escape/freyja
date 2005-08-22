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

#ifndef BONE_0_9_3_API
	/* A nice trick to support deprecated function signatures, just recompile */
	#define freyjaGetBoneName1s freyjaGetBoneNameString
	#define freyjaBoneName1s freyjaBoneName
	#define freyjaBoneAddChild1i freyjaBoneAddChild
	#define freyjaBoneRemoveChild1i freyjaBoneRemoveChild
	#define freyjaGetBoneRotationEulerXYZ3fv freyjaGetBoneRotationEuler3fv
	#define freyjaBoneRotateEulerXYZ3f freyjaBoneRotateEuler3f
	#define freyjaGetBoneSkeletalBoneIndex

	// These aren't bone functions, but utilities
	#define freyjaBoneRemoveVertex
	#define freyjaBoneAddMesh1i 
	#define freyjaBoneRemoveMesh1i
#else
	#define freyjaGetBoneNameString freyjaGetBoneName1s
	#define freyjaBoneName freyjaBoneName1s
	#define freyjaBoneParent freyjaBoneParent1i 
	#define freyjaBoneFlags freyjaBoneFlags1i 
	#define freyjaBoneRotateEuler3fv freyjaBoneRotateEulerXYZ3fv
	#define freyjaBoneRotateQuat4fv freyjaBoneRotateQuatWXYZ4fv 
	#define freyjaBoneAddChild freyjaBoneAddChild1i
	#define freyjaGetBoneRotationEuler3fv freyjaGetBoneRotationEulerXYZ3fv
#endif

#ifndef BONE_0_9_3_API

	///////////////////////////////////////////////////////////////////////
	// BONE
	///////////////////////////////////////////////////////////////////////

	byte freyjaIsBoneAllocated(index_t boneIndex);
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
	 *        call to freyjaBoneAddChild1i is needed after
	 *        all bones in skeleton are allocated
	 ------------------------------------------------------*/

	void freyjaBoneName(index_t boneIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable bone name
	 ------------------------------------------------------*/

	void freyjaBoneRemoveMesh(index_t boneIndex, index_t meshIndex);
	void freyjaBoneAddMesh(index_t boneIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Mesh is added to Bone's child list
	 *
	 *        Either makes mesh tree connection or
	 *        simulates by vertex weights and pivots
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

	void freyjaBoneTransform(index_t boneIndex, 
	                         freyja_transform_action_t action, 
	                         vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


#else

	////////////////////////////////////////////////////////////////
	// Bone
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaBoneCreate(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new rest bone for skeleton
	 *        Returns valid Index or -1 on Error
	 ------------------------------------------------------*/

	void freyjaIsBoneDelete(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	char freyjaIsBoneAllocated(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	// Bone mutators /////////////////

	void freyjaBoneTransform(index_t boneIndex, 
	                         freyja_transform_action_t action, 
	                         vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBoneAddKeyFrame(index_t boneIndex, index_t frameIndex, 
								vec_t time, vec3_t translate, vec3_t rotate);
	/*------------------------------------------------------
	 * Pre  : frameIndex - used for virtual grouping 
	 *        time - time to next frame
	 *        translate, rotate - transform change from rest frame
	 *
	 * Post : Appends a simple keyframe transition to bone
	 *        This call isn't API 10.0 compatible
	 ------------------------------------------------------*/

	void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets a strong weight in vertex for this bone
	 ------------------------------------------------------*/

	void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes weights in vertex for this bone
	 ------------------------------------------------------*/

	void freyjaBoneFlags1i(index_t boneIndex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone flags
	 ------------------------------------------------------*/

	void freyjaBoneParent1i(index_t boneIndex, index_t parentIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone parent	
	 *
	 *        This doesn't affect skeleton, a follow up
	 *        call to freyjaBoneAddChild1i is needed after
	 *        all bones in skeleton are allocated
	 ------------------------------------------------------*/

	void freyjaBoneName1s(index_t boneIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable bone name
	 ------------------------------------------------------*/

	void freyjaBoneRemoveMesh1i(index_t boneIndex, index_t meshIndex);
	void freyjaBoneAddMesh1i(index_t boneIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Mesh is added to Bone's child list
	 *
	 *        Either makes mesh tree connection or
	 *        simulates by vertex weights and pivots, so
	 *        if you want to use skinning don't use these
	 ------------------------------------------------------*/

	void freyjaBoneRemoveChild1i(index_t boneIndex, index_t childIndex);
	void freyjaBoneAddChild1i(index_t boneIndex, index_t childIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Child is added to Bone's child list
	 *
	 ------------------------------------------------------*/

	void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z);
	void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone relative position to parent
	 ------------------------------------------------------*/

	void freyjaBoneRotateEulerXYZ3f(index_t boneIndex,
									vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateEulerXYZ3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaBoneRotateQuatWXYZ4f(index_t boneIndex,
									vec_t w, vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateQuatWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaBonePosition3fv(index_t boneIndex, vec3_t xyz); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone absolute position using IK solver 
	 ------------------------------------------------------*/


	// Bone accessors /////////////////

	const char *freyjaGetBoneName1s(index_t boneIndex);
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
	 *        Returns INDEX_INVALID if there is no parent
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationQuatWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationEulerXYZ3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 ------------------------------------------------------*/

	void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s relative position
	 ------------------------------------------------------*/

	index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns local skeletal index of this bone
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

	void freyjaGetBone16fv(index_t boneIndex, matrix_t mat); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bone's absolute position and rotation 
	 ------------------------------------------------------*/
#endif
}

#endif
