/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created
 ==========================================================================*/

#include <math.h>
#include "VertexABI.h"

#include "Mesh.h"
#include "MeshABI.h"

using namespace freyja;


Vector<Mesh *> Mesh::mGobalPool;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mesh::Mesh()
{
	uint32 i, count;

	/* Setup gobal pool UID reference */
	mOldUID = INDEX_INVALID;
	mUID = count = mGobalPool.size();

	for (i = 0; i < count; ++i)
	{
		if (mGobalPool[i] == 0x0)
		{
			mUID = i;
			break;
		}	
	}

	mGobalPool.assign(mUID, this);
}


Mesh::~Mesh()
{
	/* Mark NULL in pool, marking free slot */
	mGobalPool.assign(mUID, 0x0);
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 Mesh::getCount()
{
	return mGobalPool.size();
}


Mesh *Mesh::getMesh(index_t meshIndex)
{
	if (meshIndex < mGobalPool.size())
		return mGobalPool[meshIndex];

	return 0x0;
}


uint32 Mesh::getPolygonCount()
{
	return polygons.end();
}


uint32 Mesh::getUID()
{
	return mUID;
}


uint32 Mesh::getVertexCount()
{
	return vertices.end(); 
}


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
// C ABI
////////////////////////////////////////////////////////////

index_t gFreyjaCurrentMesh = INDEX_INVALID;

index_t freyjaGetCurrentMesh()
{
	return gFreyjaCurrentMesh;
}


index_t freyjaMeshCreate()
{
	Mesh *mesh = new Mesh();
	return mesh->getUID();
}


void freyjaMeshDelete(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
		delete mesh;
}


void freyjaMeshTransform(index_t meshIndex,
						 freyja_transform_action_t action, 
						 vec_t x, vec_t y, vec_t z)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (!mesh)
		return;

	switch (action)
	{
	case fTranslate:
		break;

	case fRotate:
		break;

	case fScale:
		break;

	case fScaleAboutPoint:
		break;

	case fRotateAboutPoint:
		break;
	}

	freyjaPrintMessage("freyjaMeshTransform> No appropriate transform, file bug with %s", EMAIL_ADDRESS);
}

void freyjaMeshUVMapPlanar(index_t meshIndex)
{
#ifdef FIXME
	Mesh *mesh = Mesh::getMesh(meshIndex);
	index_t poly;
	index_t vertex;
	index_t texel;
	uint32 i, j, icount, jcount;
	vec_t u, v;


	if (mesh == 0x0)
		return;

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{  
		poly = egg->getPolygon(mesh->polygon[i]);

		if (!poly)
			continue;

		// Generate texels and record their ids
		for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
		{
			vertex = egg->getVertex(poly->vertex[j]);

			if (!vertex)
				continue;

			// Mongoose 2002.01.18, Generate UV from vertex XYZ
			freyjaGenerateUVFromXYZ(vertex->pos, &u, &v);

			if (poly->texel.empty())
			{
				texel = 0x0;
			}
			else
			{
				texel = egg->getTexel(poly->texel[j]);
			}
			
			if (!texel)
			{
				vertex->uv[0] = u;
				vertex->uv[1] = v;
			}
			else
			{
				texel->st[0] = u;
				texel->st[1] = v;
			}
		}
	}
#else
	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);
#endif
}


void freyjaMeshUVMapSpherical(index_t meshIndex)
{
	uint32 i, vertexCount;
	index_t vertexIndex;
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uv;


	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		
		freyjaGetVertexPosition3fv(vertexIndex, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = latitude;

		freyjaVertexTexCoord2fv(vertexIndex, uv);
	}
}


void freyjaMeshMaterial(index_t meshIndex, index_t materialIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);
	index_t poly;
	uint32 i, count;
	

	if (mesh == 0x0 || materialIndex == INDEX_INVALID)
	{
		freyjaPrintMessage("freyjaMeshMaterial> ERROR: Invalid mesh[%i]\n",
						   meshIndex);
		return;
	}

	mesh->mMaterial = materialIndex;

	count = freyjaGetMeshPolygonCount(materialIndex);

	for (i = 0; i < count; ++i)
	{  
		poly = freyjaGetMeshPolygonIndex(materialIndex, i);

		if (poly == INDEX_INVALID)
			continue;

		freyjaPolygonMaterial(poly, materialIndex);
	}
}


