/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the 3DS plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-09:
 * Mongoose - Created
 ==========================================================================*/

#include <freyja_model/EggPlugin.h>
#include <stdio.h>

#include "3ds.h"



/* Export as C functions */
extern "C" {

	int freyja_model__3ds_check(char *filename);
	int freyja_model__3ds_import(char *filename);
	int freyja_model__3ds_export(char *filename);
}


int freyja_model__3ds_check(char *filename)
{
	char header[8];
	FILE *f;


	f = fopen(filename, "rb");
	
	if (!f)
		return -1;
	
	fread(&header, 8, 1, f);
	fclose(f);
	
	if (header[0] == 'M' && header[1] == 'M')
	{
		return 0;
	}
	else
	{
		return -3;
	}
}


int freyja_model__3ds_import(char *filename)
{
	Map<unsigned int, unsigned int> trans;
	Map<unsigned int, unsigned int> trans2;
	unsigned int i, t, v, o;

	
	if (freyja_model__3ds_check(filename) < 0)
	{
		fprintf(stderr, "freyja_model__3ds_import> Failed to load file\n");
		return -1;
	}

	File3ds tds(filename);

	//word		numFaces		(word object);
	//word		numVertices		(word object);
	//XYZ3DS	getOrigin		(word object);
	//XYZ3DS	getMidpoint		(word object);
	//char* 	getName			(word object);
	//Face3DS*	getFaceHead		(word object);
	//XYZ3DS*	getVertexHead	(word object);
	//MatMap*	getMatMap		(word object);
	
	for (o = 0; o < tds.numObjects(); ++o) 
	{
        XYZ3DS *verts = tds.getVertexHead(o);
		MatMap *uvs = tds.getMatMap(o);
		Face3DS *faces = tds.getFaceHead(o);
		unsigned int vertCount = tds.numVertices(o);
		unsigned int faceCount = tds.numFaces(o);
		

		// Start a new mesh
		eggBegin(FREYJA_MESH);
		
		// Start a new vertex group
		eggBegin(FREYJA_GROUP);  
		
		// Clear for each group
		trans.Clear();

		for (i = 0; i < vertCount; i++) 
		{
			// Store vertices in group
			v = eggVertexStore3f(verts[i].x, verts[i].z, -verts[i].y); // XZY
			
			// Generates id translator list
			trans.Add(i, v);
		}
		
		eggEnd(); // GROUP
		
		for (i = 0; i < faceCount; i++) 
		{
			// Start a new polygon
			eggBegin(FREYJA_POLYGON);
			
			// Store vertices by true id, using translator list
			eggVertex1i(trans[faces[i].vcA]);
			eggVertex1i(trans[faces[i].vcB]);
			eggVertex1i(trans[faces[i].vcC]);

			// Store texels by true id, using translator list
			if (uvs)
			{
				t = eggTexCoordStore2f(uvs[faces[i].vcA].U, uvs[faces[i].vcA].V);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(uvs[faces[i].vcB].U, uvs[faces[i].vcB].V);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(uvs[faces[i].vcC].U, uvs[faces[i].vcC].V);
				eggTexCoord1i(t);				
			}
			else
			{
				t = eggTexCoordStore2f(0.0, 0.5);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(0.5, 0.5);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(0.0, 0.0);
				eggTexCoord1i(t);
			}

			if (faces[i].material)
			{
				t = eggTextureStoreFilename(faces[i].material->texture1.filename);
				eggTexture1i(t);
			}
			else
			{
				eggTexture1i(0);
			}

			eggEnd(); // polygon
		}

		eggEnd(); // mesh
	}

	return 0;
}


int freyja_model__3ds_export(char *filename)
{
	printf("freyja_model__3ds_export> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);
	
	return -1;
}

