/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : BezierPatch
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class is the BezierPatch class for Freyja.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.14:
 * Mongoose - Created
 ==========================================================================*/

#include "BezierPatch.h"

#ifdef FIXME
// FIXME: Generate a contiguous mesh sharing vertices and 
//        compose a single tristrip that reverses on edges
bool pasteSelectedPatch()
{
	Vector<unsigned int> transV, tmp;
	unsigned int index, i, n, cursor, divs = 7;


	gTestPatch.generatePolygonMesh(divs);

	freyjaBegin(FREYJA_MESH);
	freyjaMeshFlags1u(0x0);

	freyjaBegin(FREYJA_VERTEX_GROUP);
	freyjaGroupCenter3f(0, 0, 0);

	for (i = gTestPatch.vertices.begin(); i < gTestPatch.vertices.end(); ++i)
	{
		index = freyjaVertex3fv(gTestPatch.vertices[i]->mVec);
		freyjaVertexNormal3f(index, 0, 1, 0);
		freyjaVertexTexCoord2fv(index, gTestPatch.texcoords[i]->mVec);
		transV.pushBack(index);
	}

	freyjaEnd(); // FREYJA_GROUP

	// FIXME: This is just a test case to get the facets right,
	//        notice some vertices will not be used that were created above
	for (i = 1, n = (divs + divs + 2) * (divs - 1); i < n; i += 2)
	{
		//freyja_print("! %i -> %i", i+divs+divs+1, i);
		tmp.pushBack(i+divs+divs+1);
		transV.assign(i+divs+divs+1, transV[i]);
	}

	//for (i = 0, n = tmp.end(); i < n; ++i)
	//	freyjaVertexDelete(tmp[i]);

	for (i = 0, cursor = divs + divs, n = (divs * (divs+1) * 2); i < n; ++i)
	{
		//if (i == 14 || i == 28 + 2 || i == 42 + 4) // Found for 7x7
		if (i == cursor)
		{
			i += 2;
			cursor += divs + divs + 2;
		}

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(gTestPatch.texture);
		freyjaPolygonVertex1i(transV[i]);
		freyjaPolygonVertex1i(transV[i+1]);
		freyjaPolygonVertex1i(transV[i+2]);
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	freyja_print("Mesh generated from patch...");

	return true;
}
#endif

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

//BezierPatch::BezierPatch()
//{
//}

BezierPatch::~BezierPatch()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_BEZIERPATCH
int runBezierPatchUnitTest(int argc, char *argv[])
{
	BezierPatch test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[BezierPatch class test]\n");

	return runBezierPatchUnitTest(argc, argv);
}
#endif