void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh && polygonIndex != INDEX_INVALID)
	{
		mesh->polygons.pushBack(polygonIndex);
	}
}


void freyjaMeshClampTexCoords(index_t meshIndex)
{
	uint32 i, count;
	vec2_t uv;
	index_t item;


	/* Clamp texcoords */
	count = freyjaGetMeshTexCoordCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		item = freyjaGetMeshTexCoordIndex(meshIndex, i);
		freyjaGetTexCoord2fv(item, uv);

		if (uv[0] < 0.0f)
			uv[0] = 0.0f;
		else if (uv[0] > 1.0f)
			uv[0] = 1.0f;

		if (uv[1] < 0.0f)
			uv[1] = 0.0f;
		else if (uv[1] > 1.0f)
			uv[1] = 1.0f;
		
		freyjaTexCoord2fv(item, uv);
	}


	/* Clamp vertex UVs*/
	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		item = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexTexCoord2fv(item, uv);

		if (uv[0] < 0.0f)
			uv[0] = 0.0f;
		else if (uv[0] > 1.0f)
			uv[0] = 1.0f;

		if (uv[1] < 0.0f)
			uv[1] = 0.0f;
		else if (uv[1] > 1.0f)
			uv[1] = 1.0f;
		
		freyjaVertexTexCoord2fv(item, uv);
	}
}


void freyjaMeshUVMapCylindrical(index_t meshIndex)
{
	int32 i, j, vertexCount;
	index_t vertexIndex;
	vec_t longitude, latitude, ysize;
	vec3_t xyz;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};
	vec2_t uv;
	

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexPosition3fv(vertexIndex, xyz);

		for (j = 0; j < 3; ++j)
		{
			if (xyz[j] < min[j])
				min[j] = xyz[j];

			if (xyz[j] > max[j])
				max[j] = xyz[j];
		}
	}

	if (max[1] >= 0)
	{
		if (min[1] >= 0)
		{
			ysize = max[1] - min[1];
		}
		else
		{
			ysize = max[1] + -min[1];
		}
	}
	else
	{
		ysize = -max[1] + min[1];
	}

	if (ysize < 0.0001 && ysize > -0.0001)
		ysize = 1.0f;

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexPosition3fv(vertexIndex, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = xyz[1] / ysize;

		freyjaVertexTexCoord2fv(vertexIndex, uv);
	}
}


void freyjaMeshPromoteTexcoordsToPloymapping(index_t meshIndex)
{
	vec2_t uv;
	uint32 i, j, k, vertexCount, polygonCount;
	index_t vertexIndex, texcoordIndex, polygonIndex;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

    for (i = 0; i < polygonCount; ++i)
    {
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			// NOTE: I just update all UV -> polymapp to avoid corrupt 
			//       'texture faces' eg not completely polymapped
			freyjaPolygonTexCoordPurge(polygonIndex);

			for (k = 0; k < vertexCount; ++k) 
			{
				vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				freyjaGetVertexTexCoord2fv(vertexIndex, uv);

				texcoordIndex = freyjaTexCoordCreate2fv(uv);
				freyjaPolygonAddTexCoord(polygonIndex, texcoordIndex);
			}
		}
	}
}



