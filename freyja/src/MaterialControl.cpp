/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
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
#include <freyja/FreyjaImage.h>
#include <freyja/MaterialABI.h>
#include <freyja/TextureABI.h>
#include <freyja/PerlinNoise.h>
#include "Texture.h"
#include "FreyjaOpenGL.h"

#include "MaterialControl.h"

using namespace freyja3d;
	

MaterialControl *MaterialControl::mHack = NULL;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MaterialControl::MaterialControl() : EvSelectId(0), EvShineId(0), EvSetNameId(0), 
						   EvSetTextureNameId(0), mFlags(fNone)
{
	// We move to class based index_t later instead integer ids
	for (uint32 i = 0; i < 4; ++i)
	{
		EvAmbientId[i] = EvDiffuseId[i] = EvEmissiveId[i] = EvSpecularId[i] = 0;
	}

	mHack = this;
}


MaterialControl::~MaterialControl()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

// Replace 'freyja' Material format with 'libfreyja' Material format + metadata
bool MaterialControl::LoadMaterial(const char *filename)
{
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

	int32 matIndex;
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
	if (perlinLoaded)
	{
		PerlinNoise perlin;
		byte *image = perlin.generateBuffer(width, height, seed);
		
		if (image)
		{	
			if (clamp)
				perlin.clampBufferIntensity(image, width, height, iA, iB, d);
			
			/* Modulate by a color and add a base half intensity */
			FreyjaImage img;
			byte *rgb;

			img.loadPixmap(image, width, height, FreyjaImage::INDEXED_8);
			img.getImage(&rgb);

			for (uint32 i = 0, n = width * height * 3; i < n; ++i)
			{
				/* NOTE: No clamping or scaling of colors, however there is a 
				   weakened 50 / 50 add in the sense that ADD can only contrib
				   _up_to_ 50% of full intensity ( 255 ).

				   The reason for this is to allow bleeding for plasma, etc.
				*/

				// Modulate and adjust intensity per pixel
				rgb[i] = (byte)(rgb[i] * modcolor[0]) +	(byte)(128 * addcolor[0]);
				++i;
				rgb[i] = (byte)(rgb[i] * modcolor[1]) +	(byte)(128 * addcolor[1]);
				++i;
				rgb[i] = (byte)(rgb[i] * modcolor[2]) +	(byte)(128 * addcolor[2]);
			}

			LoadTextureBuffer(rgb, width, height, 24, 3);
			freyjaMaterialTexture(matIndex, mTextureId-1);
			freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
		}
	}

	RefreshInterface();

	return true;
}


// Replace 'freyja' Material format with 'libfreyja' Material format + metadata
bool MaterialControl::SaveMaterial(const char *filename)
{
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
}


