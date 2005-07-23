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
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/PerlinNoise.h>
#include <freyja/FreyjaImage.h>

#include "freyja_events.h"
#include "Texture.h"
#include "FreyjaModel.h"


extern FreyjaModel *gFreyjaModel; // hhmm...
unsigned int gPerlinNoiseSeed = 257;
unsigned int gPerlinNoiseW = 256;
unsigned int gPerlinNoiseH = 256;
unsigned int gPerlinNoiseClamp = 1;
vec_t gPerlinNoiseIA = 1.0f;
vec_t gPerlinNoiseIB = 2.0f;
vec_t gPerlinNoiseD = 20.0f;
vec4_t gColorPerlinAdd = {0.51, 0.51, 0.29, 1.0};
vec4_t gColorPerlinMult = {0.61, 0.51, 0.35, 1.0};


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

	if (clamp)
		perlin.clampBufferIntensity(image, w, h, iA, iB, d);

	/* Modulate by a color and add a base half intensity */
 	FreyjaImage img;
	byte *rgb;
	uint32 i, n;
	img.loadPixmap(image, w, h, FreyjaImage::INDEXED_8);
	img.getImage(&rgb);

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

	gFreyjaModel->loadTextureBuffer(rgb, w, h, 24, Texture::RGB);

	freyja_event_gl_refresh();
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
	static bool on = true;

	extern void mgtk_event_dialog_visible_set(int, int);
	mgtk_event_dialog_visible_set(freyja_get_event_id_by_name("eDialogPerlinNoise"), on);

	on = !on;
} 


void PerlinNoiseEventsAttach()
{
	FreyjaEventCallback::add("ePerlinNoiseGen", &ePerlinNoiseGen);


	// FIXME: Add limits and a GUI generator wrapper for this
	//        the GUI generator wrapper will have to wait until 
	//        interface is done to call itself back to generate
	// FIXME: Also find a way to make these data members of the other event
	//        if possible ( remember callbacks might need functions )
	FreyjaEventCallback::add("eDialogPerlinNoise", &eDialogPerlinNoise);
	FreyjaEventCallback::add("eDialogPerlinNoise", &eDialogPerlinNoise);
	FreyjaEventCallbackUInt::add("ePerlinNoiseSeed", &ePerlinNoiseSeed);
	FreyjaEventCallbackUInt::add("ePerlinNoiseW", &ePerlinNoiseW);
	FreyjaEventCallbackUInt::add("ePerlinNoiseH", &ePerlinNoiseH);
	FreyjaEventCallbackUInt::add("ePerlinNoiseClamp", &ePerlinNoiseClamp);
	FreyjaEventCallbackVec::add("ePerlinNoiseIA", &ePerlinNoiseIA);
	FreyjaEventCallbackVec::add("ePerlinNoiseIB", &ePerlinNoiseIB);
	FreyjaEventCallbackVec::add("ePerlinNoiseD", &ePerlinNoiseD);
}


void PerlinNoiseGUIAttach()
{
	freyja_append_item_to_menu(ePluginMenu, "PerlinNoise", freyja_get_event_id_by_name("eDialogPerlinNoise"));

	freyja_event_set_color(eColorPerlinAdd, 0.51f, 0.51f, 0.29f, 1.0f);
	freyja_event_set_color(eColorPerlinMult, 0.61f, 0.51f, 0.35f, 1.0f);
}

/*
(dialog "Freyja :: PerlinNoise test" eDialogPerlinNoise 0
	(vbox 0 0 0 0 0
		(hbox 1 0 0 1 0
			(icon "freyja.png" IconSize_Dialog))

		(label "Perlin Noise "  0.0 0.5)

		(button "Generate texture" eMode ePerlinNoiseGen)

		(hbox 1 0 0 1 0
			(label "Seed "  0.0 0.5)
			(spinbutton 257 0 1000 ePerlinNoiseSeed))

		(hbox 1 0 0 1 0
			(label "Width "  0.0 0.5)
			(spinbutton 256 8 1024 ePerlinNoiseW))

		(hbox 1 0 0 1 0
			(label "Height "  0.0 0.5)
			(spinbutton 256 8 1024 ePerlinNoiseH))

		(hbox 1 0 0 1 0
			(label "Clamping on? "  0.0 0.5)
			(spinbutton 1 0 1 ePerlinNoiseClamp))

		(hbox 1 0 0 1 0
			(label "iA "  0.0 0.5) 
			(spinbutton2 1.0 0.0 99999.0 1.0 1.0 1.0 1 ePerlinNoiseIA))

		(hbox 1 0 0 1 0
			(label "iB "  0.0 0.5) 
			(spinbutton2 2.0 0.0 99999.0 1.0 1.0 1.0 1 ePerlinNoiseIB))

		(hbox 1 0 0 1 0
			(label "d "  0.0 0.5) 
			(spinbutton2 20.0 0.0 100.0 1.0 1.0 1.0 3 ePerlinNoiseD))

		(hbox 1 0 0 1 2 
			(label "Modulate color " 0.0 0.5)
			(colorbutton 1 eColorPerlinMult))

		(hbox 1 0 0 1 2 
			(label "Add color " 0.0 0.5)
			(colorbutton 1 eColorPerlinAdd))))
*/



FreyjaAppPluginTest PerlinNoiseAppPlugin(PerlinNoiseEventsAttach, PerlinNoiseGUIAttach);



