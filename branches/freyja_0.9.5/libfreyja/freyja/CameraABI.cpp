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


mstl::Vector<freyja::Camera *> gCameras;


Camera *freyjaGetCameraObject(index_t idx)
{
	return ( idx < gCameras.size() ) ? gCameras[idx] : NULL;
}


uint32 freyjaGetCameraCount()
{
	return gCameras.size();
}


void freyjaCameraPoolClear()
{
	gCameras.erase();
}


index_t freyjaCameraCreate()
{
	Camera *cam = new Camera();  // this isn't 'filling in' deleted cameras
	gCameras.push_back(cam);
	return gCameras.size() - 1;
}


void freyjaCameraDelete(index_t cameraIndex)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);

	if (cam)
	{
		gCameras[cameraIndex] = NULL;
		delete cam;
	}
}


void freyjaCameraClearFlags(index_t cameraIndex, byte flags)
{
	FREYJA_INFOMSG(0, "Not implemented");
}


void freyjaCameraSetFlags(index_t cameraIndex, byte flags)
{
	FREYJA_INFOMSG(0, "Not implemented");
}


void freyjaCameraName(index_t cameraIndex, const char *name)
{	
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		cam->SetName(name);
	}
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


void freyjaCameraPos3fv(index_t cameraIndex, vec3_t xyz)
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


void freyjaCameraTarget3fv(index_t cameraIndex, vec3_t xyz)
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


const char *freyjaGetCameraNameString(index_t cameraIndex)
{
	Camera* cam = freyjaGetCameraObject(cameraIndex);
	if (cam) 
	{
		return cam->GetName();
	}

	return " "; 
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

	
void freyjaCameraMetadata(index_t camera, const char *metadata)
{
	Camera* cam = freyjaGetCameraObject(camera);
	if (cam) 
	{
		return cam->SetMetadata(metadata);
	}
}


const char *freyjaGetCameraMetadata(index_t camera)
{
	Camera* cam = freyjaGetCameraObject(camera);
	if (cam) 
	{
		return cam->GetMetadata();
	}

	return " "; 
}



// Animation

index_t freyjaCameraCreateTrack(index_t camera)
{
	return 0;
}

uint32 freyjaGetCameraTrackCount(index_t camera)
{
	return 0;
}

void freyjaCameraTrackDuration(index_t camera, index_t track, vec_t duration)
{
}

void freyjaCameraTrackRate(index_t camera, index_t track, vec_t fps)
{
}


index_t freyjaCameraCreateKeyframe(index_t camera, index_t track, vec_t time)
{
	return 0;
}

uint32 freyjaGetCameraKeyframeCount(index_t camera, index_t track)
{
	return 0;
}

vec_t freyjaGetCameraKeyframeTime(index_t camera, 
								  index_t track, index_t key)
{
	return 0.0f;
}

void freyjaCameraKeyframeMetadata(index_t camera, 
								  index_t track, index_t key, 
								  const char *metadata)
{
}

const char *freyjaGetCameraKeyframeMetadata(index_t camera, 
											index_t track, index_t key)
{
	return NULL;
}

void freyjaCameraKeyframePos3f(index_t camera, index_t track, index_t key,
							   vec_t x, vec_t y, vec_t z)
{
}

void freyjaGetCameraKeyframePos3fv(index_t camera, 
								   index_t track, index_t key,
								   vec3_t xyz)
{
}

void freyjaGetCameraKeyframeRotQuat4fv(index_t camera, 
									   index_t track, index_t key,
									   vec4_t wxyz)
{
}

void freyjaCameraKeyframeRotQuat4f(index_t camera, index_t track, index_t key,
								   vec_t w, vec_t x, vec_t y, vec_t z)
{
}
