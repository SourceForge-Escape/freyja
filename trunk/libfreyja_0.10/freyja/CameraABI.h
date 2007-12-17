/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : CameraABI 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_CAMERAABI_H_
#define GUARD__FREYJA_CAMERAABI_H_

#include "freyja.h"

extern "C" {

	///////////////////////////////////////////////////////////////////////
	// CAMERA
	///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetCameraCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of cameras in scene.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraPoolClear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Frees all allocated cameras.
	 *
	 ------------------------------------------------------*/

	index_t freyjaCameraCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void freyjaCameraDelete(index_t cameraIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaCameraClearFlags(index_t cameraIndex, byte flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clear camera flags
	 *
	 ------------------------------------------------------*/

	void freyjaCameraSetFlags(index_t cameraIndex, byte flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera flags
	 *
	 ------------------------------------------------------*/

	void freyjaCameraName(index_t cameraIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable camera name
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraPos3fv(index_t cameraIndex, vec3_t xyz);
	void freyjaCameraPos3f(index_t cameraIndex, vec_t x, vec_t y, vec_t z);
	void freyjaCameraPos3fv(index_t cameraIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera world position.
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraTarget3fv(index_t cameraIndex, vec3_t xyz);
	void freyjaCameraTarget3f(index_t cameraIndex, vec_t x, vec_t y, vec_t z);
	void freyjaCameraTarget3fv(index_t cameraIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera's target world position directly.
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraUp3fv(index_t cameraIndex, vec3_t xyz);
	void freyjaCameraUp3f(index_t cameraIndex, vec_t x, vec_t y, vec_t z);
	void freyjaCameraUp3fv(index_t cameraIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera's up vector directly.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraRotQuat4f(index_t cameraIndex,
							   vec_t w, vec_t x, vec_t y, vec_t z);
	void freyjaCameraRotQuat4fv(index_t cameraIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Remember freyja uses { W X Y Z } quaternions to match 0.9.3 ABI.
	 * Post : Set camera orientation.
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraRotQuat4fv(index_t cameraIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets camera[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	const char *freyjaGetCameraNameString(index_t cameraIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets camera[index]'s position
	 ------------------------------------------------------*/

	void freyjaCameraMetadata(index_t camera, const char *metadata);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set XML metadata for this camera.
	 *
	 ------------------------------------------------------*/

	const char *freyjaGetCameraMetadata(index_t camera);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get XML metadata for this camera.
	 *
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////
	// Tracks
	///////////////////////////////////////////////////////////////

	index_t freyjaCameraCreateTrack(index_t camera);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new camera animation track for mesh
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetCameraTrackCount(index_t camera);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of animation tracks for this camera.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraTrackDuration(index_t camera, index_t track, vec_t duration);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set track duration.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraTrackRate(index_t camera, index_t track, vec_t fps);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set track rate in frames per second.
	 *
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////
	// Keyframes 
	///////////////////////////////////////////////////////////////

	index_t freyjaCameraCreateKeyframe(index_t camera, index_t track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new camera animation keyframe for track
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetCameraKeyframeCount(index_t camera, index_t track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Number of keyframes in the track for this camera.
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetCameraKeyframeTime(index_t camera, 
									  index_t track, index_t key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Time offset of the keyframe for the track...
	 *
	 ------------------------------------------------------*/

	void freyjaCameraKeyframeMetadata(index_t camera, 
									  index_t track, index_t key, 
									  const char *metadata);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set XML metadata for this keyframe.
	 *
	 ------------------------------------------------------*/

	const char *freyjaGetCameraKeyframeMetadata(index_t camera, 
												index_t track, index_t key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get XML metadata for this keyframe.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraKeyframePos3f(index_t camera, index_t track, index_t key,
								   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera position for keyframe.
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraKeyframePos3fv(index_t camera, 
									   index_t track, index_t key,
									   vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get camera position for keyframe.
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraKeyframeRotQuat4fv(index_t camera, 
										   index_t track, index_t key,
										   vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Quaterion is WXYZ order to match 0.9.3 ABIs! 
	 * Post : Get camera orientation for keyframe.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraKeyframeRotQuat4f(index_t camera, index_t track, index_t key,
									   vec_t w, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : Quaterion is WXYZ order to match 0.9.3 ABIs! 
	 * Post : Set camera orientation for keyframe.
	 *
	 ------------------------------------------------------*/

}


#endif // GUARD__FREYJA_CAMERAABI_H_
