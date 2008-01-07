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

#include "Camera.h"

#include "CameraABI.h"

using freyja::Camera;
using hel::Vec3;
using hel::Quat;


freyja_ptr freyjaCameraCreate( const char* name )
{
	Camera* camera = new Camera( name );
	return camera->ToPtr();
}


void freyjaCameraDestroy( freyja_ptr camera_ptr )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		delete camera;
	}
}


void freyjaCameraGetTarget3f( freyja_ptr camera_ptr, vec3_t xyz )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->GetTarget().Get( xyz );
	}	
}


void freyjaGetCameraUp3fv( freyja_ptr camera_ptr, vec3_t xyz )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->GetUp().Get( xyz );
	}
}


void freyjaCameraSetTarget3f( freyja_ptr camera_ptr, vec_t x, vec_t y, vec_t z )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->SetTarget( Vec3(x, y, z) );
	}
}


void freyjaCameraSetTarget3fv( freyja_ptr camera_ptr, const vec3_t xyz )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->SetTarget( xyz );
	}
}


void freyjaCameraSetUp3f( freyja_ptr camera_ptr, vec_t x, vec_t y, vec_t z )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->SetUp( Vec3(x, y, z) );
	}
}


void freyjaCameraSetUp3fv( freyja_ptr camera_ptr, const vec3_t xyz )
{
	Camera* camera = Camera::Cast( camera_ptr );
	if ( camera )
	{
		camera->SetUp( xyz );
	}
}

	