void freyjaMeshGenerateVertexNormals(index_t meshIndex)
{
	Vector<Vector3d *> faceNormals;
	Vector<index_t> ref;
	Vector3d a, b, c, aa, bb, normal;
	uint32 i, j, meshVertexCount, polygonCount, vertexCount, refCount;
	index_t v0, v1, v2, vertexIndex, polygonIndex, refIndex;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	meshVertexCount = freyjaGetMeshVertexCount(meshIndex);

	if (!meshVertexCount || !polygonCount)
		return;

    for (i = 0; i < polygonCount; ++i)
    {
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		
		/* Just need 3 sides -- assume all coplanar these are simple polygons,
		 * so just take first 3 vertices to make 2 edge vectors */
		v0 = freyjaGetPolygonVertexIndex(polygonIndex, 0);
		v1 = freyjaGetPolygonVertexIndex(polygonIndex, 1);
		v2 = freyjaGetPolygonVertexIndex(polygonIndex, 2);

		freyjaPrintMessage("%i <%d %d %d>", polygonIndex, v0, v1, v2);
		freyjaGetVertexPosition3fv(v0, a.mVec);
		freyjaGetVertexPosition3fv(v1, b.mVec);
		freyjaGetVertexPosition3fv(v2, c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));
	}


	/* Compute vertex normals */
    for (i = 0; i < meshVertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, i);

		if (vertexIndex < 0)
		{
			freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		refCount = freyjaGetVertexPolygonRefCount(vertexIndex);

		for (j = 0; j < refCount; ++j)
		{
			refIndex = freyjaGetVertexPolygonRefIndex(vertexIndex, j);

			if (refIndex > polygonCount)
			{
				freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR bad face\n");
				continue;
			}

			normal += *faceNormals[refIndex];
		}

		normal.normalize();

		freyjaVertexNormal3fv(vertexIndex, normal.mVec);
    }

	faceNormals.erase();
}


void freyjaMeshTesselateTriangles(index_t meshIndex)
{
	Vector<long> purge;
	uint32 i, polygonCount, vertexCount;
	index_t polygonIndex, a, b, c, d, ta, tb, tc, td, material;
	index_t polygon;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		material = freyjaGetPolygonMaterial(polygonIndex);

		if (polygonIndex != INDEX_INVALID)
		{
			vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			switch (vertexCount)
			{
			case 3:
				// freyjaPrintMessage("You already have a triangle dumbass!");
				break;
			
			case 4:
				/* 1. Get ABCD quad vertices */
				a = freyjaGetPolygonVertexIndex(polygonIndex, 0);
				b = freyjaGetPolygonVertexIndex(polygonIndex, 1);
				c = freyjaGetPolygonVertexIndex(polygonIndex, 2);
				d = freyjaGetPolygonVertexIndex(polygonIndex, 3);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					ta = freyjaGetPolygonTexCoordIndex(polygonIndex, 0);
					tb = freyjaGetPolygonTexCoordIndex(polygonIndex, 1);
					tc = freyjaGetPolygonTexCoordIndex(polygonIndex, 2);
					td = freyjaGetPolygonTexCoordIndex(polygonIndex, 3);
				}


				/* 2. Make ABC ACD triangles */
				polygon = freyjaPolygonCreate();
				freyjaMeshAddPolygon(meshIndex, polygon);
				freyjaPolygonMaterial(polygon, material);
				freyjaPolygonAddVertex(polygon, a);
				freyjaPolygonAddVertex(polygon, b);
				freyjaPolygonAddVertex(polygon, c);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					freyjaPolygonAddTexCoord(polygon, ta);
					freyjaPolygonAddTexCoord(polygon, tb);
					freyjaPolygonAddTexCoord(polygon, tc);
				}

				polygon = freyjaPolygonCreate();
				freyjaMeshAddPolygon(meshIndex, polygon);
				freyjaPolygonMaterial(polygon, material);
				freyjaPolygonAddVertex(polygon, a);
				freyjaPolygonAddVertex(polygon, c);
				freyjaPolygonAddVertex(polygon, d);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					freyjaPolygonAddTexCoord(polygon, ta); // should dupe a?
					freyjaPolygonAddTexCoord(polygon, tc);
					freyjaPolygonAddTexCoord(polygon, td);
				}

				/* 3. Prepare to remove ABCD polygon and update references */
				purge.pushBack(polygonIndex);

			default:
				freyjaPrintMessage("General polygons not supported in this branch, so please use quads and triangles in import/export system.");
			}
		}
	}


	/* Remove old polygons that were tesselated from the mesh */
	for (i = purge.begin(), polygonCount = purge.end(); i < polygonCount; ++i)
	{
		freyjaMeshRemovePolygon(meshIndex, purge[i]);
	}
}


