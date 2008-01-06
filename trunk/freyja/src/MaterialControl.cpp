/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : MaterialControl
 * License : No use w/o permission, (C) 2000-2007 Mongoose
 * Comments: This class is an event control for backend materials, and
 *           the related user interface signals.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.10:
 * Mongoose - Created, reformed from various events and such.
 ==========================================================================*/

#include <mstl/SystemIO.h>
#include <freyja/PixelBuffer.h>
#include <freyja/MaterialABI.h>
#include <freyja/PerlinNoise.h>
#include "Texture.h"
#include "FreyjaOpenGL.h"

#include "MaterialControl.h"

using namespace freyja3d;
using namespace freyja;
	

MaterialControl *MaterialControl::mInstance = NULL;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MaterialControl::MaterialControl() : 
	EvSelectId(0), EvShineId(0), EvSetNameId(0), 
	EvSetTextureNameId(0), mFlags(fNone),
	mRecent("freyja-dev-recent-material", "eRecentMaterial")
{
	// We move to class based index_t later instead integer ids
	for (uint32 i = 0; i < 4; ++i)
	{
		EvAmbientId[i] = EvDiffuseId[i] = EvEmissiveId[i] = EvSpecularId[i] = 0;
	}

	mInstance = this;
}




////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

// Replace 'freyja' Material format with 'libfreyja' Material format + metadata
bool MaterialControl::LoadMaterial(const char *filename)
{
#warning FIXME This is now obsolete.
	return false;

#if 0
	if (!filename || !filename[0])
	{
		return false;
	}


	mstl::SystemIO::TextFileReader r;

	if (!r.Open(filename))
	{
		perror("MaterialControl::LoadMaterial() : ");
		return false;
	}

	index_t matIndex = INDEX_INVALID;
	uint32 mode = 0;
	bool perlinLoaded = false;
	vec_t iA = 1.0f, iB = 2.0f, d = 20.0f;
	int32 width = 256, height = 256, seed = 257;
	bool clamp = false;
	float addcolor[3] = {130.0/255.0, 130.0/255.0, 74.0/255.0};
	float modcolor[3] = {156.0/255.0, 130.0/255.0, 89.0/255.0};

	while (!r.IsEndOfFile())
	{
		const char *buffer = r.ParseSymbol();

		if (strncmp(buffer, "[Material]", 11) == 0)
		{
			if (mFlags & fLoadMaterialInSlot)
			{
				matIndex = freyjaGetCurrentMaterial();
			}
			else
			{
				matIndex = freyjaMaterialCreate();
			}

			mode = 1;
		}
		else if (strncmp(buffer, "[PerlinNoise]", 14) == 0)
		{
			mode = 2;
		}
		else if (mode == 1)
		{
			// Lex('=')
			r.FindNextChar('=');

			if (strncmp(buffer, "Shininess", 9) == 0)
			{				
				freyjaMaterialShininess(matIndex, r.ParseFloat());
			}
			else if  (strncmp(buffer, "Texture", 7) == 0)
			{
				if (LoadTexture(r.ParseStringLiteral()))
				{
					if (mTextureId > -1)
					{
						freyjaMaterialTexture(matIndex, mTextureId-1);
						freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
					}
				}
			}
			else if  (strncmp(buffer, "TextureName", 11) == 0)
			{
				const char *s = r.ParseStringLiteral();

				if (LoadTexture(s))
				{
					if (mTextureId > -1)
					{
						freyjaMaterialTextureName(matIndex, s);
						freyjaMaterialTexture(matIndex, mTextureId-1);
						freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
					}
				}
			}
			else if  (strncmp(buffer, "Name", 4) == 0)
			{
				freyjaMaterialName(matIndex, r.ParseStringLiteral());
			}
			else if  (strncmp(buffer, "EnableBlending", 14) == 0)
			{
				if (r.ParseBool())
					freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Blending);
			}
			else if (strncmp(buffer, "BlendSource", 11) == 0)
			{
				int32 val = OpenGL::BlendStringToInt(r.ParseSymbol());
				freyjaMaterialBlendSource(matIndex, val);
			}
			else if (strncmp(buffer, "BlendDest", 9) == 0)
			{						
				int32 val = OpenGL::BlendStringToInt(r.ParseSymbol());
				freyjaMaterialBlendDestination(matIndex, val);
			}
			else if (strncmp(buffer, "Ambient", 7) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialAmbient(matIndex, c);
			}
			else if (strncmp(buffer, "Diffuse", 7) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialDiffuse(matIndex, c);
			}
			else if (strncmp(buffer, "Specular", 8) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialSpecular(matIndex, c);
			}
			else if (strncmp(buffer, "Emissive", 8) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialEmissive(matIndex, c);				
			}
		}
		else if (mode == 2)
		{
			// Lex('=')
			r.FindNextChar('=');

			perlinLoaded = true;

			if (strncmp(buffer, "Seed", 4) == 0)
			{
				seed = r.ParseInteger();
				SetResourceInt("ePerlinNoiseSeed", seed);
			}
			else if (strncmp(buffer, "Width", 5) == 0)
			{
				width = r.ParseInteger();
				SetResourceInt("ePerlinNoiseW", width);
			}
			else if (strncmp(buffer, "Height", 6) == 0)
			{
				height = r.ParseInteger();
				SetResourceInt("ePerlinNoiseH", height);
			}
			else if (strncmp(buffer, "Clamp", 5) == 0)
			{
				clamp = r.ParseBool();
				SetResourceInt("ePerlinNoiseClamp", clamp);
			}
			else if (strncmp(buffer, "AddColor", 8) == 0)
			{
				addcolor[0] = r.ParseFloat();
				r.FindNextChar(',');
				addcolor[1] = r.ParseFloat();
				r.FindNextChar(',');
				addcolor[2] = r.ParseFloat();
				SetResourceColor("eColorPerlinAdd", 
								 addcolor[0], addcolor[1], addcolor[2], 1.0);
			}
			else if (strncmp(buffer, "ModulateColor", 14) == 0)
			{
				modcolor[0] = r.ParseFloat();
				r.FindNextChar(',');
				modcolor[1] = r.ParseFloat();
				r.FindNextChar(',');
				modcolor[2] = r.ParseFloat();
				SetResourceColor("eColorPerlinMult", 
								 modcolor[0], modcolor[1], modcolor[2], 1.0);
			}
			else if (strncmp(buffer, "iA", 2) == 0)
			{
				iA = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseIA", iA);
			}
			else if (strncmp(buffer, "iB", 2) == 0)
			{
				iB = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseIB", iB);
			}
			else if (strncmp(buffer, "d", 1) == 0)
			{
				d = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseD", d);
			}
		}
	}
	
	r.Close();

	// Convert this to Material metadata
	if ( perlinLoaded )
	{
		PerlinNoise perlin;
		byte* bitmap = perlin.generateBuffer( width, height, seed );
		byte* pixmap = new byte[ width * height * 3 ];
		
		if ( bitmap && pixmap )
		{	
			if (clamp)
			{
				perlin.clampBufferIntensity(bitmap, width, height, iA, iB, d);
			}

			for (uint32 i = 0, n = width * height; i < n; ++i)
			{
				/* NOTE: No clamping or scaling of colors, however there is a 
				   weakened 50 / 50 add in the sense that ADD can only contrib
				   _up_to_ 50% of full intensity ( 255 ).

				   The reason for this is to allow bleeding for plasma, etc.
				*/

			
				/* Modulate by a color and add a base half intensity */ 
				const unsigned int idx = i * 3;
				pixmap[idx  ] = (byte)(bitmap[i] * modcolor[0]) + (byte)(128 * addcolor[0]);
				pixmap[idx+1] = (byte)(bitmap[i] * modcolor[1]) + (byte)(128 * addcolor[1]);
				pixmap[idx+2] = (byte)(bitmap[i] * modcolor[2]) + (byte)(128 * addcolor[2]);
			}

			LoadTextureBuffer( pixmap, width, height, 24, 3 );
			freyjaMaterialTexture(matIndex, mTextureId-1);
			freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);

			delete [] bitmap; // FIXME: Should have library deallocator handle this.
		}
	}


	/* Set selected material to this, and refresh user interface. */
	//if ( matIndex != INDEX_INVALID )
	//	SetSelected( matIndex );
	RefreshInterface();
	mRecent.AddFilename( filename );

	return true;
