/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "EggPlugin.h" // temp for testing
#include "FreyjaPluginABI.h"




// TODO parent plugin class

/* Mesh generation utils */
void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	long index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index =freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.5));

	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.75));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 1.0));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.50));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.50));


	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[4]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[5]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonTexCoord1i(texcoords[6]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[7]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[8]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[9]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[10]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[11]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[12]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[13]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[14]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[15]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[16]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[17]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[18]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[19]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[20]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[21]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[22]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[23]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	long i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	count = freyjaGetCount(FREYJA_VERTEX); 

	list = new Vector<unsigned int>();

	if (count == FREYJA_PLUGIN_ERROR)
	{
		return list;
	}


	/* Using egg iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_LIST_CURRENT));
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(vec3_t bbox[2],
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_LIST_CURRENT));
				}
			}
		}
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


void freyjaPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->freyjaPrintMessage(format, &args);
	va_end(args);
}


void freyjaPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->freyjaPrintError(format, &args);
	va_end(args);
}


void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		freyjaPrintMessage("freyjaGenerateUVFromXYZ> ERROR: Invalid UV");
		return;
	}

	*u = (xyz[0] > 0) ? xyz[0] : -xyz[0];
	s = 0.025;
  
	while (*u > 1.0)
	{
		*u *= s;
		s *= 0.01;
	}
  
	*v = (xyz[1] > 0) ? xyz[1] : -xyz[1];
	s = 0.025;
  
	while (*v > 1.0)
	{
		*v *= s;
		s *= 0.01;
	}
  
	*u = 1.0 - *u;
	*v = 1.0 - *v;
}


void freyjaGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, vertexCount, faceCount;
	long v0, v1, v2, index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");

	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);

		freyjaPrintMessage("<%d %d %d>", v0, v1, v2);
		freyjaGetVertexByIndex3fv(v0, a.mVec);
		freyjaGetVertexByIndex3fv(v1, b.mVec);
		freyjaGetVertexByIndex3fv(v2, c.mVec);

		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));

		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

		if (index < 0)
		{
			freyjaPrintError("freyjaGenerateVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		ref = freyjaGetVertexPolygonRef();

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			normal += *faceNormals[ref[j]];
		}

		normal.normalize();

		freyjaVertexNormal3fv(index,normal.mVec);

		freyjaPrintMessage("%d :: %d faces :: %f %f %f", index,
						   ref.size(),
						   normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
    }

	faceNormals.erase();
	
	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBoundingVolume(BoundingVolume &vol)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	count = freyjaGetCount(FREYJA_VERTEX); 

	if (count < 1)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

		freyjaGetVertexByIndex3fv(index, xyz);
		
		if (vol.isVertexInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesInBox(vec3_t bbox[2],
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
		freyjaGetVertexByIndex3fv(index, xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(index);
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


////////////////////////////////////////////////////////////////////


void freyjaBegin(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBegin(type);
}


void freyjaEnd()
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaEnd();
}


long freyjaTexCoord2f(vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTexCoord2f(u, v);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaVertexStore3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexWeightStore(unsigned int index, 
						  vec_t weight, unsigned int bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexWeight(index, weight, bone);
}


void freyjaVertexTexCoord2f(unsigned int vIndex, vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexTexCoord2f(vIndex, u, v);
}


void freyjaVertexNormal3f(unsigned int vIndex, vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexNormal3f(vIndex, x, y, z);
}


void freyjaPolygonVertex1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonVertex1i(egg_id);
}


void freyjaPolygonTexCoord1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonTexCoord1i(egg_id);
}


void freyjaPolygonMaterial1i(int id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonMaterial1i(id);  
}


unsigned long freyjaGetFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaFlags();

	return 0;
}


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGroupCenter(x, y, z);
}


void freyjaBoneName(char *name)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneName(name);
}


void freyjaBonePos3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBonePos(x, y, z);
}


void freyjaBoneFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneFlags(flags);
}


void freyjaBoneAddMesh1u(unsigned int mesh)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneAddMesh(mesh);
}


void freyjaBoneAddChild1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneAddChild(tag);
}


void freyjaMeshTreeAddFrame3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshTreeAddFrame(x, y, z);
}


void freyjaBoneRotate3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneRotate(x, y, z);
}


void freyjaBoneRotateQuaternion4f(vec_t w, vec_t x, vec_t y, vec_t z)
{
	vec_t heading, bank, attitude;
	Quaternion q = Quaternion(w, x, y, z);

	q.getEulerAngles(&heading, &bank, &attitude);

	heading *= 57.295779513082323;
	bank *= 57.295779513082323;
	attitude *= 57.295779513082323;

	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBoneRotate(heading, bank, attitude);
}


void freyjaMeshTreeFrameAddBone(long tag)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshTreeFrameAddBone(tag);
}


int freyjaGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;

	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureFilename(index, 
															   filename);
	return FREYJA_PLUGIN_ERROR;	
}


int freyjaGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureImage(index, w, h, 
														 depth, type, image);
	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureStoreFilename(char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreFilename(filename);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureStoreBuffer(unsigned char *image, unsigned int depth,
							  unsigned int width, unsigned int height,
							  freyja_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreBuffer(image, depth,
															width, height,
															type);

	return FREYJA_PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

long freyjaGetCount(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCount(type);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaIterator(freyja_object_t type, int item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaIterator(type, item);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaGetVertexByIndex3fv(long index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexByIndex(index, xyz);
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertex(xyz);
}


void freyjaGetVertexNormal3f(vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(nxyz);
}


long freyjaGetPolygon1u(freyja_object_t type, long item, long *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetPolygon3f(freyja_object_t type, long item, vec_t *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaCriticalSection(freyja_lock_t request)
{
	freyjaPrintMessage("freyjaCriticalSection> Not implemented, %s:%i\n", 
					   __FILE__, __LINE__);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneName(unsigned int index, unsigned int size, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


void freyjaSetBoneParent(int index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);


	if (bone)
	{
		bone->parent = index;
	}	
}


long freyjaGetBoneParent(long index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);


	if (bone)
	{
		return bone->parent;
	}

	return -2;
}


long freyjaGetBoneRotationWXYZ4fv(unsigned int index, vec4_t wxyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		Quaternion q = Quaternion(bone->rot[0], bone->rot[1], bone->rot[2]);
		q.getQuaternion4fv(wxyz);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneRotationXYZ3fv(unsigned int index, vec3_t xyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		xyz[0] = bone->rot[0];
		xyz[1] = bone->rot[1];
		xyz[2] = bone->rot[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneTranslation3fv(unsigned int index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneMeshIndex(long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshIndex(element);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetCurrent(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCurrent(type);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneMeshCount()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshCount();

	return FREYJA_PLUGIN_ERROR;
}


void freyjaGetBoneRotate3f(vec_t *x, vec_t *y, vec_t *z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetBoneRotate(x, y, z);
}


void freyjaMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshFlags1u(flags);	
}



