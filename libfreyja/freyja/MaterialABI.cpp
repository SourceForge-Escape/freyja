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
#include "ResourceManager.h"
#include "MaterialABI.h"

using namespace mstl;
using namespace freyja;

typedef ResourceManager<Material> MaterialManager;


///////////////////////////////////////////////////////////////////////
// Material ABI 
//
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaMaterialCreate( const char* name )
{
	Material* material = new Material( name );

	if ( !MaterialManager::GetInstance( )->Register( name, material ) )
	{
		delete material;
		material = NULL;
	}

	FREYJA_ASSERTMSG( material != NULL, "" );

	return (material) ? material->ToPtr() : NULL;
}


freyja_ptr freyjaMaterialCreateFromFile( const char *filename )
{
	Material* material = Material::Import( filename );

	if ( !material ||
		 !MaterialManager::GetInstance( )->Register( material->GetName(), material ) )
	{
		delete material;
		material = NULL;
	}
	
	return (material) ? material->ToPtr() : NULL;
}


void freyjaMaterialDestroy( freyja_ptr material_ptr )
{
	Material* material = Material::Cast( material_ptr );

	if ( material )
	{
		MaterialManager::GetInstance( )->Unregister( material->GetName(), true );
	}
}

freyja_ptr freyjaMaterialFind( const char* name )
{
	Material* material = MaterialManager::GetInstance( )->Request( name );
	return (material) ? material->ToPtr() : NULL;
}

uint32 freyjaMaterialGetCount( )
{
	return MaterialManager::GetInstance()->GetCount( );
}

const char* freyjaMaterialGetName( freyja_ptr material_ptr )
{
	Material* material = Material::Cast( material_ptr );
	return (material) ? material->GetName() : NULL;
}

const char* freyjaMaterialGetFilename( freyja_ptr material_ptr )
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetFilename() : NULL;
}

int16 freyjaMaterialGetShaderId( freyja_ptr material_ptr )
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetShaderId() : -1;
}

freyja_ptr freyjaMaterialGetDecalMap( freyja_ptr material_ptr )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetDiffuseMap( freyja_ptr material_ptr )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetEmissiveMap( freyja_ptr material_ptr )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetHeightMap( freyja_ptr material_ptr )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetNormalMap( freyja_ptr material_ptr )
{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}

freyja_ptr freyjaMaterialGetSpecularMap( freyja_ptr material_ptr )

{
	MARK_MSG("Not implemented in 10.x tracer.");
	return NULL;
}
const char* freyjaMaterialGetShaderFilename( freyja_ptr material_ptr )
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetShaderFilename() : NULL;
}

const char* freyjaMaterialGetBlendSource(freyja_ptr material_ptr)
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetBlendSrc() : NULL;
}

const char* freyjaMaterialGetBlendDestination(freyja_ptr material_ptr)
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetBlendDest() : NULL;
}


vec_t freyjaMaterialGetShininess(freyja_ptr material_ptr)
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetShininess() : 0.0f;
}


vec_t freyjaMaterialGetTransparency(freyja_ptr material_ptr)
{
	Material* material = Material::Cast( material_ptr );
	return ( material ) ? material->GetTransparency() : 0.0f;
}


void freyjaMaterialGetAmbientColor( freyja_ptr material_ptr, vec4_t ambient )
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		for ( uint16 i = 0; i < 4; ++i )
		{
			ambient[i] = material->GetAmbientColor()[i];
		}
	}
}

void freyjaMaterialGetDiffuseColor(freyja_ptr material_ptr, vec4_t diffuse)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		for ( uint16 i = 0; i < 4; ++i )
		{
			diffuse[i] = material->GetDiffuseColor()[i];
		}
	}
}

void freyjaMaterialGetSpecularColor(freyja_ptr material_ptr, vec4_t specular)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		for ( uint16 i = 0; i < 4; ++i )
		{
			specular[i] = material->GetSpecularColor()[i];
		}
	}
}

void freyjaMaterialGetEmissiveColor(freyja_ptr material_ptr, vec4_t emissive)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		for ( uint16 i = 0; i < 4; ++i )
		{
			emissive[i] = material->GetEmissiveColor()[i];
		}
	}
}

void freyjaMaterialSetName(freyja_ptr material_ptr, const char *name)
{
	Material* material = MaterialManager::GetInstance( )->Request( name );

	if ( !material )
	{
		material = Material::Cast( material_ptr );
		if ( material )	material->SetName( name );
	}
}

void freyjaMaterialSetShaderFilename(freyja_ptr material_ptr, const char* filename)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetShaderFilename( filename );
	}
}

void freyjaMaterialSetShaderId(freyja_ptr material_ptr, int16 shader)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetShaderId( shader );
	}
}

void freyjaMaterialSetDecalMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetDiffuseMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetEmissiveMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetHeightMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetNormalMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetSpecularMap( freyja_ptr material_ptr, freyja_ptr pixelbuffer )
{
	MARK_MSG("Not implemented in 10.x tracer.");
}

void freyjaMaterialSetAmbientColor(freyja_ptr material_ptr, const vec4_t ambient)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetAmbientColor( ambient );
	}
}

void freyjaMaterialSetDiffuseColor(freyja_ptr material_ptr, const vec4_t diffuse)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetDiffuseColor( diffuse );
	}
}

void freyjaMaterialSetSpecularColor(freyja_ptr material_ptr, const vec4_t specular)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetSpecularColor( specular );
	}
}

void freyjaMaterialSetEmissiveColor(freyja_ptr material_ptr, const vec4_t emissive)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetEmissiveColor( emissive );
	}
}

void freyjaMaterialSetShininess(freyja_ptr material_ptr, vec_t exponent)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetShininess( exponent );
	}
}

void freyjaMaterialSetTransparency(freyja_ptr material_ptr, vec_t transparency)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetTransparency( transparency );
	}
}

void freyjaMaterialSetBlendSource(freyja_ptr material_ptr, const char* factor)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetBlendSrc( factor );
	}
}

void freyjaMaterialSetBlendDestination(freyja_ptr material_ptr, const char* factor)
{
	Material* material = Material::Cast( material_ptr );
	if ( material )
	{
		material->SetBlendDest( factor );
	}
}