#endif
}


// Replace 'freyja' Material format with 'libfreyja' Material format + metadata
bool MaterialControl::SaveMaterial(const char *filename)
{
#warning FIXME This is now obsolete.
	return false;
#if 0
	if (!filename || !filename[0])
	{
		return false;
	}

	mstl::SystemIO::TextFileWriter w;

	if (!w.Open(filename))
	{
		perror("Material::saveFile> ");
		return false;
	}

	int32 matIndex = freyjaGetCurrentMaterial();
	w.Print("[Material]\n");

	/* Name */
	char *name = (char*)freyjaGetMaterialName(matIndex);
	w.Print("Name = \"%s\"\n", name);

	/* Texture */
	uint32 texture = freyjaGetMaterialTexture(matIndex);
	if (texture && freyjaGetMaterialTextureName(matIndex))
	{
		w.Print("TextureName = \"%s\"\n", 
				freyjaGetMaterialTextureName(matIndex));
	}

	/* Blend factors */
	w.Print("EnableBlending = %s\n", 
			(freyjaGetMaterialFlags(matIndex) & fFreyjaMaterial_Blending) ? 
			"true" : "false");

	uint32 blend_src = freyjaGetMaterialBlendSource(matIndex);
	w.Print("BlendSource = %s\n", OpenGL::BlendIntToString(blend_src));

	uint32 blend_dest = freyjaGetMaterialBlendDestination(matIndex);
	w.Print("BlendDest = %s\n",  OpenGL::BlendIntToString(blend_dest));

	/* Ambient color */
	vec4_t ambient;
	freyjaGetMaterialAmbient(matIndex, ambient);
	w.Print("Ambient = %f, %f, %f, %f\n", 
			ambient[0], ambient[1], ambient[2], ambient[3]);

	/* Diffuse color */
	vec4_t diffuse;
	freyjaGetMaterialDiffuse(matIndex, diffuse);
	w.Print("Diffuse = %f, %f, %f, %f\n", 
			diffuse[0], diffuse[1], diffuse[2], diffuse[3]);

	/* Specular color */
	vec4_t specular;
	freyjaGetMaterialSpecular(matIndex, specular);
	w.Print("Specular = %f, %f, %f, %f\n", 
			specular[0], specular[1], specular[2], specular[3]);

	/* Emissive color */
	vec4_t emissive; 
	freyjaGetMaterialEmissive(matIndex, emissive);
	w.Print("Emissive = %f, %f, %f, %f\n", 
			emissive[0], emissive[1], emissive[2], emissive[3]);

	/* Specular exponent */
	vec_t shininess = freyjaGetMaterialShininess(matIndex);
	w.Print("Shininess = %f\n", shininess);

	// No 'named' texture dump perlin noise
	if (texture && !freyjaGetMaterialTextureName(matIndex))
	{
		int seed = GetResourceInt("ePerlinNoiseSeed");
		int ww = GetResourceInt("ePerlinNoiseW");
		int h = GetResourceInt("ePerlinNoiseH");
		int clamp = GetResourceInt("ePerlinNoiseClamp");
		float ia = GetResourceFloat("ePerlinNoiseIA");
		float ib = GetResourceFloat("ePerlinNoiseIB");
		float d = GetResourceFloat("ePerlinNoiseD");
		float mr, mb, mg, ma;
		GetResourceColor("eColorPerlinMult", mr, mb, mg, ma);
		float ar, ab, ag, aa;
		GetResourceColor("eColorPerlinAdd", ar, ab, ag, aa);

		w.Print("\n\n[PerlinNoise]\nSeed = %i\nWidth = %i\nHeight = %i\nClamp = %s\niA = %f\niB = %f\nd = %f\nModulateColor = %f, %f, %f\nAddColor = %f, %f, %f", seed, ww, h, clamp ? "true" : "false", ia, ib, d, mr, mb, mg, ar, ab, ag);
	}


	w.Close();

	return true;
#endif
}