void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);
	Vector<index_t> keep;
	uint32 i, count;


	if (mesh)
	{
		count = mesh->polygons.end();

		for (i = 0; i < count; ++i)
		{
			if (mesh->polygons[i] != polygonIndex)
			{
				keep.pushBack(mesh->polygons[i]);
			}
		}

		mesh->polygons.clear();

		for (i = 0, count = keep.end(); i < count; ++i)
		{
			mesh->polygons.pushBack(keep[i]);
		}
	}
}


void freyjaMeshNormalFlip(index_t meshIndex)
{
	int32 i, n, v;

	for (i = 0, n = freyjaGetMeshVertexCount(meshIndex); i < n; ++i)
	{
		v = freyjaGetMeshVertexIndex(meshIndex, i);

		if (v > -1)
		{
			freyjaVertexNormalFlip(v);
		}
	}
}


void freyjaMeshName1s(index_t meshIndex, const char *name)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
	{
		strncpy(mesh->mName, name, 64);
		mesh->mName[63] = 0;
	}
}


uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex)
{
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	int32 i, j, frames, maxFrames = 0, polygonIndex;
	int32 vertexIndex, vertexCount;

	/* Mongoose 2005.01.01, 
	 * Vertex frames are stored in the vertices themselves, find the 
	 * max frame count -- indexing greater than frame count will
	 * return frame[0] position in vertices with less than maxFrames
	 * frame, so it's safe and does what one would expect asymmetrically */
	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, j);
			frames = freyjaGetVertexFrameCount(vertexIndex);

			if (frames > maxFrames)
				maxFrames = frames;
		}
	}

	return maxFrames;
}


uint32 freyjaGetMeshPolygonCount(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
	{
		return mesh->polygons.end();
	}

	return 0;
}


index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex, uint32 element)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh && element < mesh->vertices.size())
	{
		return mesh->vertices[element];
	}

	return INDEX_INVALID;
}


index_t freyjaGetMeshVertexIndex(index_t meshIndex, uint32 element)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh && element < mesh->vertices.size())
	{
		return mesh->vertices[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexCount(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
	{
		return mesh->vertices.size();
	}

	return 0;
}


index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, uint32 element)
{
//	Mesh *mesh = Mesh::getMesh(meshIndex);

	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

// 	if (mesh && element < mesh->texcoords.size())
// 	{
// 		return mesh->texcoords[element];
// 	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshTexCoordCount(index_t meshIndex)
{
// 	Mesh *mesh = Mesh::getMesh(meshIndex);

	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

// 	if (mesh && element < mesh->texcoords.size())
// 	{
// 		return mesh->texcoords.size();
// 	}

	return 0;
}


index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh && element < mesh->polygons.size())
	{
		return mesh->polygons[element];
	}

	return INDEX_INVALID;
}


void freyjaMeshPosition(index_t meshIndex, const vec3_t xyz)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
	{
		mesh->mPosition = Vector3d(xyz);
	}
}


byte freyjaIsMeshAllocated(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh != 0x0)
		return 1;
	
	return 0;
}


void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh != 0x0)
	{
		xyz[0] = mesh->mPosition.mVec[0];
		xyz[1] = mesh->mPosition.mVec[1];
		xyz[2] = mesh->mPosition.mVec[2];
	}
}


void freyjaMeshFlags(index_t meshIndex, byte flags)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh != 0x0)
		mesh->mFlags = flags;	
}


byte freyjaGetMeshFlags(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh != 0x0)
	{
		return mesh->mFlags;
	}

	return 0;
}


char *freyjaGetMeshNameString(index_t meshIndex)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (mesh)
	{
		return mesh->mName;
	}

	return 0x0;
}


void freyjaGetMeshName1s(index_t meshIndex, uint32 lenght, char *name)
{
	Mesh *mesh = Mesh::getMesh(meshIndex);

	if (!name || !name[0])
		return;

	name[0] = 0;

	if (mesh)
	{
		if (lenght > 64)
			lenght = 63;

		strncpy(name, mesh->mName, lenght);
		name[lenght-1] = 0;
	}
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAMESH
int runFreyjaMeshUnitTest(int argc, char *argv[])
{
	FreyjaMesh test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaMesh class test]\n");

	return runFreyjaMeshUnitTest(argc, argv);
}
#endif
