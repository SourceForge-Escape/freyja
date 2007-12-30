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


Camera *freyjaGetCameraObject(index_t idx)
{
#warning FIXME
	return NULL; //( idx < gCameras.size() ) ? gCameras[idx] : NULL;
}


uint32 freyjaGetCameraCount()
{
#warning FIXME
	return 0; //gCameras.size();
}


void freyjaCameraPoolClear()
{
#warning FIXME
	//gCameras.erase();
}


freyja_ptr freyjaCameraCreate( const char* name )
{
	Camera* camera = new Camera( name );
	return camera->GetUID();
}


void freyjaCameraDelete( freyja_ptr camera )
{
#warning FIXME
}




void freyjaGetCameraPos3fv(index_t cameraIndex, vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		Vec3 v = cam->GetPos();
		helCopyVec3(v.mVec, xyz);
	}
}


void freyjaCameraPos3f(index_t cameraIndex, vec_t x, vec_t y, vec_t z)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetPos( Vec3(x, y, z) );
}


void freyjaCameraPos3fv(index_t cameraIndex, const vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetPos( Vec3(xyz) );
}


void freyjaGetCameraTarget3fv(index_t cameraIndex, vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		Vec3 v = cam->GetTarget();
		helCopyVec3(v.mVec, xyz);
	}
}


void freyjaCameraTarget3f(index_t cameraIndex, vec_t x, vec_t y, vec_t z)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetTarget( Vec3(x, y, z) );
}


void freyjaCameraTarget3fv(index_t cameraIndex, const vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetTarget( Vec3(xyz) );
}


void freyjaCameraRotQuat4f(index_t cameraIndex,
						   vec_t w, vec_t x, vec_t y, vec_t z)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetRot( Quat(w, x, y, z) );
}

void freyjaCameraRotQuat4fv(index_t cameraIndex, vec4_t wxyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) cam->SetRot( Quat(wxyz) );
}

void freyjaGetCameraRotQuat4fv(index_t cameraIndex, vec4_t wxyz)
{
}


void freyjaCameraUp3f(index_t cameraIndex, vec_t x, vec_t y , vec_t z)
{
	vec3_t xyz = { x, y, z };
	freyjaGetCameraUp3fv( cameraIndex, xyz );
}


void freyjaCameraUp3fv(index_t cameraIndex, const vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		cam->SetUp( xyz );
	}
}

void freyjaGetCameraUp3fv(index_t cameraIndex, vec3_t xyz)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		Vec3 v = cam->GetUp();
		v.Get(xyz);
	}
}

	
