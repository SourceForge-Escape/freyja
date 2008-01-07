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

	void freyjaCameraDestroy( freyja_ptr camera );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaCameraGetTarget3f( freyja_ptr camera_ptr, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetCameraUp3fv( freyja_ptr camera_ptr, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaCameraSetTarget3f( freyja_ptr camera_ptr, vec_t x, vec_t y, vec_t z );
	void freyjaCameraSetTarget3fv( freyja_ptr camera_ptr, const vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera's target world position directly.
	 *
	 ------------------------------------------------------*/

	void freyjaCameraSetUp3f( freyja_ptr camera_ptr, vec_t x, vec_t y, vec_t z );
	void freyjaCameraSetUp3fv( freyja_ptr camera_ptr, const vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set camera's up vector directly.
	 *
	 ------------------------------------------------------*/

} // extern "C"


#endif // GUARD__FREYJA_CAMERAABI_H_