void MaterialControl::RefreshInterface()
{
	uint32 mIndex = freyjaGetCurrentMaterial();
		
	mgtk_textentry_value_set(EvSetNameId, freyjaGetMaterialName(mIndex));
		
	mgtk_spinbutton_value_set(EvSetShaderId, freyjaGetMaterialShader(mIndex));

	mgtk_textentry_value_set(EvSetShaderFilenameId, 
							 freyjaGetMaterialShaderName(mIndex));
		
	mgtk_spinbutton_value_set(EvSetTextureId,
							  freyjaGetMaterialTexture(mIndex));

	mgtk_textentry_value_set(EvSetTextureNameId,
							 freyjaGetMaterialTextureName(mIndex));
		
	vec4_t ambient;
	freyjaGetMaterialAmbient(mIndex, ambient);
	freyja_event_set_color(eColorMaterialAmbient, 
						   ambient[0], ambient[1], ambient[2], ambient[3]);
	for (uint32 i = 0; i < 4; ++i)
	{
		mgtk_spinbutton_value_set(EvAmbientId[i], ambient[i]);	
	}


	vec4_t diffuse;
	freyjaGetMaterialDiffuse(mIndex, diffuse);
	freyja_event_set_color(eColorMaterialDiffuse, 
						   diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	for (uint32 i = 0; i < 4; ++i)
	{
		mgtk_spinbutton_value_set(EvDiffuseId[i], diffuse[i]);
	}


	vec4_t specular;
	freyjaGetMaterialSpecular(mIndex, specular);
	freyja_event_set_color(eColorMaterialSpecular, 
						   specular[0], specular[1], specular[2], specular[3]);
	for (uint32 i = 0; i < 4; ++i)
	{
		mgtk_spinbutton_value_set(EvSpecularId[i], specular[i]);
	}


	vec4_t emissive;
	freyjaGetMaterialEmissive(mIndex, emissive);
	freyja_event_set_color(eColorMaterialEmissive, 
						   emissive[0], emissive[1], emissive[2], emissive[3]);
	for (uint32 i = 0; i < 4; ++i)
	{
		mgtk_spinbutton_value_set(EvEmissiveId[i], emissive[i]);
	}

	vec_t shininess = freyjaGetMaterialShininess(mIndex);
	mgtk_spinbutton_value_set(EvShineId, shininess);

	uint32 src = freyjaGetMaterialBlendSource(mIndex);
	mgtk_option_menu_value_set(eBlendSrcMenu, src);

	uint32 dest = freyjaGetMaterialBlendDestination(mIndex);
	mgtk_option_menu_value_set(eBlendDestMenu, dest);

	uint32 flags = freyjaGetMaterialFlags(mIndex);

	mgtk_togglebutton_value_set(EvEnableBlendingId, 
								(flags & fFreyjaMaterial_Blending));

	mgtk_togglebutton_value_set(EvEnableTextureId, 
								(flags & fFreyjaMaterial_Texture));

	// Just go ahead and render a new frame in case the function calling
	// this fails to do so.
	RefreshContext();
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void MaterialControl::AttachMethodListeners()
{
	CreateListener("eSetMaterialShaderFilename", &MaterialControl::EvNop);

	EvSelectId = CreateListener("eSetMaterial", &MaterialControl::EvSelect);
	EvAmbientId[0] = CreateListener("eMaterialAmbient0", &MaterialControl::EvAmbient0);
	EvAmbientId[1] = CreateListener("eMaterialAmbient1", &MaterialControl::EvAmbient1);
	EvAmbientId[2] = CreateListener("eMaterialAmbient2", &MaterialControl::EvAmbient2);
	EvAmbientId[3] = CreateListener("eMaterialAmbient3", &MaterialControl::EvAmbient3);
	EvDiffuseId[0] = CreateListener("eMaterialDiffuse0", &MaterialControl::EvDiffuse0);
	EvDiffuseId[1] = CreateListener("eMaterialDiffuse1", &MaterialControl::EvDiffuse1);
	EvDiffuseId[2] = CreateListener("eMaterialDiffuse2", &MaterialControl::EvDiffuse2);
	EvDiffuseId[3] = CreateListener("eMaterialDiffuse3", &MaterialControl::EvDiffuse3);
	EvSpecularId[0] = CreateListener("eMaterialSpecular0", &MaterialControl::EvSpecular0);
	EvSpecularId[1] = CreateListener("eMaterialSpecular1", &MaterialControl::EvSpecular1);
	EvSpecularId[2] = CreateListener("eMaterialSpecular2", &MaterialControl::EvSpecular2);
	EvSpecularId[3] = CreateListener("eMaterialSpecular3", &MaterialControl::EvSpecular3);
	EvEmissiveId[0] = CreateListener("eMaterialEmissive0", &MaterialControl::EvEmissive0);
	EvEmissiveId[1] = CreateListener("eMaterialEmissive1", &MaterialControl::EvEmissive1);
	EvEmissiveId[2] = CreateListener("eMaterialEmissive2", &MaterialControl::EvEmissive2);
	EvEmissiveId[3] = CreateListener("eMaterialEmissive3", &MaterialControl::EvEmissive3);
	EvShineId = CreateListener("eMaterialShine", &MaterialControl::EvShine);
	EvSetNameId = CreateListener("eSetMaterialName", &MaterialControl::EvSetName);
	EvSetShaderId =CreateListener("eSetMaterialShader", &MaterialControl::EvSetShader);

	EvSetTextureId = 
	CreateListener("eSetMaterialTexture", &MaterialControl::EvSetTexture);

	EvSetTextureNameId = 
	CreateListener("eSetTextureNameA", &MaterialControl::EvSetTextureName);

	CreateListener("eNewMaterial", &MaterialControl::EvNewMaterial);
	CreateListener("eOpenMaterial", &MaterialControl::EvOpenMaterial);
	CreateListener("eSaveMaterial", &MaterialControl::EvSaveMaterial);

	EvEnableBlendingId = 
	CreateListener("eEnableMaterialBlending", &MaterialControl::EvEnableBlending);

	EvEnableTextureId = 
	CreateListener("eEnableMaterialTexture", &MaterialControl::EvEnableTexture);

	CreateListener("eEnableDetailTexture", &MaterialControl::EvEnableDetailTexture);

	CreateListener("eTextureSlotLoad", &MaterialControl::EvTextureSlotLoad);
	CreateListener("eMaterialSlotLoad", &MaterialControl::EvMaterialSlotLoad);

	CreateListener("eEnableNormalize", &MaterialControl::EvEnableNormalize);

	EvGLSLFragmentModeId =
	CreateListener("eGLSLFragmentMode", &MaterialControl::EvGLSLFragmentMode);

	EvARBFragmentModeId = 
	CreateListener("eARBFragmentMode", &MaterialControl::EvARBFragmentMode);

	CreateListener("eOpenShader", &MaterialControl::EvOpenShader);
	CreateListener("eOpenTexture", &MaterialControl::EvOpenTexture);

	CreateListener("eBlendSrc", &MaterialControl::EvBlendSrc);
	CreateListener("eBlendDest", &MaterialControl::EvBlendDest);

	CreateListener("eTextureUpload", &MaterialControl::EvTextureUpload);

	//CreateListener("", &MaterialControl::);
}


bool MaterialControl::LoadTexture(const char *filename)
{
	int err = -1;

	// Mongoose 2002.01.10, Evil...
	if (SystemIO::File::CompareFilenameExtention(filename, ".lst") == 0)
	{
		FILE *f = fopen(filename, "r");
		
		if (!f)
		{
			perror(filename);
			return false;
		}

		const unsigned int bufferSize = 256;
		unsigned int i = 0;
		char buffer[bufferSize];
		char c;

		while (fscanf(f, "%c", &c) != EOF)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
				break;
			case ';':
				Print("Loading texture from list '%s'\n", buffer);
				LoadTexture(buffer);
				
				i = 0;
				buffer[0] = 0;
				break;
			default:
				if (i > bufferSize-1)
					i = bufferSize-1;
				
				buffer[i++] = c;
				buffer[i] = 0;
			}
		} 
		
		fclose(f);

		return true;
	}
	
	Print("[FreyjaModel::loadTexture]\n");
	Print(" Loading texture '%s'\n", filename);

	FreyjaImage img;
	unsigned char *image;
	unsigned int w, h;

	if (!img.loadImage(filename))
	{
		img.getImage(&image);
		w = img.getWidth();
		h = img.getHeight();

		switch (img.getColorMode())
		{
		case FreyjaImage::RGBA_32:
			err = LoadTextureBuffer(image, w, h, 32, 4);//Texture::RGBA);
			break;

		case FreyjaImage::RGB_24:
			err = LoadTextureBuffer(image, w, h, 24, 3);//Texture::RGB);
			break;

		case FreyjaImage::INDEXED_8:
			err = LoadTextureBuffer(image, w, h, 8, 1);//Texture::INDEXED);
			break;

		default:
			Print("MaterialManager: Use RGB_24 and RGBA_32 images only.\n");
			
			if (image)
				delete [] image;

			if (err >= 0)
			{
				freyjaMaterialTexture(freyjaGetCurrentMaterial(), err);
				freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
				Print("Material[%i].texture = { %i }", freyjaGetCurrentMaterial(), err);
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
	vec4_t color;
	freyjaGetMaterialAmbient(freyjaGetCurrentMaterial(), color);
	color[i] = value;
	freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
	freyja_event_gl_refresh();	
}


void MaterialControl::SetDiffuse(uint32 i, vec_t value)
{
	vec4_t color;
	freyjaGetMaterialDiffuse(freyjaGetCurrentMaterial(), color);
	color[i] = value;
	freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
	freyja_event_gl_refresh();
}


void MaterialControl::SetSpecular(uint32 i, vec_t value)
{
	vec4_t color;
	freyjaGetMaterialSpecular(freyjaGetCurrentMaterial(), color);
	color[i] = value;
	freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
	freyja_event_gl_refresh();
}


void MaterialControl::SetEmissive(uint32 i, vec_t value)
{
	vec4_t color;
	freyjaGetMaterialEmissive(freyjaGetCurrentMaterial(), color);
	color[i] = value;
	freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
	freyja_event_gl_refresh();
}


////////////////////////////////////////////////////////////
// Events 
////////////////////////////////////////////////////////////

void MaterialControl::EvSelect(uint32 value)
{
	if (!freyja_event_set_range(EvSelectId, value, 
								0, freyjaGetMaterialCount()-1))
	{
		if (value != freyjaGetCurrentMaterial())
		{
			freyjaCurrentMaterial(value);
			freyja_print("Selected material[%i] = '%s'.", value,
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
}


void MaterialControl::EvShine(vec_t value)
{	
	freyjaMaterialShininess(freyjaGetCurrentMaterial(), value);
	freyja_event_gl_refresh();
}


void MaterialControl::EvSetName(char *text)
{
	if (text && text[0])
	{
		freyjaMaterialName(freyjaGetCurrentMaterial(), text);
	}
}


void MaterialControl::EvBlendSrc(uint32 value)
{
	index_t material = freyjaGetCurrentMaterial();

	switch (value)
	{
	case 0:
		freyjaMaterialBlendSource(material, GL_ZERO);
		break;

	case 1:
		freyjaMaterialBlendSource(material, GL_ONE);
		break;

	case 2:
		freyjaMaterialBlendSource(material, GL_SRC_COLOR);
		break;

	case 3:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_SRC_COLOR);
		break;

	case 4:
		freyjaMaterialBlendSource(material, GL_DST_COLOR);
		break;

	case 5:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_DST_COLOR);
		break;

	case 6:
		freyjaMaterialBlendSource(material, GL_SRC_ALPHA);
		break;

	case 7:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case 8:
		freyjaMaterialBlendSource(material, GL_DST_ALPHA);
		break;

	case 9:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_DST_ALPHA);
		break;

	case 10:
		freyjaMaterialBlendSource(material, GL_SRC_ALPHA_SATURATE);
		break;

	case 11:
		freyjaMaterialBlendSource(material, GL_CONSTANT_COLOR);
		break;

	case 12:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_CONSTANT_COLOR);
		break;

	case 13:
		freyjaMaterialBlendSource(material, GL_CONSTANT_ALPHA);
		break;

	case 14:
		freyjaMaterialBlendSource(material, GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

	default:
		freyja_print("Unknown Blend Source event %i.", value);
	}
		
	freyja_event_gl_refresh();
}


void MaterialControl::EvBlendDest(uint32 value)
{
	index_t material = freyjaGetCurrentMaterial();

	switch (value)
	{
	case 0:
		freyjaMaterialBlendDestination(material, GL_ZERO);
		break;

	case 1:
		freyjaMaterialBlendDestination(material, GL_ONE);
		break;

	case 2:
		freyjaMaterialBlendDestination(material, GL_SRC_COLOR);
		break;

	case 3:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_SRC_COLOR);
		break;

	case 4:
		freyjaMaterialBlendDestination(material, GL_DST_COLOR);
		break;

	case 5:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_DST_COLOR);
		break;

	case 6:
		freyjaMaterialBlendDestination(material, GL_SRC_ALPHA);
		break;

	case 7:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case 8:
		freyjaMaterialBlendDestination(material, GL_DST_ALPHA);
		break;

	case 9:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_DST_ALPHA);
		break;

	case 10:
		freyjaMaterialBlendDestination(material, GL_SRC_ALPHA_SATURATE);
		break;

	case 11:
		freyjaMaterialBlendDestination(material, GL_CONSTANT_COLOR);
		break;

	case 12:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_CONSTANT_COLOR);
		break;

	case 13:
		freyjaMaterialBlendDestination(material, GL_CONSTANT_ALPHA);
		break;

	case 14:
		freyjaMaterialBlendDestination(material, GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

	default:
		freyja_print("Unknown Blend Dest event %i.", value);
	}

	freyja_event_gl_refresh();
}


