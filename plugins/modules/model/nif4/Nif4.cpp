/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Nif4
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: NetImmerse Model 4.0 class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.08.16:
 * Mongoose - Created
 ==========================================================================*/

#include "Nif4.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Nif4::Nif4()
{
	mHeader[0] = 0;
	mHeaderSz = 48; /* 38? This was 128 in 3.x */
}


Nif4::~Nif4()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Nif4::loadModel(const char *filename)
{
	SystemIO::FileReader r;
	Vector<NiNode *> ninodes;
	Chunk chunk;


	if (r.Open(filename) == false)
		return false;

	/* Get header and check to see if this is a Nifv4.0.0.2 */
	r.ReadString(mHeaderSz, mHeader);

	if (strncmp(NIF4_HEADER_START, mHeader, 34))
		return false;

	while (chunk.readHeader(r) && !r.IsEndOfFile())
	{
		printf("Ni Chunk: '%s'\n", chunk.name);

		if (!strncmp(chunk.name, "NiNode", 6))
		{
			NiNode *ninode = new NiNode();			
			ninodes.pushBack(ninode);
			ninode->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiTextKeyExtraData", 18))
		{
			NiTextKeyExtraData *chunk = new NiTextKeyExtraData();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiTriShapeData", 14))
		{
			NiTriShapeData *chunk = new NiTriShapeData();			
			mNiTriShapeData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiTriShape", 10))
		{
			NiTriShape *nitrishape = new NiTriShape();			
			//nitrishapes.pushBack(nitrishape);
			nitrishape->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiGeomMorpherController", 23))
		{
			 NiGeomMorpherController *chunk = new NiGeomMorpherController();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiMorphData", 11))
		{
			 NiMorphData *chunk = new NiMorphData();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiTexturingProperty", 19))
		{
			 NiTexturingProperty *chunk = new NiTexturingProperty();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiSourceTexture", 15))
		{
			 NiSourceTexture *chunk = new NiSourceTexture();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else if (!strncmp(chunk.name, "NiMaterialProperty", 18))
		{
			 NiMaterialProperty *chunk = new NiMaterialProperty();			
			//mNiTextKeyExtraData.pushBack(chunk);
			chunk->readChunk(r);
		}
		else
		{
			printf("Ni Chunk: '%s', %i @ %li not implemented\n",
				   chunk.name, chunk.len, r.GetOffset());

			r.Close();
			break;
		}
	}

	r.Close();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Plugin hooks
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <freyja/FreyjaPluginABI.h>

extern "C" {

	int freyja_model__nif4_check(char *filename);
	int freyja_model__nif4_import(char *filename);
	int freyja_model__nif4_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginDescription1s("NetImmerse 4 Model (*.nif)");
	freyjaPluginAddExtention1s("*.nif");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__nif4_check(filename))
		return freyja_model__nif4_import(filename);

	return -1;
}


int freyja_model__nif4_check(char *filename)
{	
	SystemIO::FileReader r;
	char header[48];
	unsigned int headerSz = 48;

	if (r.Open(filename) == false)
		return -1;

	/* Get header and check to see if this is a Nifv4.0.0.2 */
	r.ReadString(headerSz, header);
	r.Close();

	if (strncmp(NIF4_HEADER_START, header, 34))
		return -1;

	return 0;
}


int freyja_model__nif4_import(char *filename)
{
	Vector<unsigned int> vertices;
	Vector<unsigned int> texcoords;
	Nif4 nif4;
	long i, j, idx;


	if (nif4.loadModel(filename) == false)
		return -1;

	freyjaBegin(FREYJA_MODEL);


	for (i = nif4.mNiTriShapeData.begin(); i < (int)nif4.mNiTriShapeData.end(); ++i)
	{
		Nif4::NiTriShapeData *data = nif4.mNiTriShapeData[i];

		if (!data)
			continue;

		vertices.clear();
		texcoords.clear();

		freyjaBegin(FREYJA_MESH);
		freyjaBegin(FREYJA_VERTEX_GROUP);

		for (j = 0; j < data->num_vertices; ++j)
		{
			idx = freyjaVertexCreate3f(data->coordinates[j].x,
								 data->coordinates[j].z,
								 data->coordinates[j].y);

			freyjaVertexNormal3f(idx,
								 data->normals[j].x,
								 data->normals[j].z,
								 data->normals[j].y);

			if (data->num_uv_sets > 0)
			{
				freyjaTexCoord2f(idx,
								 data->uv_set[0].textureinfo[j].u,
								 data->uv_set[0].textureinfo[j].v);
			}

			vertices.pushBack(idx);
		}

		freyjaEnd(); // FREYJA_VERTEX_GROUP

		for (j = 0; j < data->num_triangles; ++j)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonMaterial1i(0);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[0]]);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[1]]);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[2]]);
			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH		
	}

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__nif4_export(char *filename)
{
	freyjaPrintError("nif4_export> ERROR: Not implemented.\n");
	return -1;
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_NIF4
int runNif4UnitTest(int argc, char *argv[])
{
	Nif4 nif4;

	if (nif4.loadModel(argv[1]) == false)
		return -1;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Nif4 class test]\n");

	return runNif4UnitTest(argc, argv);
}
#endif