void MaterialControl::RefreshInterface()
{
	freyja::Material* mat = GetCurrentMaterial( );

	if ( !mat )
	{
		//mgtk_textentry_value_set( EvSetNameId, "Invalid material." );
		return;
	}

	{
		const char* s = mat->GetName();
		mgtk_textentry_value_set( EvSetNameId, (s != NULL) ? s : " " );
	}
	
	mgtk_spinbutton_value_set(EvSetShaderId, mat->GetShaderId() );

	{
		const char* s = mat->GetShaderFilename();
		mgtk_textentry_value_set(EvSetShaderFilenameId, (s != NULL) ? s : " " );
	}
	
#warning FIXME UI does not support new multitexture yet.
	mgtk_spinbutton_value_set(EvSetTextureId, mat->GetDecalMapId() );

	{
		//const char* s = mat->GetDecalMap()->GetFilename();
		//mgtk_textentry_value_set(EvSetTextureNameId, (s != NULL) ? s : " " );
	}
	
	/* Color buttons. */
	{
		vec4_t color;
		freyjaMaterialGetAmbientColor( mat->ToPtr(), color );
		mgtk_event_set_color(eColorMaterialAmbient, color[0], color[1], color[2], color[3]);
		freyjaMaterialGetDiffuseColor( mat->ToPtr(), color );
		mgtk_event_set_color(eColorMaterialDiffuse, color[0], color[1], color[2], color[3]);
		freyjaMaterialGetSpecularColor( mat->ToPtr(), color );
		mgtk_event_set_color(eColorMaterialSpecular, color[0], color[1], color[2], color[3]);
		freyjaMaterialGetEmissiveColor( mat->ToPtr(), color );
		mgtk_event_set_color(eColorMaterialEmissive, color[0], color[1], color[2], color[3]);
	}

	/* Color component spinbuttons. */
	for ( uint32 i = 0; i < 4; ++i )
	{
		mgtk_spinbutton_value_set(EvAmbientId[i], mat->GetAmbientColor()[i]);	

		mgtk_spinbutton_value_set(EvDiffuseId[i], mat->GetDiffuseColor()[i]);

		mgtk_spinbutton_value_set(EvSpecularId[i], mat->GetSpecularColor()[i]);

		mgtk_spinbutton_value_set(EvEmissiveId[i], mat->GetEmissiveColor()[i]);
	}

	mgtk_spinbutton_value_set(EvShineId, mat->GetShininess() );

	/* Blend factor. */
	mgtk_option_menu_value_set( eBlendSrcMenu, GetBlendId( mat->GetBlendSrc() ) );
	mgtk_option_menu_value_set( eBlendDestMenu, GetBlendId( mat->GetBlendDest() ) );

	// No flags in new material.
	//uint32 flags = freyjaGetMaterialFlags(mIndex);
	//mgtk_togglebutton_value_set(EvEnableBlendingId, (flags & fFreyjaMaterial_Blending));
	//mgtk_togglebutton_value_set(EvEnableTextureId, (flags & fFreyjaMaterial_Texture));

	RefreshContext( );
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void MaterialControl::AttachMethodListeners()
{
	CreateListener("eSetMaterialShaderFilename", &MaterialControl::EvNop);

	CreateListener1u("eRecentMaterial", &MaterialControl::EvRecentFile);

	EvSelectId = CreateListener1u("eMaterialIterator", &MaterialControl::EvSelect);
	EvAmbientId[0] = CreateListener1f("eMaterialAmbient0", &MaterialControl::EvAmbient0);
	EvAmbientId[1] = CreateListener1f("eMaterialAmbient1", &MaterialControl::EvAmbient1);
	EvAmbientId[2] = CreateListener1f("eMaterialAmbient2", &MaterialControl::EvAmbient2);
	EvAmbientId[3] = CreateListener1f("eMaterialAmbient3", &MaterialControl::EvAmbient3);
	EvDiffuseId[0] = CreateListener1f("eMaterialDiffuse0", &MaterialControl::EvDiffuse0);
	EvDiffuseId[1] = CreateListener1f("eMaterialDiffuse1", &MaterialControl::EvDiffuse1);
	EvDiffuseId[2] = CreateListener1f("eMaterialDiffuse2", &MaterialControl::EvDiffuse2);
	EvDiffuseId[3] = CreateListener1f("eMaterialDiffuse3", &MaterialControl::EvDiffuse3);
	EvSpecularId[0] = CreateListener1f("eMaterialSpecular0", &MaterialControl::EvSpecular0);
	EvSpecularId[1] = CreateListener1f("eMaterialSpecular1", &MaterialControl::EvSpecular1);
	EvSpecularId[2] = CreateListener1f("eMaterialSpecular2", &MaterialControl::EvSpecular2);
	EvSpecularId[3] = CreateListener1f("eMaterialSpecular3", &MaterialControl::EvSpecular3);
	EvEmissiveId[0] = CreateListener1f("eMaterialEmissive0", &MaterialControl::EvEmissive0);
	EvEmissiveId[1] = CreateListener1f("eMaterialEmissive1", &MaterialControl::EvEmissive1);
	EvEmissiveId[2] = CreateListener1f("eMaterialEmissive2", &MaterialControl::EvEmissive2);
	EvEmissiveId[3] = CreateListener1f("eMaterialEmissive3", &MaterialControl::EvEmissive3);
	EvShineId = CreateListener1f("eMaterialShine", &MaterialControl::EvShine);
	EvSetShaderId = CreateListener1u("eSetMaterialShader", &MaterialControl::EvSetShader);

	EvSetTextureId = 
	CreateListener1u("eSetMaterialTexture", &MaterialControl::EvSetTexture);

	CreateListener("eNewMaterial", &MaterialControl::EvNewMaterial);
	CreateListener("eOpenMaterial", &MaterialControl::EvOpenMaterial);
	CreateListener("eSaveMaterial", &MaterialControl::EvSaveMaterial);

	EvEnableBlendingId = 
	CreateListener1u("eEnableMaterialBlending", &MaterialControl::EvEnableBlending);

	EvEnableTextureId = 
	CreateListener1u("eEnableMaterialTexture", &MaterialControl::EvEnableTexture);

	CreateListener1u("eEnableDetailTexture", &MaterialControl::EvEnableDetailTexture);

	CreateListener1u("eTextureSlotLoad", &MaterialControl::EvTextureSlotLoad);
	CreateListener1u("eMaterialSlotLoad", &MaterialControl::EvMaterialSlotLoad);

	CreateListener1u("eEnableNormalize", &MaterialControl::EvEnableNormalize);

	EvGLSLFragmentModeId =
	CreateListener1u("eGLSLFragmentMode", &MaterialControl::EvGLSLFragmentMode);

	EvARBFragmentModeId = 
	CreateListener1u("eARBFragmentMode", &MaterialControl::EvARBFragmentMode);

	CreateListener1u("eBlendSrc", &MaterialControl::EvBlendSrc);
	CreateListener1u("eBlendDest", &MaterialControl::EvBlendDest);

	CreateListener1u("eTextureUpload", &MaterialControl::EvTextureUpload);

	/* Strings. */
	CreateListener1s( "eOpenShader", &MaterialControl::EvOpenShader );
	CreateListener1s( "eOpenTexture", &MaterialControl::EvOpenTexture );
	EvSetNameId = CreateListener1s( "eSetMaterialName", &MaterialControl::EvSetName );
	EvSetTextureNameId = CreateListener1s( "eSetTextureNameA", &MaterialControl::EvSetTextureName );
}


bool MaterialControl::LoadTexture( const char* filename )
{
	int err = -1;

	Print("[MaterialControl::LoadTexture]\n");
	Print(" Loading texture '%s'\n", filename);

	PixelBuffer* img = PixelBuffer::Create( filename );
	if ( img )
	{
		byte* image = img->CopyPixmap();
		uint32 w = img->GetWidth();
		uint32 h = img->GetHeight();
		uint16 components = PixelBuffer::GetBytesPerPixel( img->GetPixelFormat() );

		if ( components )
		{
			err = LoadTextureBuffer( image, w, h, components*8, components );
		}
		else
		{
			Print("MaterialManager: Use RGB_24 and RGBA_32 images only.\n");
			
			if (image)
				delete [] image;

			if (err >= 0)
			{
				//freyjaMaterialTexture(freyjaGetCurrentMaterial(), err);
				//freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
				//Print("Material[%i].texture = { %i }", freyjaGetCurrentMaterial(), err);
				RefreshInterface();
			}

			return false;
		}
		
		delete [] image;
		
		//printf("[Success]\n");

		return true;
	}
	
	return false;
}


bool MaterialControl::LoadTextureBuffer(byte *image, uint32 width, uint32 height, 
								   uint32 bpp, uint32 colortype)
{
	if (image == NULL || width == 0 || height == 0 || bpp == 0)
		return false;

	int err = 0;
	Texture::ColorMode type = ((colortype == 4) ? Texture::RGBA :
							   (colortype == 3) ? Texture::RGB :
							   Texture::INDEXED);

	if (mFlags & fLoadTextureInSlot)
	{
		err = Texture::mSingleton->loadBufferSlot(image, width, height, 
												  type, bpp, mTextureId);
	}
	else
	{
		err = mTextureId = 
		Texture::mSingleton->loadBuffer(image, width, height, type, bpp);
	}

	if (err < 0)
	{
		printf("MaterialControl::LoadTextureBuffer() ERROR Loading buffer\n");
	}

	return (err == 0);
}


////////////////////////////////////////////////////////////
// Public Events 
////////////////////////////////////////////////////////////

void MaterialControl::SetAmbient(uint32 i, vec_t value)
{
#warning FIXME This method is obsolete.
	//vec4_t color;
	//freyjaGetMaterialAmbient(freyjaGetCurrentMaterial(), color);
	//color[i] = value;
	//freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
	//mgtk_event_gl_refresh();	
}


void MaterialControl::SetDiffuse(uint32 i, vec_t value)
{
#warning FIXME This method is obsolete.
}


void MaterialControl::SetSpecular(uint32 i, vec_t value)
{
#warning FIXME This method is obsolete.
}


void MaterialControl::SetEmissive(uint32 i, vec_t value)
{
#warning FIXME This method is obsolete.
}


////////////////////////////////////////////////////////////
// Events 
////////////////////////////////////////////////////////////

void MaterialControl::EvSelect(uint32 value)
{
#warning FIXME This method is obsolete.
#if 0
	if (!mgtk_event_set_range(EvSelectId, value, 
								0, freyjaGetMaterialCount()-1))
	{
		if (value != freyjaGetCurrentMaterial())
		{
			freyjaCurrentMaterial(value);
			freyja3d_print("Selected material[%i] = '%s'.", value,
						 freyjaGetMaterialName(value));
			RefreshInterface();
			RefreshContext();
		}
		
		// This is here to support the obsolete idea of texture -> mesh
		// binding, since we do material -> mesh binding now
		if (value != GetSelected())
		{
			SetSelected(value);
			RefreshContext();
		}
	}
#endif
}


void MaterialControl::EvShine(vec_t value)
{	
#warning FIXME This method is obsolete.
	//freyjaMaterialShininess(freyjaGetCurrentMaterial(), value);
	//mgtk_event_gl_refresh();
}


void MaterialControl::EvSetName(char *text)
{
	if ( text && text[0] )
	{
		freyjaMaterialSetName( GetCurrentMaterialPtr(), text);
	}
}


int16 MaterialControl::GetBlendId( const char* name ) 
{
	mstl::String s(name);
	int16 id = -1;

	if ( s == "GL_ZERO" )
		id = 0;
	else if ( s == "GL_ONE")
		id = 1;
	else if ( s == "GL_SRC_COLOR")
		id = 2;
	else if ( s == "GL_ONE_MINUS_SRC_COLOR")
		id = 3;
	else if ( s == "GL_DST_COLOR")
		id = 4;
	else if ( s == "GL_ONE_MINUS_DST_COLOR")
		id = 5;
	else if ( s == "GL_SRC_ALPHA")
		id = 6;
	else if ( s == "GL_ONE_MINUS_SRC_ALPHA")
		id = 7;
	else if ( s == "GL_DST_ALPHA")
		id = 8;
	else if ( s == "GL_ONE_MINUS_DST_ALPHA")
		id = 9;
	else if ( s == "GL_SRC_ALPHA_SATURATE")
		id = 10;
	else if ( s == "GL_CONSTANT_COLOR")
		id = 11;
	else if ( s == "GL_ONE_MINUS_CONSTANT_COLOR")
		id = 12;
	else if ( s == "GL_CONSTANT_ALPHA")
		id = 13;
	else if ( s == "GL_ONE_MINUS_CONSTANT_ALPHA")
		id = 14;

	return id;
}

const char* MaterialControl::GetBlendString( uint32 value ) 
{
	const char* s = NULL;

	switch (value)
	{
	case 0:
		s = "GL_ZERO";
		break;

	case 1:
		s = "GL_ONE";
		break;

	case 2:
		s = "GL_SRC_COLOR";
		break;

	case 3:
		s = "GL_ONE_MINUS_SRC_COLOR";
		break;

	case 4:
		s = "GL_DST_COLOR";
		break;

	case 5:
		s = "GL_ONE_MINUS_DST_COLOR";
		break;

	case 6:
		s = "GL_SRC_ALPHA";
		break;

	case 7:
		s = "GL_ONE_MINUS_SRC_ALPHA";
		break;

	case 8:
		s = "GL_DST_ALPHA";
		break;

	case 9:
		s = "GL_ONE_MINUS_DST_ALPHA";
		break;

	case 10:
		s = "GL_SRC_ALPHA_SATURATE";
		break;

	case 11:
		s = "GL_CONSTANT_COLOR";
		break;

	case 12:
		s = "GL_ONE_MINUS_CONSTANT_COLOR";
		break;

	case 13:
		s = "GL_CONSTANT_ALPHA";
		break;

	case 14:
		s = "GL_ONE_MINUS_CONSTANT_ALPHA";
		break;

	default:
		;
	}

	return s;
}


void MaterialControl::EvBlendSrc(uint32 value)
{
	freyjaMaterialSetBlendSource( GetCurrentMaterialPtr(), GetBlendString( value ) );		
	mgtk_event_gl_refresh();
}


void MaterialControl::EvBlendDest(uint32 value)
{
	freyjaMaterialSetBlendDestination( GetCurrentMaterialPtr(), GetBlendString( value ) );		
	mgtk_event_gl_refresh();
}


void MaterialControl::EvEnableDetailTexture(uint32 value)
{
#warning FIXME This method is obsolete.
#if 0
	if (value)
	{
		freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), 
							  fFreyjaMaterial_DetailTexture);
	}
	else
	{
		freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), 
								fFreyjaMaterial_DetailTexture);
	}
	freyja3d_print("Material detail texturing is [%s]", value ? "ON" : "OFF");
	mgtk_event_gl_refresh();
