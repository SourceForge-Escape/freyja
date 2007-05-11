/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : perlinnoise plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <freyja/PerlinNoise.h>
#include <freyja/FreyjaImage.h>
#include <freyja/PluginABI.h>
#include <freyja/TextureABI.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>

using namespace mstl;
using namespace mgtk;

extern "C" {

	void freyja_perlinnoise_init(void (*func)(const char*, void*));
}

unsigned int gPerlinNoiseSeed = 257;
unsigned int gPerlinNoiseW = 256;
unsigned int gPerlinNoiseH = 256;
unsigned int gPerlinNoiseClamp = 1;
vec_t gPerlinNoiseIA = 1.0f;
vec_t gPerlinNoiseIB = 2.0f;
vec_t gPerlinNoiseD = 20.0f;
vec4_t gColorPerlinAdd = {0.51, 0.51, 0.29, 1.0};
vec4_t gColorPerlinMult = {0.61, 0.51, 0.35, 1.0};

void PerlinNoiseEventsAttach();
void PerlinNoiseGUIAttach();

/* This hooks up the plugin to the appliciation */
//ResourceAppPluginTest PerlinNoisePlugin(PerlinNoiseEventsAttach,
//										PerlinNoiseGUIAttach);

void *perlinnoise_captured1 = 0x0;

void freyja_perlinnoise_init(void (*func)(const char*, void*))
{
	ResourcePlugin *plugin;
	plugin = new ResourcePlugin(PerlinNoiseEventsAttach, PerlinNoiseGUIAttach);	
}


void ePerlinNoiseGen()
{
	PerlinNoise perlin;
	uint32 seed = 257, w = 256, h = 256, clamp = 1;
	vec_t iA = 1.0f, iB = 2.0f, d = 20.0f;
	byte *image;

	seed = gPerlinNoiseSeed;
	w = gPerlinNoiseW;
	h = gPerlinNoiseH;
	clamp = gPerlinNoiseClamp;
	iA = gPerlinNoiseIA;
	iB = gPerlinNoiseIB; 
	d = gPerlinNoiseD; 

	image = perlin.generateBuffer(w, h, seed);

	if (!image)
	{
		mgtk_print("ePerlinNoiseGen: Invalid image generation");
		return;
	}

	if (clamp)
		perlin.clampBufferIntensity(image, w, h, iA, iB, d);

	/* Modulate by a color and add a base half intensity */
 	FreyjaImage img;
	byte *rgb;
	uint32 i, n;
	img.loadPixmap(image, w, h, FreyjaImage::INDEXED_8);
	img.getImage(&rgb);

	if (!rgb)
	{
		mgtk_print("ePerlinNoiseGen: Invalid image after color conversion");
		return;
	}

	// hahaha it's 0600 no sleep -- can't wait to clean this prototype!
	for (i = 0, n = w * h * 3; i < n; ++i)
	{
		/* NOTE: No clamping or scaling of colors, however there is a 
		         weakened 50 / 50 add in the sense that ADD can only contrib
				 _up_to_ 50% of full intensity ( 255 ).

				The reason for this is to allow bleeding for plasma, etc.
		*/

		// Modulate and adjust intensity per pixel
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[0]) + 
				(byte)(128 * gColorPerlinAdd[0]);
		++i;
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[1]) +
				(byte)(128 * gColorPerlinAdd[1]);
		++i;
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[2]) + 
				(byte)(128 * gColorPerlinAdd[2]);
	}

#ifdef BUILT_INTO_APPLICATION
	freyja_load_texture_buffer(rgb, w, h, 24);
#else
	unsigned int tid = freyjaTextureCreateBuffer(rgb, 3, w, h, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!ePerlinNoiseGen generated texture successfully.");
	else
		mgtk_print("!ePerlinNoiseGen failed to upload texture to application.");

	freyjaTextureDelete(tid);
#endif

	mgtk_event_gl_refresh();
}


void ePerlinNoiseSeed(unsigned int i)
{
	gPerlinNoiseSeed = i;
}


void ePerlinNoiseW(unsigned int i)
{
	gPerlinNoiseW = i;
}


void ePerlinNoiseH(unsigned int i)
{
	gPerlinNoiseH = i;
}