void MaterialControl::EvEnableDetailTexture(uint32 value)
{
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
	freyja_print("Material detail texturing is [%s]", value ? "ON" : "OFF");
	freyja_event_gl_refresh();
}


void MaterialControl::EvEnableNormalize(uint32 value)
{
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

	freyja_print("Material normalization is [%s]", value ? "ON" : "OFF");
	freyja_event_gl_refresh();
}


void MaterialControl::EvEnableBlending(uint32 value)
{
	if (value)
	{
		freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), 
							  fFreyjaMaterial_Blending);
	}
	else
	{
		freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), 
								fFreyjaMaterial_Blending);
	}

	freyja_print("Material blending [%s]", value ? "ON" : "OFF");
	freyja_event_gl_refresh();
}


void MaterialControl::EvEnableTexture(uint32 value)
{
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

	freyja_print("Material texture usage is [%s]", value ? "ON" : "OFF");
	freyja_event_gl_refresh();
}


void MaterialControl::EvNewMaterial()
{
	index_t i = freyjaMaterialCreate();
	freyja_print("New material [%i] created.", i);
}


void MaterialControl::EvOpenMaterial(char *filename)
{
	if (LoadMaterial(filename))
	{
		RefreshInterface();
	}
}


void MaterialControl::EvSaveMaterial(char *filename)
{
	SaveMaterial(filename);
}