#endif
}


void MaterialControl::EvEnableNormalize(uint32 value)
{
#warning FIXME This method is obsolete.
#if 0
	if (value)
	{
		freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), 
							  fFreyjaMaterial_Normalize);
	}
	else
	{
		freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), 
								fFreyjaMaterial_Normalize);
	}

	freyja3d_print("Material normalization is [%s]", value ? "ON" : "OFF");
	mgtk_event_gl_refresh();
#endif
}


void MaterialControl::EvEnableBlending(uint32 value)
{
	if ( GetCurrentMaterial() )
	{
		GetCurrentMaterial()->EnableBlending( value );
		freyja3d_print( "Material blending [%s]", value ? "ON" : "OFF" );
		RefreshContext( );
	}
}


void MaterialControl::EvEnableTexture(uint32 value)
{
#warning FIXME This method is obsolete.
#if 0
	if (value)
	{
		freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), 
							  fFreyjaMaterial_Texture);
	}
	else
	{
		freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), 
								fFreyjaMaterial_Texture);
	}

	freyja3d_print("Material texture usage is [%s]", value ? "ON" : "OFF");
	mgtk_event_gl_refresh();
#endif
}


void MaterialControl::EvNewMaterial()
{
#warning FIXME This method is obsolete.
#if 0
	index_t i = freyjaMaterialCreate();
	freyja3d_print("New material [%i] created.", i);
#endif
}