void ePerlinNoiseClamp(unsigned int i)
{
	gPerlinNoiseClamp = i;
}


void ePerlinNoiseIA(vec_t v)
{
	gPerlinNoiseIA = v;
}


void ePerlinNoiseIB(vec_t v)
{
	gPerlinNoiseIB = v;
}


void ePerlinNoiseD(vec_t v)
{
	gPerlinNoiseD = v;
}


void eDialogPerlinNoise()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogPerlinNoise"), 1);
} 


void eColorPerlinMult(float *c, unsigned long count)
{
	uint32 i;

	for (i = 0; i < 4; ++i)
		gColorPerlinMult[i] = c[i];

	mgtk_event_set_color(Resource::mInstance->getIntByName("eColorPerlinMult"), c[0], c[1], c[2], c[3]);
	mgtk_event_gl_refresh();
}


void eColorPerlinAdd(float *c, unsigned long count)
{
	uint32 i;

	for (i = 0; i < 4; ++i)
		gColorPerlinAdd[i] = c[i];

	mgtk_event_set_color(Resource::mInstance->getIntByName("eColorPerlinAdd"), c[0], c[1], c[2], c[3]);
	mgtk_event_gl_refresh();
}


void eGenerateTextureCheckerboard()
{
	byte bg[4] = { (byte)(255 * gColorPerlinAdd[0]), (byte)(255 * gColorPerlinAdd[1]), (byte)(255 * gColorPerlinAdd[2]), (byte)(255 * gColorPerlinAdd[3]) };

	byte fg[4] = { (byte)(255 * gColorPerlinMult[0]), (byte)(255 * gColorPerlinMult[1]), (byte)(255 * gColorPerlinMult[2]), (byte)(255 * gColorPerlinMult[3]) };
					
	uint32 width = 256;
	uint32 height = 256;
	uint32 runlen = 16;
	byte *image = new unsigned char[height*width*3];
	bool swap = true;

	for (uint32 i = 0, l = 0; i < width; ++i, ++l)
	{
		for (uint32 j = 0; j < height; ) // 2px black border
		{
			byte *rgba = (swap) ? bg : fg;			

			for (uint32 k = 0; k < runlen; ++k, ++j)
			{
				uint32 idx = ( height * i + j ) * 3;

				/* RBGA */
				image[idx]   = rgba[0];
				image[idx+1] = rgba[1];
				image[idx+2] = rgba[2];
				//image[idx+3] = rgba[3];
			}

			swap = !swap;
		}

		if (l >= runlen)
		{
			l = 0;
			swap = !swap;	
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!ePerlinNoiseGen generated texture successfully.");
	else
		mgtk_print("!ePerlinNoiseGen failed to upload texture to application.");

	freyjaTextureDelete(tid);
	//delete [] image;
}

void eGenerateTextureGradiant()
{
	uint32 width = 256;
	uint32 height = 256;
	byte *image = new unsigned char[height*width*3];
	float iw = 1.0f / (float)width;
	//float ih = 1.0f / (float)height;

	for (uint32 i = 0; i < height; ++i)
	{
		for (uint32 j = 0; j < width; ++j) 
		{
			uint32 idx = ( width * i + j ) * 3;
			float u = j * iw;
			//float v = i * ih;

			const float gr = (1.0f - u);
			const float gr2 = u;

			// Diagonal
			//bg = (byte)(255 * (1.0f - ( (0.5f * u) + (0.5f * v) ) ) );
			//fg = (byte)(255 * (( (0.5f * u) + (0.5f * v) ) ) );

			for (uint32 k = 0; k < 3; ++k)
			{
				float c = gColorPerlinAdd[k] * gr + gColorPerlinMult[k] * gr2;
				image[idx+k]   = (byte)(255 * c);
			}
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!%s() generated texture successfully.", __func__);
	else
		mgtk_print("!%s() failed to upload texture to application.", __func__);

	freyjaTextureDelete(tid);
	//delete [] image;
}


void eGenerateTextureColorcube()
{
	uint32 width = 256;
	uint32 height = 256;
	byte *image = new unsigned char[height*width*3];
	float iw = 1.0f / (float)width;
	float ih = 1.0f / (float)height;

	for (uint32 i = 0; i < height; ++i)
	{
		for (uint32 j = 0; j < width; ++j) 
		{
			uint32 idx = ( width * i + j ) * 3;
			float u = j * iw;
			float v = i * ih;

			image[idx]   = (byte)(255 *v);
			image[idx+1] = (byte)(255 * (0.5f * u + 0.5f * v) );
			image[idx+2] = (byte)(255 * ( 0.5f - 0.25f * u + 0.25f * v ) );
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!%s() generated texture successfully.", __func__);
	else
		mgtk_print("!%s() failed to upload texture to application.", __func__);

	freyjaTextureDelete(tid);
	//delete [] image;
}


void ePerlinNoiseNop()
{
}


void PerlinNoiseEventsAttach()
{
	ResourceEventCallback::add("ePerlinNoisePluginsMenu", &ePerlinNoiseNop);

	ResourceEventCallback::add("ePerlinNoiseMenu", &ePerlinNoiseNop);

	ResourceEventCallback::add("ePerlinNoiseGen", &ePerlinNoiseGen);

	ResourceEventCallback::add("ePerlinNoiseCheckerboard", &eGenerateTextureCheckerboard);
	ResourceEventCallback::add("ePerlinNoiseColorcube", &eGenerateTextureColorcube);
	ResourceEventCallback::add("eGenerateTextureGradiant", &eGenerateTextureGradiant);

	


	// FIXME: Add limits and a GUI generator wrapper for this
	//        the GUI generator wrapper will have to wait until 
	//        interface is done to call itself back to generate
	// FIXME: Also find a way to make these data members of the other event
	//        if possible ( remember callbacks might need functions )
	ResourceEventCallback::add("eDialogPerlinNoise", &eDialogPerlinNoise);

	ResourceEventCallbackUInt::add("ePerlinNoiseSeed", &ePerlinNoiseSeed);
	ResourceEventCallbackUInt::add("ePerlinNoiseW", &ePerlinNoiseW);
	ResourceEventCallbackUInt::add("ePerlinNoiseH", &ePerlinNoiseH);
	ResourceEventCallbackUInt::add("ePerlinNoiseClamp", &ePerlinNoiseClamp);

	ResourceEventCallbackVec::add("ePerlinNoiseIA", &ePerlinNoiseIA);
	ResourceEventCallbackVec::add("ePerlinNoiseIB", &ePerlinNoiseIB);
	ResourceEventCallbackVec::add("ePerlinNoiseD", &ePerlinNoiseD);

	ResourceEventCallbackVecArray::add("eColorPerlinMult", &eColorPerlinMult);
	ResourceEventCallbackVecArray::add("eColorPerlinAdd", &eColorPerlinAdd);
}


void PerlinNoiseGUIAttach()
{
	char *basename = "plugins/perlinnoise.mlisp";
	char *filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	int menuId = Resource::mInstance->getIntByName("ePerlinNoiseMenu");
	int mainMenuId = Resource::mInstance->getIntByName("ePluginMenu");

	int pluginMenuId = Resource::mInstance->getIntByName("ePerlinNoisePluginsMenu");
	mgtk_append_menu_to_menu(mainMenuId, "Texture Generator", pluginMenuId);

	int id = Resource::mInstance->getIntByName("ePerlinNoiseCheckerboard");
	mgtk_append_item_to_menu(menuId, "Checkerboard Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Checkerboard", id);

	id = Resource::mInstance->getIntByName("ePerlinNoiseColorcube");
	mgtk_append_item_to_menu(menuId, "Colorcube Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Colorcube", id);

	id = Resource::mInstance->getIntByName("eGenerateTextureGradiant");
	mgtk_append_item_to_menu(menuId, "Gradiant Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Gradiant", id);

	id = Resource::mInstance->getIntByName("ePerlinNoiseGen");
	mgtk_append_item_to_menu(pluginMenuId, "Perlin Noise", id);

	int add = Resource::mInstance->getIntByName("eColorPerlinAdd");
	mgtk_event_set_color(add, 0.51f, 0.51f, 0.29f, 1.0f);
	
	int multi = Resource::mInstance->getIntByName("eColorPerlinMult");
	mgtk_event_set_color(multi, 0.61f, 0.51f, 0.35f, 1.0f);
}






