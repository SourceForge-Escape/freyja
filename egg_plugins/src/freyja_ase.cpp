/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the ASE plug-in module
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

#include <string.h>
#include <stdio.h>

#include "EggPlugin.h"

#include "Ase.h"


extern "C" {

	int freyja_model__ase_check(char *filename);
	int freyja_model__ase_import(char *filename);
	int freyja_model__ase_export(char *filename);
}


int freyja_model__ase_check(char *filename)
{
	char header[64];
	FILE *f;
	char c;
	
	
	f = fopen(filename, "rb");
	
	if (!f)
		return -1;
	
	fread(&header, 32, 1, f);      
	fclose(f);
  
	header[31] = 0;
	c = header[19];
	header[19] = 0;  

	if (strcmp(header, "*3DSMAX_ASCIIEXPORT") == 0)
		return 0;
	else
		return -3;
}


int freyja_model__ase_import(char *filename)
{
	Ase ase;
	Map<unsigned int, unsigned int> trans;
	Map<unsigned int, unsigned int> trans2;
	unsigned int i, v, t;


	if (ase.load(filename))
	{
		fprintf(stderr, "ase_import> Failed to load file\n");
		return -1;
	}

	ase.printInfo();
	
	printf("ase_import> WARNING Not fully implemented\n");
	
	// Start a new mesh
	eggBegin(FREYJA_MESH);
	
	// Start a new vertex group
	eggBegin(FREYJA_GROUP);
	
	for (i = 0; (int)i < ase.mVertexCount; ++i)
	{
		// Store vertices in group
		v = eggVertexStore3f(ase.mVertices[i][0], 
							 ase.mVertices[i][1], 
							 ase.mVertices[i][2]);
		
		// Generates id translator list
		trans.Add(i, v);
	}
	
	eggEnd(); // FREYJA_GROUP
	
	for (i = 0; (int)i < ase.mUVWCount; ++i)
	{
		// Store texels
		t = eggTexelStore2f(ase.mUVWs[i][0], ase.mUVWs[i][1]);
		
		// Generates id translator list
		trans2.Add(i, t);
	}
	
	for (i = 0; (int)i < ase.mFaceCount; ++i)
	{
		// Start a new polygon
		eggBegin(FREYJA_POLYGON);

		// Store vertices and texels by true id, using translator lists
		eggVertex1i(trans[ase.mFaces[i].vertIndex[0]]);
		eggTexel1i(trans2[ase.mFaces[i].uvwIndex[0]]);
		eggVertex1i(trans[ase.mFaces[i].vertIndex[1]]);
		eggTexel1i(trans2[ase.mFaces[i].uvwIndex[1]]);
		eggVertex1i(trans[ase.mFaces[i].vertIndex[2]]);
		eggTexel1i(trans2[ase.mFaces[i].uvwIndex[2]]);
		
		eggTexture1i(0);
		
		eggEnd(); // FREYJA_POLYGON
	}

	if (ase.mNormalCount > 0)
	{
		for (i = 0; (int)i < ase.mVertexCount; ++i)
		{
			eggVertexNormalStore3f(trans[i],
								   ase.mNormals[i][0], 
								   ase.mNormals[i][1], 
								   ase.mNormals[i][2]);
		}
	}
	
	eggEnd(); // FREYJA_MESH
	
	return 0;
}


int freyja_model__ase_export(char *filename)
{
	Map<unsigned int, unsigned int> trans;
	unsigned int vert;
	float st[2];
	int v, t, texel;
	Ase ase;
	

	printf("ase_export> WARNING Not fully implemented\n");
	
	if (!eggGetNum(FREYJA_MESH))
	{
		return -1;
	}
	
	// Don't allow use of internal iterators or
	// changes of data by other processes
	eggCriticalSection(EGG_WRITE_LOCK);
	
	ase.mVertexCount = eggGetNum(FREYJA_VERTEX);
	ase.mUVWCount = eggGetNum(FREYJA_POLYGON) * 3;
	ase.mFaceCount = eggGetNum(FREYJA_POLYGON); 
	
	ase.mVertices = new vec3_t[ase.mVertexCount];
	//ase.mNormals = new vec3_t[ase.mVertexCount];
	ase.mFaces = new ase_triangle_t[ase.mUVWCount];
	ase.mUVWs = new vec3_t[ase.mUVWCount];
	
	/* Using egg iterator interface */
	eggIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (v = 0; v < ase.mVertexCount; ++v)
	{
		eggGetVertex3f(ase.mVertices[v]);
		
		// Use translator list
		vert = eggIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
		trans.Add(vert, v);
		
		printf("trans.Add(%i, %i)\n", vert, v);
		eggIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}
	
	// Using list interface, as opposed to array
	eggIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
	
	for (t = 0, texel = 0; t < ase.mFaceCount; ++t)
	{
		// Use translator list
		eggGetPolygon1u(FREYJA_VERTEX, 0, &vert);
		
		//printf("trans[%i] = %i\n", vert, trans[vert]);
		
		ase.mFaces[t].vertIndex[0] = trans[vert];
		eggGetPolygon1u(FREYJA_VERTEX, 1, &vert);
		ase.mFaces[t].vertIndex[1] = trans[vert];
		eggGetPolygon1u(FREYJA_VERTEX, 2, &vert);
		ase.mFaces[t].vertIndex[2] = trans[vert];
		
		// FIXME: Texels aren't right for multiple texels per vertex
		eggGetPolygon3f(FREYJA_TEXEL, 0, st);
		ase.mFaces[t].uvwIndex[0] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[0]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[0]][1] = st[1];
		
		eggGetPolygon3f(FREYJA_TEXEL, 1, st);
		ase.mFaces[t].uvwIndex[1] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[1]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[1]][1] = st[1];
		
		eggGetPolygon3f(FREYJA_TEXEL, 2, st);
		ase.mFaces[t].uvwIndex[2] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[2]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[2]][1] = st[1];
		
		// Using list interface, as opposed to array
		eggIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}
	
	eggCriticalSection(EGG_WRITE_UNLOCK);
	
	return (ase.save(filename));
}