void MaterialControl::EvOpenMaterial()
{
	mstl::String path = freyja_rc_map_string("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Materal...", 
							  path.c_str(), 0,
							  "Freyja Material (*.mat)", "*.mat");

	if ( filename )
	{
		if ( LoadMaterial(filename) )
		{
			RefreshInterface();
		}
	}
			
	mgtk_filechooser_blocking_free( filename );	
}


void MaterialControl::EvSaveMaterial()
{
	mstl::String path = freyja_rc_map_string("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Save Materal...", 
							  path.c_str(), 1,
							  "Freyja Material (*.mat)", "*.mat");

	if ( filename )
	{
		SaveMaterial( filename );
	}
			
	mgtk_filechooser_blocking_free( filename );	
}


void MaterialControl::EvSetTextureName(char *text)
{
#warning FIXME This method is obsolete.
	static bool haltTexture = false;

	if (!haltTexture)
	{
		haltTexture = true;
		//freyjaMaterialTextureName(freyjaGetCurrentMaterial(), text);
		haltTexture = false;
	}
}


void MaterialControl::EvSetShader(uint32 value)
{
#warning FIXME This method is obsolete.
	//freyjaMaterialShader(freyjaGetCurrentMaterial(), value);
	mgtk_event_gl_refresh();
}


