/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Sims
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the SIMS plugin module.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.26:
 * Mongoose - Created
 ==========================================================================*/


#include "Sims.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Sims::Sims()
{
}


Sims::~Sims()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool IFFPak::check(const char *filename)
{
	FreyjaFileReader r;


	if (!r.openFile(filename))
		return false;

	/* Data is BIG_ENDIAN */
	r.bigEndianMode();

	r.readCharString(35, mId);
	r.readCharString(25, mId2);
	r.closeFile();
	
	if (strncmp(mId, "IFF FILE 2.5:TYPE FOLLOWED BY SIZE", 34))
		return false;

	if (strncmp(mId2, " JAMIE DOORNBOS & MAXIS 1", 25))
		return false;

	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool IFFPak::load(const char *filename)
{
	FreyjaFileReader r;

	if (!r.openFile(filename))
		return false;

	/* Data is BIG_ENDIAN */
	r.bigEndianMode();

	r.readCharString(35, mId);
	r.readCharString(25, mId2);
	
	if (strncmp(mId, "IFF FILE 2.5:TYPE FOLLOWED BY SIZE", 34))
		return false;

	if (strncmp(mId2, " JAMIE DOORNBOS & MAXIS 1", 25))
		return false;

	mResourceMapOffset = r.readLong();
	printf("mResourceMapOffset = %li\n", mResourceMapOffset);

	while (loadChunk(r))
		;

	r.closeFile();

	return true;
}


bool IFFPak::loadChunk(FreyjaFileReader &r)
{
	long type, size, off;
	short id, flags;
	char buffer[128];
	char name[4];


	type = r.readLong();

	if (r.endOfFile())
		return false;

	size = r.readLong();
	id = r.readInt16();
	flags = r.readInt16();
	r.readCharString(64, buffer);	
	memcpy(name, (char*)&type, 4);

	printf("%c%c%c%c Chunk: 0x%lx %li %i 0x%x '%s'\n", 
		   name[3], name[2], name[1], name[0],
		   type, size, id, flags, buffer);

	off = (size - 76) + r.getFileOffset();

	switch (type)
	{
	case IFF_BCON:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_BHAV:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_BMP_:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_CTSS:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_DGRP:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_FWAV:
		printf("FWAV {\n");
		r.readCharString(14, buffer);
		printf("\tWav = '%s'\n", buffer);
		printf("}\n");
		r.setFileOffset(off);
		return true;
		break;

	case IFF_OBJD:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_PALT:
		{
			unsigned char *rgbPal = new unsigned char[784];

			r.readLong(); // Always 1
			r.readLong(); // Always 256
			r.readLong(); // Always 0
			r.readLong(); // Always 0

			r.readBuffer(784, rgbPal);
			mPalettes.pushBack(rgbPal);
		}
		r.setFileOffset(off);
		return true;
		break;

	case IFF_rsmp:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_SLOT:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_SPR2:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_STRN:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_TTAs:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_TTAB:
		r.setFileOffset(off);
		return true;
		break;

	case IFF_XXXX:
		r.setFileOffset(off);
		return true;
		break;

	default:
		printf("UNKNOWN{\n}\n");

		printf("#define IFF_%c%c%c%c 0x%lx\n",
			   name[3], name[2], name[1], name[0],
			   type);

		printf("\n");

		printf("case IFF_%c%c%c%c:\nr.setFileOffset(off);\nreturn true;\nbreak;\n", name[3], name[2], name[1], name[0]);

		printf("\n");

		r.setFileOffset(off);
	}

	return false;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_SIMS
int runSimsUnitTest(int argc, char *argv[])
{
	Sims test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Sims class test]\n");

	return runSimsUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <stdio.h>
#include <freyja/FreyjaPlugin.h>


extern "C" {
	int freyja_model__sims_check(char *filename);
	int freyja_model__sims_import(char *filename);
	int freyja_model__sims_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("SIMS Pak (*.iff)");
	freyjaPluginAddExtention1s("iff");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__sims_check(filename))
		return freyja_model__sims_import(filename);

	return -1;
}


int freyja_model__sims_check(char *filename)
{
	IFFPak sims;

	return ((sims.check(filename) == true) ? 0 : -1);
}


int freyja_model__sims_import(char *filename)
{
  Vector<long> transV;
  IFFPak sims;


  if (sims.load(filename) == false)
    return -1;

#ifdef FIXME
  freyjaBegin(FREYJA_MODEL);
  freyjaBegin(FREYJA_MESH);
  freyjaBegin(FREYJA_VERTEX_GROUP);

  for (i = 0; i < sims.VertexCount(); i++)
  {
    v = freyjaVertex3f(vert[0], vert[1], vert[2]);
    trans.Add(i, v);
  }

  freyjaEnd(); // FREYJA_GROUP

  for (i = 0; i < sims.FaceCount(); i++)
  {
    freyjaBegin(FREYJA_POLYGON);
    
    for (j = 0; j < n; ++j)
    {
		freyjaPolygonVertex1i(transV[]);
		index = freyjaTexCoord2f(, );
		freyjaPolygonTexCoord1i(index);
	}
    }

    freyjaPolygonMaterial1i(-1);

    freyjaEnd(); // FREYJA_POLYGON
  }

  freyjaEnd(); // FREYJA_MESH
  freyjaEnd(); // FREYJA_MODEL
#endif
  
  return 0;
}


int freyja_model__sims_export(char *filename)
{
	freyjaPrintError("freyja_model__sims_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);
	return -1;
}
#endif
