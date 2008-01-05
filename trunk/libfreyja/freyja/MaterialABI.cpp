/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the material ABI.
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

#include "Material.h"
#include "MaterialABI.h"

using namespace mstl;
using namespace freyja;

///////////////////////////////////////////////////////////////////////
// Material ABI 
//
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaMaterialCreate( const char* name )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}


freyja_ptr freyjaMaterialCreateFromFile( const char *filename )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

void freyjaMaterialDestroy( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

freyja_ptr freyjaMaterialFind( const char* name )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

uint32 freyjaMaterialGetCount( )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return 0;
}

const char* freyjaMaterialGetName( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

const char* freyjaMaterialGetFilename( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

int16 freyjaMaterialGetShaderId( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return -1;
}

freyja_ptr freyjaMaterialGetDecalMap( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetDiffuseMap( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetEmissiveMap( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetHeightMap( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetNormalMap( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetSpecularMap( freyja_ptr material )

{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}
const char* freyjaMaterialGetShaderFilename( freyja_ptr material )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

const char* freyjaMaterialGetBlendSource(freyja_ptr material)
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

const char* freyjaMaterialGetBlendDestination(freyja_ptr material)
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}


vec_t freyjaMaterialGetShininess(freyja_ptr material)
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return 0.0f;
}

vec_t freyjaMaterialGetTransparency(freyja_ptr material)
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return 0.0f;
}

void freyjaMaterialGetAmbientColor( freyja_ptr material, vec4_t ambient )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialGetDiffuseColor(freyja_ptr material, vec4_t diffuse)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialGetSpecularColor(freyja_ptr material, vec4_t specular)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialGetEmissiveColor(freyja_ptr material, vec4_t emissive)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetName(freyja_ptr material, const char *name)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetShaderFilename(freyja_ptr material, const char* filename)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetShaderId(freyja_ptr material, int16 shader)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetDecalMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetDiffuseMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetEmissiveMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetHeightMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetNormalMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetSpecularMap( freyja_ptr material, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetAmbientColor(freyja_ptr material, const vec4_t ambient)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetDiffuseColor(freyja_ptr material, const vec4_t diffuse)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetSpecularColor(freyja_ptr material, const vec4_t specular)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetEmissiveColor(freyja_ptr material, const vec4_t emissive)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetShininess(freyja_ptr material, vec_t exponent)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetTransparency(freyja_ptr material, vec_t transparency)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetBlendSource(freyja_ptr material, const char* factor)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetBlendDestination(freyja_ptr material, const char* factor)
{
	MARK_MSG("Not implemented in 10.x tracer.");
}