void MaterialControl::EvSetTextureName(char *text)
{
	static bool haltTexture = false;

	if (!haltTexture)
	{
		haltTexture = true;
		freyjaMaterialTextureName(freyjaGetCurrentMaterial(), text);
		haltTexture = false;
	}
}


void MaterialControl::EvSetShader(uint32 value)
{
	freyjaMaterialShader(freyjaGetCurrentMaterial(), value);
	freyja_event_gl_refresh();
}


void MaterialControl::EvSetTexture(uint32 value)
{
	SetSelected(value);
	freyjaMaterialTexture(freyjaGetCurrentMaterial(), value);
	RefreshContext();
}


void MaterialControl::EvTextureUpload(uint32 id)
{
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
}


void MaterialControl::EvOpenTexture(char *text)
{
	if (text == NULL || text[0] == 0) 
		return;

	bool loaded = LoadTexture(text);
	if (loaded)
	{
		uint32 e = resourceGetEventId1s("eSetTextureNameA");
		uint32 texture = mTextureId - 1;
		uint32 mat = freyjaGetCurrentMaterial();

		mgtk_textentry_value_set(e, text);
		freyjaMaterialSetFlag(mat, fFreyjaMaterial_Texture);
		mgtk_spinbutton_value_set(EvSetTextureId, texture);
		freyjaMaterialTexture(mat, texture);
		freyjaMaterialTextureName(mat, text);

		RefreshContext(); //Dirty();
	}

	Print("%s %s", text, loaded ? "loaded" : "failed to load");
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
		
		// Propagate to material backend
#if 1
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


uint32 MaterialControl::CreateListener(const char *name, MethodPtr1f ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, vec_t>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


uint32 MaterialControl::CreateListener(const char *name, MethodPtr1s ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, char *>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


uint32 MaterialControl::CreateListener(const char *name, MethodPtr1u ptr)
{
	MethodDelegate *d = 
	new MethodDelegateArg1<MaterialControl, uint32>(this, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////