void MaterialControl::EvSetTexture(uint32 value)
{
#warning FIXME This method is obsolete.
	SetSelected(value);
	//freyjaMaterialTexture(freyjaGetCurrentMaterial(), value);
	RefreshContext();
}


void MaterialControl::EvTextureUpload(uint32 id)
{
#if FIXME
	byte *image;
	uint32 w, h, bpp, type;

	/* Texture image was stored as raw buffer */
	freyjaGetTextureImage(id, w, h, bpp, type, image); // FIXME C++ exportable

	if (image)
	{
		switch (type)
		{
		case RGBA_32:
			Texture::mSingleton->loadBuffer(image, w, h, Texture::RGBA, 32);
			break;
			
		case RGB_24:
			Texture::mSingleton->loadBuffer(image, w, h, Texture::RGB, 24);
			break;

		case INDEXED_8:
			Texture::mSingleton->loadBuffer(image, w, h, Texture::INDEXED, 8);
			break;
			
		default:
			FREYJA_ASSERTMSG(false, "Image type %i is not supported.", type);
		}
	}
#endif
}


void MaterialControl::EvOpenTexture( char *text )
{
	if ( text == NULL || text[0] == 0 ) 
		return;

	PixelBuffer* img = PixelBuffer::Create( text );
	bool loaded = false;

	if ( img )
	{
		byte* image = img->CopyPixmap();
		uint32 w = img->GetWidth();
		uint32 h = img->GetHeight();
		uint16 components = PixelBuffer::GetBytesPerPixel( img->GetPixelFormat() );

		if ( image && components )
		{
			int err = LoadTextureBuffer( image, w, h, components*8, components );
			uint32 e = resourceGetEventId1s("eSetTextureNameA");
			uint32 texture = mTextureId - 1;
			freyja::Material* mat = GetCurrentMaterial();

			if ( mat )
			{
				mgtk_textentry_value_set(e, text);
				mgtk_spinbutton_value_set(EvSetTextureId, texture);

				mat->SetDecalMapId( texture );
				//freyjaMaterialSetFlag(mat, fFreyjaMaterial_Texture);
				//mat->SetTextureId( texture );
				//freyjaMaterialTextureName(mat, text);

				RefreshContext(); 
			}

			loaded = true; // In OpenGL at least.
		}

		if ( image )
			delete [] image;

		delete img;
	}

	Print( "%s %s", text, loaded ? "loaded" : "failed to load" );
}


