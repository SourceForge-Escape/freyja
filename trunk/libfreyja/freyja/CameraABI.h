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

	freyja_ptr freyjaCameraCreate( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void freyjaCameraDelete( freyja_ptr camera );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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


}


#endif // GUARD__FREYJA_CAMERAABI_H_
