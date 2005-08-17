/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : reverseengineer plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
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
#include <mstl/String.h>
#include <freyja/FreyjaPluginABI.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_lineageIIextract_init(void (*func)(const char*, void*));
}

void L2ExtractEventsAttach();
void L2ExtractGUIAttach();

char *gL2ExtractFilename = 0x0;
char *gL2ExtractOut = 0x0;
uint32 gL2ExtractOffset = 0;
uint32 gL2ExtractSize = 0;


void freyja_lineageIIextract_init(void (*func)(const char*, void*))
{
	ResourceAppPluginTest *plugin;
	plugin = new ResourceAppPluginTest(L2ExtractEventsAttach,
										L2ExtractGUIAttach);	
}


// From my pak vfs plugin for lineageII, which is too buggy to put in svn
byte *extract_from_lineageII(const char *filename, uint32 offset, uint32 size)
{
	FILE *f;
	byte *buffer = 0x0;
	uint32 signature, i;
	byte key;
	char stringHeader[16];
	char stringVersion[16];


	f = fopen(filename, "rb");

	if (!f)
	{
		perror(filename);
		return 0x0;
	}

	fread(&signature, 4, 1, f);

	/* UT package signature 0x9E2a83c1 */
	switch (signature)
	{
	case 0x9E2a83c1:
		printf("Standard Unreal package format?\n");
		break;


	case 0x0069004c:
		/*  16 bytes Header   ( 16bit char string )
		 *  12 bytes Version  ( 16bit char string )
		 * [ 2 bytes Minor? ] ( Always Ver111 0x53 0x52  "SR"? )
		 * [ 2 bytes longer?] ( Always Ver121 0x50E44D0F ".M.P"? )
		 *
		 *  The rest seems to be XOR encrypted data by 0xAC in Ver111	
		 */

		fread(stringHeader, 12, 1, f);
		fread(stringVersion, 12, 1, f);
	
		/* Mongoose: 2 Byte wide chars used in string header, 
			assuming english, so strip every other byte */
		stringHeader[5] = stringHeader[6];
		stringHeader[4] = stringHeader[4];
		stringHeader[3] = stringHeader[2];
		stringHeader[2] = stringHeader[0];
		stringHeader[6] = stringHeader[8];
		stringHeader[7] = stringHeader[10];
		stringHeader[1] = ((char *)(&signature))[2];
		stringHeader[0] = ((char *)(&signature))[0];
		stringHeader[8] = 0;

		for (i = 1; i < 6; ++i)
		{
			stringVersion[i] = stringVersion[i*2];
		}

		stringVersion[6] = 0;

		printf("Lineage II encrypted package '%s' '%s'\n", 
				stringHeader, stringVersion);

		if (strcmp(stringVersion, "Ver121") == 0)
		{
			/* Multiple valid keys, so let's look for common 0 byte */
			fseek(f, 0x23, SEEK_SET);
			fread(&key, 1, 1, f);
		}
		else
		{
			key = 0xAC;  /* Ver111 key is a constant */
		}

		fseek(f, offset, SEEK_SET);
		buffer = new byte[size];
		fread(buffer, 1, size, f);

		for (i = 0; i < size; ++i)
		{
			buffer[i] ^= key;
		}

		fclose(f);
		break;


	default:
		printf("Not a known UT package 0x%x\n", signature);
	}


	return buffer;
}


void eL2Extract()
{
	FILE *f;
	byte *buffer = 0x0;

	buffer = extract_from_lineageII(gL2ExtractFilename,
                                    gL2ExtractOffset,
                                    gL2ExtractSize);

	if (buffer)
	{
		f = fopen(gL2ExtractOut, "wb");

		if (f)
		{
			fwrite(buffer, 1, gL2ExtractSize, f);
			fclose(f);
			mgtk_print("! Wrote file '%s'", gL2ExtractOut);
		}

		delete [] buffer;
	}
}


void eL2ExtractFilename(char *s)
{
	if (gL2ExtractFilename)
		delete [] gL2ExtractFilename;

	gL2ExtractFilename = String::strdup(s);
}


void eL2ExtractOut(char *s)
{
	if (gL2ExtractOut)
		delete [] gL2ExtractOut;

	gL2ExtractOut= String::strdup(s);
}


void eL2ExtractOffset(unsigned int value)
{
	gL2ExtractOffset = value;
}


void eL2ExtractSize(unsigned int value)
{
	gL2ExtractSize = value;
}


void eDialogL2Extract()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogL2Extract"), 1);
} 


void L2ExtractEventsAttach()
{
	ResourceEventCallback::add("eL2Extract", &eL2Extract);
	ResourceEventCallbackString::add("eL2ExtractFilename", &eL2ExtractFilename);
	ResourceEventCallbackString::add("eL2ExtractOut", &eL2ExtractOut);
	ResourceEventCallbackUInt::add("eL2ExtractOffset", &eL2ExtractOffset);
	ResourceEventCallbackUInt::add("eL2ExtractSize", &eL2ExtractSize);
	ResourceEventCallback::add("eDialogL2Extract", &eDialogL2Extract);
}


void L2ExtractGUIAttach()
{
	char *filename;
	char *basename = "plugins/lineageIIextract.mlisp";
	int id, menuId;
	uint32 e;


	id = Resource::mInstance->getIntByName("eDialogL2Extract");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	mgtk_append_item_to_menu(menuId, "LineageII extractor", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	e = resourceGetEventId1s("eL2ExtractOut");
	mgtk_textentry_value_set(e, "/tmp/dump");
}

