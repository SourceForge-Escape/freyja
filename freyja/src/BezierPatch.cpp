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


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

//BezierPatch::BezierPatch()
//{
//}

//BezierPatch::~BezierPatch()
//{
//}


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



////////////////////////////////////////////////////////////////////////////


/* Render patch test -- this should be model specific later */ 
void draw_patch()
{
	if (FreyjaRender::mPatchDisplayList > 0)
	{
		glCallList(FreyjaModel::gTestPatch.displayList);
		BezierPatchOpenGL::DrawPatch(FreyjaModel::gTestPatch);
	}
}


void init_patch()
{
	FreyjaRender::mPatchDisplayList = 1;
	BezierPatchOpenGL::BuildOpenGLDisplayList(FreyjaModel::gTestPatch, 7);
}


// FIXME: Generate a contiguous mesh sharing vertices and 
//        compose a single tristrip that reverses on edges
bool FreyjaModel::pasteSelectedPatch()
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
		index = freyjaVertexCreate3fv(gTestPatch.vertices[i]->mVec);
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



void FreyjaModel::movePatchControlPoint(Vector3d xyz)
{
	gTestPatch.control[gTestPatch.x][gTestPatch.y] = xyz;
}

void FreyjaModel::selectPatchControlPoint(Vector3d xyz)
{
	unsigned int besti = 0, bestj = 0, i, j;
	Vector3d u, v;
	vec_t dist, closest = 99999;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			v = xyz;
			u = Vector3d(gTestPatch.control[i][j]);
			dist = helDist3v(v.mVec, u.mVec);
			
			if (dist < closest)
			{
				closest = dist;

				besti = i;
				bestj = j;
			}
		}
	}

	gTestPatch.x = besti;
	gTestPatch.y = bestj;
}


void FreyjaModel::selectPatchControlPoint(float xx, float yy)
{
	unsigned int besti = 0, bestj = 0, i, j, x, y;
	Vector3d u, v;
	vec_t dist, closest = 99999;


	switch (getCurrentPlane())
	{
	case PLANE_XY:
		x = 0;
		y = 1;
		break;

	case PLANE_XZ:
		x = 0;
		y = 2;
		break;

	case PLANE_ZY: // side
		x = 2;
		y = 1;
		break;
	default: ;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			v = Vector3d(xx, yy, 0);
			u = Vector3d(gTestPatch.control[i][j].mVec[x], 
						 gTestPatch.control[i][j].mVec[y], 0);
			dist = helDist3v(v.mVec, u.mVec);
			
			if (dist < closest)
			{
				closest = dist;

				besti = i;
				bestj = j;
			}
		}
	}

	gTestPatch.x = besti;
	gTestPatch.y = bestj;
}


void FreyjaModel::movePatchControlPoint(float xx, float yy)
{
	switch (getCurrentPlane())
	{
	case PLANE_XY:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = yy;
		break;

	case PLANE_XZ:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = yy;
		break;

	case PLANE_ZY: // side
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = yy;
		break;
	default: ;
	}
}