void MaterialControl::EvOpenShader(char *text)
{
	if (text == NULL || text[0] == 0) 
		return;

	uint32 fragmentId = 0; // Gets fragment id
	bool load = false;

	if (mFlags & fUsingARBFragments && 
		freyja3d::OpenGL::LoadFragmentARB(text, fragmentId))
	{
		freyja3d::OpenGL::BindFragmentARB(fragmentId);
		load = true;
	}
	else if (freyja3d::OpenGL::LoadFragmentGLSL(text, fragmentId))
	{
		freyja3d::OpenGL::BindFragmentGLSL(fragmentId);
		load = true;
	}

	if (load)
	{
		uint32 e = 
		ResourceEvent::GetResourceIdBySymbol("eSetMaterialShaderFilename");
		
		//uint32 texture = mTextureId - 1;
		mgtk_textentry_value_set(e, text);
		
#warning FIXME This method is obsolete.
#if 0
		// Propagate to material backend
		uint32 mat = freyjaGetCurrentMaterial();
		//freyjaMaterialSetFlag(mat, fFreyjaMaterial_Shader);
		mgtk_spinbutton_value_set(EvSetShaderId, fragmentId);
		freyjaMaterialShader(mat, fragmentId);
		freyjaMaterialShaderName(mat, text);
#endif
		Print("Loaded fragment program %i", fragmentId);
		RefreshContext();
	}
}


