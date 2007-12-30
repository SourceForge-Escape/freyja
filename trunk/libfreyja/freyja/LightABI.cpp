/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the Light ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "Light.h"
#include "LightABI.h"
#include "freyja.h"

using namespace freyja;
using namespace mstl;

#warning FIXME LightABI not implemented for new API.

index_t freyjaLightCreate()
{
	MSTL_MSG("Called stub function...");
	return 0;	
}


void freyjaLightDelete(index_t lightIndex)
{
	MSTL_MSG("Called stub function...");
}


uint32 freyjaGetLightCount()
{
	MSTL_MSG("Called stub function...");
	return 0;
}


index_t freyjaGetCurrentLight()
{
	MSTL_MSG("Called stub function...");
	return 0;
}


void freyjaCurrentLight(uint32 lightIndex)
{
	MSTL_MSG("Called stub function...");
}


// Light mutators /////////////////

void freyjaLightPosition4v(uint32 lightIndex, vec4_t position)
{
	MSTL_MSG("Called stub function...");
}


void freyjaLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	MSTL_MSG("Called stub function...");
}


void freyjaLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	MSTL_MSG("Called stub function...");
}


void freyjaLightSpecular(uint32 lightIndex, vec4_t specular)
{
	MSTL_MSG("Called stub function...");
}


// Light accessors /////////////////

void freyjaGetLightPosition4v(uint32 lightIndex, vec4_t position)
{
	MSTL_MSG("Called stub function...");
}


void freyjaGetLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	MSTL_MSG("Called stub function...");
}


void freyjaGetLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	MSTL_MSG("Called stub function...");
}


void freyjaGetLightSpecular(uint32 lightIndex, vec4_t specular)
{
	MSTL_MSG("Called stub function...");
}