void MaterialControl::EvARBFragmentMode(uint32 value)
{
	if (value)
	{
		SetFlag(fUsingARBFragments, true);
		mgtk_toggle_value_set(EvGLSLFragmentModeId, 0);
		Print("ARB fragment shader mode.");
	}
}


void MaterialControl::EvGLSLFragmentMode(uint32 value)
{
	if (value)
	{
		SetFlag(fUsingARBFragments, false);
		mgtk_toggle_value_set(EvARBFragmentModeId, 0);
		Print("GLSL fragment shader mode.");
	}
}


byte *MaterialControl::GenerateTextureCheckerBoard(byte bg[4], byte fg[4],
											  uint32 width, uint32 height,
											  uint32 runlen)
{
	byte *image = new unsigned char[height*width*4];
	bool swap = true;

	for (uint32 i = 0, l = 0; i < width; ++i, ++l)
	{
		for (uint32 j = 0; j < height; ) // 2px black border
		{
			byte *&rgba = (swap) ? bg : fg;			

			for (uint32 k = 0; k < runlen; ++k, ++j)
			{
				uint32 idx = ( height * i + j ) * 4;

				/* RBGA */
				image[idx]   = rgba[0];
				image[idx+1] = rgba[1];
				image[idx+2] = rgba[2];
				image[idx+3] = rgba[3];
			}

			swap = !swap;
		}

		if (l >= runlen)
		{
			l = 0;
			swap = !swap;	
		}
	}

	return image;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

uint32 MaterialControl::CreateListener(const char *name, MethodPtr ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg0<MaterialControl>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


uint32 MaterialControl::CreateListener1f(const char *name, MethodPtr1f ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, vec_t>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


uint32 MaterialControl::CreateListener1s(const char *name, MethodPtr1s ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, char *>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


uint32 MaterialControl::CreateListener1u(const char *name, MethodPtr1u ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, uint32>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// API Functions
////////////////////////////////////////////////////////////

int freyja3d_load_texture(const char* filename)
{
	int id = -1;

	if ( MaterialControl::GetInstance()->LoadTexture(filename) )
	{
		id = MaterialControl::GetInstance()->GetTextureId()-1;
	}

	freyja3d_print("! Texture callback %i : '%s'...", id, filename);

	return id;
}
	


