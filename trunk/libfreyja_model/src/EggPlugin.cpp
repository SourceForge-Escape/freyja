/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001-02-24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>

#include "EggPlugin.h"


EggPlugin *EggPlugin::mEggPlugin = 0x0;


void eggPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintMessage(format, &args);
	va_end(args);
}


void eggPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintError(format, &args);
	va_end(args);
}


int eggGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		printf("eggGenerateUVFromXYZ> ERROR: Passed invalid data");
		return -1;
	}

	*u = (xyz[0] > 0) ? xyz[0] : -xyz[0];
	s = 0.0025;
  
	while (*u > 1.0)
	{
		*u *= s;
		s *= 0.01;
	}
  
	*v = (xyz[1] > 0) ? xyz[1] : -xyz[1];
	s = 0.0025;
  
	while (*v > 1.0)
	{
		*v *= s;
		s *= 0.01;
	}
  
	*u = 1.0 - *u;
	*v = 1.0 - *v;

	return 0;
}


// FIXME: Normals need fixing 
void eggGenerateCube(vec_t sz)
{
	unsigned int v, t;
	Map <unsigned int, unsigned int> texel;
	Map <unsigned int, unsigned int> vertex;


	// Start a new mesh
	eggBegin(FREYJA_MESH);

	// Start a new vertex group
	eggBegin(FREYJA_GROUP);

	// Allocate vertices, FIXME normals are incorrect
	v = eggVertexStore3f(0.0, sz, 0.0);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(0, v);

	v = eggVertexStore3f(sz, sz, 0.0);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(1, v);
	
	v = eggVertexStore3f(sz, 0.0, 0.0);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(2, v);

	v = eggVertexStore3f(0.0, 0.0, 0.0);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(3, v);

	v = eggVertexStore3f(0.0, sz, sz);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(4, v);

	v = eggVertexStore3f(sz, sz, sz);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(5, v);

	v = eggVertexStore3f(sz, 0.0, sz);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(6, v);

	v = eggVertexStore3f(0.0, 0.0, sz);
	eggVertexNormalStore3f(v, 0.0, 1.0, 0.0);
	vertex.Add(7, v);

	eggGroupCenter3f(sz  / 2.0, sz / 2.0, sz / 2.0);

	eggEnd(); //FREYJA_GROUP 


	// Mongoose 2002.03.02, Allocate texels
	t = eggTexCoordStore2f(0.0, 1.0);
	texel.Add(0, t);

	t = eggTexCoordStore2f(0.33, 1.0);
	texel.Add(1, t);

	t = eggTexCoordStore2f(0.66, 1.0);
	texel.Add(2, t);

	t = eggTexCoordStore2f(0.0, 0.66);
	texel.Add(3, t);

	t = eggTexCoordStore2f(0.33, 0.66);
	texel.Add(4, t);

	t = eggTexCoordStore2f(0.66, 0.66);
	texel.Add(5, t);

	t = eggTexCoordStore2f(0.0, 0.33);
	texel.Add(6, t);

	t = eggTexCoordStore2f(0.33, 0.33);
	texel.Add(7, t);

	t = eggTexCoordStore2f(0.66, 0.33);
	texel.Add(8, t);

	t = eggTexCoordStore2f(0.33, 0.0);
	texel.Add(9, t);

	t = eggTexCoordStore2f(0.66, 0.0);
	texel.Add(10, t);

	t = eggTexCoordStore2f(1.0, 0.66);
	texel.Add(11, t);

	t = eggTexCoordStore2f(1.0, 0.33);
	texel.Add(12, t);

	// Allocate quad faces using triangles...

	// 1: 0 1 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggTexCoord1i(texel[0]);
	eggVertex1i(vertex[1]);
	eggTexCoord1i(texel[1]);
	eggVertex1i(vertex[3]);
	eggTexCoord1i(texel[3]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 1: 2 1 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[2]);
	eggTexCoord1i(texel[4]);
	eggVertex1i(vertex[1]);
	eggTexCoord1i(texel[1]);
	eggVertex1i(vertex[3]);
	eggTexCoord1i(texel[3]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 2 : 4 5 7
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[4]);
	eggTexCoord1i(texel[1]);
	eggVertex1i(vertex[5]);
	eggTexCoord1i(texel[2]);
	eggVertex1i(vertex[7]);
	eggTexCoord1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 2 : 6 5 7
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggTexCoord1i(texel[5]);
	eggVertex1i(vertex[5]);
	eggTexCoord1i(texel[2]);
	eggVertex1i(vertex[7]);
	eggTexCoord1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 3 : 0 4 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggTexCoord1i(texel[3]);
	eggVertex1i(vertex[4]);
	eggTexCoord1i(texel[6]);
	eggVertex1i(vertex[3]);
	eggTexCoord1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 3 : 7 4 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[4]);
	eggVertex1i(vertex[3]);
	eggTexCoord1i(texel[7]);
	eggTexCoord1i(texel[6]);
	eggTexCoord1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 4 : 0 1 4
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[4]);
	eggTexCoord1i(texel[4]);
	eggTexCoord1i(texel[7]);
	eggTexCoord1i(texel[5]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 4 : 5 1 4
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[5]);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[4]);
	eggTexCoord1i(texel[8]);
	eggTexCoord1i(texel[7]);
	eggTexCoord1i(texel[5]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 5 : 3 7 2
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[3]);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[2]);
	eggTexCoord1i(texel[5]);
	eggTexCoord1i(texel[8]);
	eggTexCoord1i(texel[11]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 5 : 6 7 2
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[2]);
	eggTexCoord1i(texel[12]);
	eggTexCoord1i(texel[8]);
	eggTexCoord1i(texel[11]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON
	
	// 6 : 1 2 5 
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[2]);
	eggVertex1i(vertex[5]);
	eggTexCoord1i(texel[7]);
	eggTexCoord1i(texel[9]);
	eggTexCoord1i(texel[8]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 6 : 6 2 5
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggVertex1i(vertex[2]);
	eggVertex1i(vertex[5]);
	eggTexCoord1i(texel[10]);
	eggTexCoord1i(texel[9]);
	eggTexCoord1i(texel[8]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	eggEnd(); // FREYJA_MESH
}



void eggGenerateCylinder(unsigned int major, unsigned int minor, 
						 vec_t height, vec_t radius)
{
	Vector<unsigned int> vertices;
	unsigned int i, j, k, v;
	double majorStep = height / major;
	double minorStep = 2.0 * HEL_PI / minor;


	eggBegin(FREYJA_MESH);

	for (i = 0; i < major; ++i) 
	{
		float z0 = 0.5 * height - i * majorStep;
		float z1 = z0 - majorStep;


		eggBegin(FREYJA_GROUP);

		// start a new tri strip 
		for (j = 0; j <= minor; ++j) 
		{      
			double a = j * minorStep;
			float x = radius * cos(a);
			float y = radius * sin(a);

			v = eggVertexStore3f(x, y, z0);
			eggVertexUVStore2f(v, j / (vec_t)minor, i / (vec_t)major);
			eggVertexNormalStore3f(v, x / radius, y / radius, 0.0f);
			vertices.pushBack(v);

			v = eggVertexStore3f(x, y, z1);
			eggVertexUVStore2f(v, j / (vec_t)minor, (i + 1) / (vec_t)major);
			eggVertexNormalStore3f(v, x / radius, y / radius, 0.0f);
			vertices.pushBack(v);
		}

		eggEnd(); // FREYJA_GROUP

		/* Generate mesh faces from tristrip data */
		for (k = (j*2), j = 2; j < k; ++j)
		{
			eggBegin(FREYJA_POLYGON);
			eggVertex1i(vertices[j-2]);
			eggVertex1i(vertices[j-1]);
			eggVertex1i(vertices[j]);
			eggTexture1i(0);
			eggEnd(); // FREYJA_POLYGON
		}

		vertices.clear();
	}

	eggEnd(); // FREYJA_MESH
}


void eggGenerateTriangleStrip(unsigned int count, vec_t sz)
{
	Vector<unsigned int> vertices;
	vec3_t xyz;
	vec2_t uv;
	vec_t x, y;
	unsigned int i, v;


	eggBegin(FREYJA_MESH);
	eggBegin(FREYJA_GROUP);

	// Allocate vertices and texels
	for (i = 0, x = y = 0.0f; i < count; ++i)
	{		
		v = eggVertexStore3f(x*sz, y*sz, 0.0f);
		eggVertexNormalStore3f(v, 0.0f, 0.0f, 1.0f);
		vertices.pushBack(v);

		// Mongoose 2002.01.18, Generate tri strip vertices in a clever way
		if (i % 2)
		{
			++x;
			--y;
		}
		else
		{
			++y;
		}

		// Mongoose 2002.01.18, Generate UV from vertex XY
		xyz[0] = x;
		xyz[1] = y;
		xyz[2] = 0;
		
		eggGenerateUVFromXYZ(xyz, uv+0, uv+1);
		eggVertexUVStore2f(v, uv[0], uv[1]);
	}

	eggEnd(); // FREYJA_GROUP

	// Generate mesh faces
	for (i = 2; i < count; ++i)
	{
		eggBegin(FREYJA_POLYGON);
		eggVertex1i(vertices[i-2]);
		eggVertex1i(vertices[i-1]);
		eggVertex1i(vertices[i]);
		eggTexture1i(0);
		eggEnd(); // FREYJA_POLYGON
	}

	eggEnd(); // FREYJA_MESH
}


void eggGenerateSphere(unsigned int major, unsigned int minor, vec_t radius)
{
	Vector<unsigned int> vertices;
	unsigned int i, j, k, v;
	vec_t x, y;
	double majorStep = (HEL_PI / major);
	double minorStep = (2.0 * HEL_PI / minor);
	vec_t nx, ny, nz, ns;


	eggBegin(FREYJA_MESH);

	for (i = 0; i < major; ++i) 
	{
		double a = i * majorStep;
		double b = a + majorStep;
		double r0 = radius * sin(a);
		double r1 = radius * sin(b);
		vec_t z0 = radius * cos(a);
		vec_t z1 = radius * cos(b);


		eggBegin(FREYJA_GROUP);

		/* Start a new tri strip */
		for (j = 0; j <= minor; ++j) 
		{
			double c = j * minorStep;

			x = cos(c);
			y = sin(c);

			v = eggVertexStore3f(x * r0, y * r0, z0);
			eggVertexUVStore2f(v, j / (vec_t)minor, i / (vec_t)major);
			nx = (x * r0) / radius;
			ny = (y * r0) / radius;
			nz = z0 / radius;
			ns = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;
			// printf("1: %f %f %f\n", nx, ny, nz);
			eggVertexNormalStore3f(v, nx, ny, nz);
			vertices.pushBack(v);


			v = eggVertexStore3f(x * r1, y * r1, z1);
			eggVertexUVStore2f(v, j / (vec_t)minor, (i + 1) / (vec_t)major);
			nx = (x * r1) / radius;
			ny = (y * r1) / radius;
			nz = z0 / radius;
			ns = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;
			// printf("2: %f %f %f\n", nx, ny, nz);
			eggVertexNormalStore3f(v, nx, ny, nz);
			vertices.pushBack(v);
		}

		//eggGroupCenter3f(0.0f, 0.0f, 0.0f);
		eggEnd(); // FREYJA_GROUP 


		// Generate mesh faces from tristrip data
		for (k = (j*2), j = 2; j < k; ++j)
		{
			eggBegin(FREYJA_POLYGON);
			eggVertex1i(vertices[j-2]);
			eggVertex1i(vertices[j-1]);
			eggVertex1i(vertices[j]);
			eggTexture1i(0);
			eggEnd(); // FREYJA_POLYGON
		}

		vertices.clear();
	}

	eggEnd(); // FREYJA_MESH
}


Vector<unsigned int> *eggFindVerticesByBox(bbox_t bbox)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	eggCriticalSection(EGG_WRITE_LOCK);

	count = eggGetNum(FREYJA_VERTEX); 

	if (count == PLUGIN_ERROR)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	eggIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		eggGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(eggIterator(FREYJA_VERTEX, 
											   FREYJA_LIST_CURRENT));
				}
			}
		}

		eggIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	eggCriticalSection(EGG_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(bbox_t bbox,
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	eggCriticalSection(EGG_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (PLUGIN_ERROR == eggIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		eggGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(eggIterator(FREYJA_VERTEX, 
											   FREYJA_LIST_CURRENT));
				}
			}
		}
	}

	eggCriticalSection(EGG_WRITE_UNLOCK);
	
	return list;
}


void eggGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, v0, v1, v2, vertexCount, faceCount;


	eggCriticalSection(EGG_WRITE_LOCK);
	
	vertexCount = eggGetNum(FREYJA_VERTEX);
	faceCount = eggGetNum(FREYJA_POLYGON); 

	eggIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		eggGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		eggGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		eggGetPolygon1u(FREYJA_VERTEX, 2, &v2);

		printf("<%d %d %d>\n", v0, v1, v2);
		eggIterator(FREYJA_VERTEX, v0);
		eggGetVertex3f(a.mVec);
		eggIterator(FREYJA_VERTEX, v1);
		eggGetVertex3f(b.mVec);
		eggIterator(FREYJA_VERTEX, v2);
		eggGetVertex3f(c.mVec);


		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(b - a, b - c);
		faceNormals.pushBack(new Vector3d(normal));

		eggIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	eggIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		egg_vertex_t *vertex = EggPlugin::mEggPlugin->eggGetVertex(eggIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT));
		eggIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);

		if (!vertex)
		{
			printf("ERROR\n");
			continue;
		}

		normal.zero();

		printf("%d :: %d faces\n", vertex->id, vertex->ref.size());
		for (j = vertex->ref.begin(); j < vertex->ref.end(); ++j)
		{
			normal += *faceNormals[j];
		}

		normal.normalize();

		vertex->norm[0] = normal.mVec[0];
		vertex->norm[1] = normal.mVec[1];
		vertex->norm[2] = normal.mVec[2];

		printf("%d :: %f %f %f\n", vertex->id, 
			   normal.mVec[0],
			   normal.mVec[1],
			   normal.mVec[2]);
    }

	faceNormals.erase();
	
	eggCriticalSection(EGG_WRITE_UNLOCK);
}


////////////////////////////////////////////////////////////////////


unsigned int eggBegin(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggBegin(type);

	return PLUGIN_ERROR;
}


unsigned int eggEnd()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggEnd();

	return PLUGIN_ERROR;
}


int eggTextureStore(EggTextureData *textureData)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStore(textureData);

	return -1;
}


unsigned int eggTexCoordStore2f(float s, float t)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTexCoordStore2f(s, t);

	return PLUGIN_ERROR;
}


unsigned int eggVertexStore3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggVertexStore3f(x, y, z);

	return PLUGIN_ERROR;
}


void eggVertexWeightStore(unsigned int index, 
						  vec_t weight, unsigned int bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexWeightStore(index, weight, bone);
}


void eggVertexUVStore2f(unsigned int vIndex, float u, float v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexUVStore2f(vIndex, u, v);
}


void eggVertexNormalStore3f(unsigned int vIndex, float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexNormalStore3f(vIndex, x, y, z);
}


void eggVertex1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertex1i(egg_id);
}


void eggTexCoord1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexCoord1i(egg_id);
}


void eggTexture1i(int id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexture1i(id);  
}


unsigned int eggFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggFlags();

	return PLUGIN_ERROR;
}


unsigned int eggGroupCenter3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGroupCenter(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagName(char *name)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagName(name);

	return PLUGIN_ERROR;  
}

unsigned int eggTagPos3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagPos(x, y, z);

	return PLUGIN_ERROR;  
}


unsigned int eggTagFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagFlags(flags);

	return PLUGIN_ERROR;
}


unsigned int eggTagAddMesh1u(unsigned int mesh)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddMesh(mesh);

	return PLUGIN_ERROR;
}


unsigned int eggTagAddSlave1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddSlave(tag);

	return PLUGIN_ERROR;
}


unsigned int eggMeshTreeAddFrame3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeAddFrame(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagRotate3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagRotate(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagRotateQuaternion4f(vec_t w, vec_t x, vec_t y, vec_t z)
{
	vec_t heading, bank, attitude;
	Quaternion q = Quaternion(w, x, y, z);

	q.getEulerAngles(&heading, &bank, &attitude);

	// For now just switch to eular ( ew ) and do I have this right?
	if (EggPlugin::mEggPlugin) // h b a; a b h
		return EggPlugin::mEggPlugin->eggTagRotate(attitude*57.295779513082323,
												   bank*57.295779513082323, 
												   heading*57.295779513082323);

	return PLUGIN_ERROR;
}


unsigned int eggMeshTreeTag1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeTag(tag);

	return PLUGIN_ERROR;
}

int eggGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;

	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTextureFilename(index, 
															filename);
	return PLUGIN_ERROR;	
}

int eggGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTextureImage(index, w, h, 
														 depth, type, image);
	return PLUGIN_ERROR;
}


unsigned int eggTextureStoreFilename(char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreFilename(filename);

	return PLUGIN_ERROR;
}

unsigned int eggTextureStoreBuffer(unsigned char *image, unsigned int depth,
								   unsigned int width, unsigned int height,
								   egg_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreBuffer(image, depth,
															width, height,
															type);

	return PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

unsigned int eggGetNum(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetNum(type);

	return PLUGIN_ERROR;
}


unsigned int eggIterator(egg_plugin_t type, int item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggIterator(type, item);

	return PLUGIN_ERROR;
}


unsigned int eggGetVertex3f(float *xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetVertex(xyz);

	return PLUGIN_ERROR;
}


unsigned int eggGetPolygon1u(egg_plugin_t type, int item, 
							 unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetPolygon(type, item, value);

	return PLUGIN_ERROR;
}


unsigned int eggGetPolygon3f(egg_plugin_t type, int item, float *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetPolygon(type, item, value);

	return PLUGIN_ERROR;
}

unsigned int eggCriticalSection(egg_lock_t request)
{
	printf("eggCriticalSection> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return PLUGIN_ERROR;
}


unsigned int eggGetTagMesh1u(unsigned int item, unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagMesh(item, value);

	return PLUGIN_ERROR;
}


unsigned int eggGetCurrent(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetCurrent(type);

	return PLUGIN_ERROR;
}


unsigned int eggGetTagMeshNum()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagMeshNum();

	return PLUGIN_ERROR;
}


unsigned int eggGetTagRotate3f(float *x, float *y, float *z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagRotate(x, y, z);

	return PLUGIN_ERROR;
}

void eggMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggMeshFlags1u(flags);	
}

////////////////////////////////////////////////////////////


EggPlugin::EggPlugin(Egg *egg, char *plugin_dir)
{
	mFlags = 0;

	mEgg = egg;
	mTag = 0x0;
	mMesh = 0x0;
	mGroup = 0x0;
	mAnimation = 0x0;
	mBoneFrame = 0x0;
  
	mTextureId = 0;

	if (plugin_dir && plugin_dir[0])
	{
		long len = strlen(plugin_dir);

		mPluginDir = new char[len + 2];
		strncpy(mPluginDir, plugin_dir, len);
		mPluginDir[len+1] = 0;
	}
	else
	{
		mPluginDir = new char[64];
		strcpy(mPluginDir, "/usr/local/lib/egg/");    
	}


	/* Mongoose 2004.05.18, 
	 * Add dir search check and sort import / export / etc */

	/* Load some default plugins they should have */
	addModule("eggv7");
	addModule("ase");
	addModule("3ds");
	addModule("tombraider");
	addModule("mdl");
	addModule("md2");
	addModule("md3");
	addModule("halflife");
	addModule("trmesh");
	addModule("lwo");
	addModule("nod");
	
	EggPlugin::mEggPlugin = this;
}


EggPlugin::~EggPlugin()
{
	mTextures.erase();
	mTextureFiles.erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned int EggPlugin::eggGetNum(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		return mEgg->getVertexCount();
		break;
	case FREYJA_TEXCOORD:
		return mEgg->getTexelCount();
		break;
	case FREYJA_MESH:
		return mEgg->getMeshCount();
		break;
	case FREYJA_GROUP:
		return mEgg->getGroupCount();
		break;
	case FREYJA_POLYGON:
		return mEgg->getPolygonCount();
		break;
	case FREYJA_BONE:
		return mEgg->getTagCount();
		break;
	case FREYJA_SKELETON:
		return mEgg->getBoneFrameCount();
		break;
	case FREYJA_SKEL_ANIM:
		return mEgg->getAnimationCount();
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggIterator(egg_plugin_t type, int item)
{
	Vector<egg_vertex_t *> *vertex;
	Vector<egg_texel_t *> *texel;
	Vector<egg_mesh_t *> *mesh;
	Vector<egg_group_t *> *group;
	Vector<egg_polygon_t *> *polygon;
	Vector<egg_tag_t *> *tag;
	Vector<egg_boneframe_t *> *boneframe;
	Vector<egg_animation_t *> *skelanim;


	switch (type)
	{
	case FREYJA_VERTEX:
		vertex = mEgg->VertexList();
    
		if (!vertex)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertex = vertex->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexVertex;
			break;
		default:
			mIndexVertex = item;
		}

		if (mIndexVertex < vertex->end())
		{
			return mIndexVertex;
		}
		break;
	case FREYJA_TEXCOORD:
		texel = mEgg->TexelList();
    
		if (!texel)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexTexCoord = texel->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexTexCoord;
			break;
		default:
			mIndexTexCoord = item;
		}

		if (mIndexTexCoord < texel->end())
		{
			return mIndexTexCoord;
		}
		break;
	case FREYJA_MESH:
		mesh = mEgg->MeshList();
    
		if (!mesh)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexMesh = mesh->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexMesh;
			break;
		default:
			mIndexMesh = item;
		}

		if (mIndexMesh < mesh->end())
		{
			return mIndexMesh;
		}
		break;
	case FREYJA_GROUP:
		group = mEgg->GroupList();
    
		if (!group)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexGroup = group->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexGroup;
			break;
		default:
			mIndexGroup = item;
		}

		if (mIndexGroup < group->end())
		{
			return mIndexGroup;
		}
		break;
	case FREYJA_POLYGON:
		polygon = mEgg->PolygonList();
    
		if (!polygon)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexPolygon = polygon->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexPolygon;
			break;
		default:
			mIndexPolygon = item;
		}

		if (mIndexPolygon < polygon->end())
		{
			return mIndexPolygon;
		}
		break;
	case FREYJA_BONE:
		tag = mEgg->TagList();
    
		if (!tag)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexBone = tag->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexBone;
			break;
		default:
			mIndexBone = item;
		}

		if (mIndexBone < tag->end())
		{
			return mIndexBone;
		}
		break;
	case FREYJA_SKELETON:
		boneframe = mEgg->BoneFrameList();
    
		if (!boneframe)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeleton = boneframe->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexSkeleton;
			break;
		default:
			mIndexSkeleton = item;
		}

		if (mIndexSkeleton < boneframe->end())
		{
			return mIndexSkeleton;
		}
		break;
	case FREYJA_SKEL_ANIM:
		skelanim = mEgg->AnimationList();
    
		if (!skelanim)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeletonAnim = skelanim->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexSkeletonAnim;
			break;
		default:
			mIndexSkeletonAnim = item;
		}

		if (mIndexSkeletonAnim < skelanim->end())
		{
			return mIndexSkeletonAnim;
		}
		break;
		//	case FREYJA_MESHTREE_ANIM:
		//		printf("EggPlugin::eggIterator> %s Not implemented, %s:%i\n", 
		//			   "FREYJAMANIMATION_ANIM", __FILE__, __LINE__);
		//		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetVertex(float *xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return PLUGIN_ERROR;

	xyz[0] = vert->pos[0];
	xyz[1] = vert->pos[1];
	xyz[2] = vert->pos[2];

	return vert->id;
}


unsigned int EggPlugin::eggGetPolygon(egg_plugin_t type, int item, 
									  unsigned int *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return PLUGIN_ERROR;

		*value = vert->id;
		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return PLUGIN_ERROR;

		*value = tex->id;
		return tex->id;
		break;
	default:
		return PLUGIN_ERROR;
	}

	return PLUGIN_ERROR;
}


egg_vertex_t *EggPlugin::eggGetVertex(unsigned int index)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *v;
	
	vertex = mEgg->VertexList();

	if (!vertex || vertex->empty() || index > vertex->size()-1)
		return 0x0;

	v = (*vertex)[index];

	return v;
}


unsigned int EggPlugin::eggGetPolygon(egg_plugin_t type, int item, 
									  float *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return PLUGIN_ERROR;

		value[0] = vert->pos[0];
		value[1] = vert->pos[1];
		value[2] = vert->pos[2];

		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return PLUGIN_ERROR;

		value[0] = tex->st[0];
		value[1] = tex->st[1];
    
		return tex->id;
		break;
	default:
		return PLUGIN_ERROR;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagMesh(unsigned int item, unsigned int *value)
{
	if (mTag)
	{
		*value = mTag->mesh[item];
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagMeshNum()
{
	if (mTag)
	{
		return mTag->mesh.size();
	}

	return PLUGIN_ERROR;
}


// FIXME: Not implmented fully
unsigned int EggPlugin::eggGetCurrent(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		break;
	case FREYJA_TEXCOORD:
		break;
	case FREYJA_MESH:
		if (mMesh)
			return mMesh->id;
		break;
	case FREYJA_GROUP:
		if (mGroup)
			return mGroup->id;
		break;
	case FREYJA_POLYGON:
		break;
	case FREYJA_BONE:
		if (mTag)
			return mTag->id;
		break;
	case FREYJA_SKELETON:
		if (mBoneFrame)
			return mBoneFrame->id;
		break;
	case FREYJA_SKEL_ANIM:
		if (mAnimation)
			return mAnimation->id;
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagRotate(float *x, float *y, float *z)
{
	if (mTag)
	{
		*x = mTag->rot[0];
		*y = mTag->rot[1];
		*z = mTag->rot[2];
   
		return 0;
	}

	return PLUGIN_ERROR;
}

	
void EggPlugin::eggPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	eggPrintError(format, &args);
	va_end(args);
}


void EggPlugin::eggPrintError(char *format, va_list *args)
{
	vfprintf(stderr, format, *args);
}


void EggPlugin::eggPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	eggPrintMessage(format, &args);
	va_end(args);
}


void EggPlugin::eggPrintMessage(char *format, va_list *args)
{
	vfprintf(stdout, format, *args);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int EggPlugin::importModel(char *filename)
{
#ifdef MODEL_PLUGINS
	bool loaded = false;
	char module_filename[128];
	char module_check[128];
	char module_import[128];
	char *name;
	int (*import)(char *filename);
	int (*check)(char *filename);
	void *handle;
	char *error;
	unsigned int i;


	if (!mEgg->checkFile(filename))
	{
		if (!mEgg->loadFile(filename))
			return 0;
		else
			return -2;
	}

	eggPrintMessage("[EggPlugin module loader invoked]\n");

	for (i = mModules.begin(); i < mModules.end(); ++i)
	{
		name = mModules[i];

		sprintf(module_filename, "%slibfreyja_loader-%s.so", mPluginDir, name);
		sprintf(module_import, "freyja_model__%s_import", name);
		sprintf(module_check, "freyja_model__%s_check", name);

		if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
		{
			eggPrintError("\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
			}

			//return -1;
			continue;
		}
		else
		{
			eggPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			check = (int (*)(char *filename))dlsym(handle, module_check);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
				dlclose(handle);
				continue;
			}

			import = (int (*)(char * filename))dlsym(handle, module_import);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
				dlclose(handle);
				continue;
			}

			loaded = (loaded || (!(*check)(filename) && !(*import)(filename)));

			if ((error = dlerror()) != NULL) 
			{
				//return -10;
				dlclose(handle);
				continue;
			}

			dlclose(handle);
		}
	}

	eggPrintMessage("[EggPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#else
	if (!mEgg->Check(filename))
	{
		if (!mEgg->Load(filename))
			return 0;
		else
			return -2;
	}
#endif
	return -1;
}


int EggPlugin::exportModel(char *filename, char *type)
{
#ifdef MODEL_PLUGINS
	bool saved = false;
	char module_filename[128];
	char module_export[128];
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	char *error;


	if (!type || !filename)
		return -100;

	if (strcmp(type, "egg") == 0)
		return mEgg->saveFile(filename);

	printf("[EggPlugin module loader invoked]\n");

	name = type;

	sprintf(module_filename, "%slibfreyja_loader-%s.so", mPluginDir, name);
	sprintf(module_export, "freyja_model__%s_export", name);

	if (!(handle = dlopen(module_filename, RTLD_NOW)))
	{
		fprintf(stderr, "\tERROR: In module '%s'.\n", module_filename);

		if ((error = dlerror()) != NULL)  
		{
			fprintf (stderr, "\tERROR: %s\n", error);
		}
	}
	else
	{
		printf("\tModule '%s' opened.\n", module_filename);
    
		export_mdl = (int (*)(char * filename))dlsym(handle, module_export);

		if ((error = dlerror()) != NULL)  
		{
			fprintf (stderr, "\tERROR: %s\n", error);
			dlclose(handle);
		}

		saved = (!(*export_mdl)(filename));

		if ((error = dlerror()) != NULL) 
		{
			dlclose(handle);
		}

		dlclose(handle);
	}

	printf("[EggPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // sucess
#else
	if (!type || !filename)
		return -100;

	if (strcmp(type, "egg") == 0)
		return mEgg->saveFile(filename);
#endif
	return -1;
}

unsigned int EggPlugin::eggBegin(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_MESH:
		mStack.push(FREYJA_MESH);
		mMesh = mEgg->newMesh();
		mEgg->addMesh(mMesh);
		break;
	case FREYJA_GROUP:
		mStack.push(FREYJA_GROUP);

		if (!mMesh)
		{
			fprintf(stderr, "EggPlugin::eggBegin> GROUP defined outside MESH.");
		}
		else
		{
			mGroup = mEgg->newGroup();
			mEgg->addGroup(mGroup);
			mMesh->group.pushBack(mGroup->id); 
		}
		break;
	case FREYJA_POLYGON:
		mStack.push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexCoordList.clear();

		if (!mMesh)
		{
			fprintf(stderr, "EggPlugin::eggEnd> Polygon defined outside MESH!");
		}
		break;
	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mTag = mEgg->addTag(0.0, 0.0, 0.0, 0x00);
		mTag->name[0] = 0;
		break;
	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
		break;
	case FREYJA_SKEL_ANIM:
		mStack.push(FREYJA_SKEL_ANIM);
		mAnimation = new egg_animation_t;
		mEgg->addAnimation(mAnimation);
	default:
		;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggEnd()
{
	unsigned int polygon;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexCoordList, mTextureId);

		if (polygon == UINT_MAX)
		{
			printf("EggPlugin::eggEnd> Polygon is invalid\n");
			return PLUGIN_ERROR;
		}

		if (mMesh)
		{
			mMesh->polygon.pushBack(polygon);
			mMesh->r_polygon.pushBack(mEgg->getPolygon(polygon));
		}
		else
		{
			fprintf(stderr, "EggPlugin::eggEnd> Polygon defined outside MESH!");
		}
		break;
	default:
		;
	}

	return PLUGIN_ERROR;
}


int EggPlugin::eggTextureStore(EggTextureData *textureData)
{
	if (textureData)
	{
		mTextures.pushBack(textureData);
		return mTextures.size() - 1;
	}

	return -1;
}


unsigned int EggPlugin::eggTextureStoreFilename(const char *filename)
{
	unsigned int i;


	if (!filename || !filename[0])
	{
		return 0;
	}

	if (!mTextureFiles.empty())
	{
		for (i = mTextureFiles.begin(); i < mTextureFiles.end(); ++i)
		{
			if (!strcmp(mTextureFiles[i], filename))
				return i;
		}
	}

	mTextureFiles.pushBack(strdup(filename));
	return mTextureFiles.size() - 1;
}

int EggPlugin::eggGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;


	if (mTextureFiles.empty() || index > mTextureFiles.size()-1)
		return -1;

	*filename = mTextureFiles[index];

	return 0;
}


int EggPlugin::eggGetTextureImage(unsigned int index, 
								  unsigned int *w, unsigned int *h, 
								  unsigned int *depth,
								  unsigned int *type,
								  unsigned char **image)
{
	EggTextureData *t;


	if (mTextures.empty() || index > mTextures.size()-1)
		return -1;

	t = mTextures[index];

	// Init
	*image = 0x0;
	*depth = 0x0;
	*type = 0x0;
	*w = 0x0;
	*h = 0x0;

	if (!t)
		return -1;

	if (t->mImage && t->mWidth && t->mHeight && t->mBitDepth)
	{
		*image = t->mImage;
		*depth = t->mBitDepth;
		*type = t->mType;
		*w = t->mWidth;
		*h = t->mHeight;

		return 0;
	}
  
	return -1;
}


unsigned int EggPlugin::eggTexCoordStore2f(float s, float t)
{
	return mEgg->addTexel(s, t);
}


unsigned int EggPlugin::eggVertexStore3f(float x, float y, float z)
{
	egg_vertex_t *vert;


	vert = mEgg->addVertex(x, y, z);

	if (vert)
	{
		if (mGroup)
		{
			mGroup->vertex.pushBack(vert->id);
		}
		else
		{
			fprintf(stderr, 
					"EggPlugin::eggVertexStore3f> Vertex outside GROUP!");
		}

		return vert->id;
	}
	else
		return PLUGIN_ERROR;
}


void EggPlugin::eggVertexWeightStore(unsigned int index, 
									 vec_t weight, unsigned int bone)
{
	egg_vertex_t *vert = mEgg->getVertex(index);
	egg_weight_t *vWeight;
	int emptySlot = -1;
	vec_t total = 0.0f;
	unsigned int i;


	if (!vert)
		return;

	for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
	{
		vWeight = vert->weights[i];

		if (vWeight)
		{
			if (weight <= 0.0) // Remove weight
			{
				delete vWeight;
				vert->weights.assign(i, 0x0);
				return;
			}

			if (vWeight->bone == bone) // Alter weight
			{
				vWeight->weight = weight;
				return;
			}

			total = vWeight->weight;
		}
		else
		{
			emptySlot = i;
		}
	}

	if (weight <= 0.0) // Don't add dead weight ( remove requested )
		return;

	if (total + weight > 1.0)  // Just give a warning for now
		eggPrintError("WARNING: Weight overflow not handled here %s:%d\n", 
					  __FILE__, __LINE__);

	vWeight = new egg_weight_t;
	vWeight->weight = weight;
	vWeight->bone = bone;

	if (emptySlot > -1)
	{
		vert->weights.assign(emptySlot, vWeight);
	}
	else
	{
		vert->weights.pushBack(vWeight);
	}

	return;
}


void EggPlugin::eggVertexUVStore2f(unsigned int vIndex, 
								   float u, float v)
{
	egg_vertex_t *vert;

  
	vert = mEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->uv[0] = u;
	vert->uv[1] = v;
}


void EggPlugin::eggVertexNormalStore3f(unsigned int vIndex, 
									   float x, float y, float z)
{
	egg_vertex_t *vert;

  
  
	vert = mEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->norm[0] = x;
	vert->norm[1] = y;
	vert->norm[2] = z;
}


void EggPlugin::eggVertex1i(unsigned int egg_id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(egg_id);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void EggPlugin::eggMeshFlags1u(unsigned int flags)
{
	if (mStack.peek() == FREYJA_MESH || !mMesh)
	{
		mMesh->flags = flags;
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggMeshFlags1u> Flag defined outside MESH!\n");
	}
}


void EggPlugin::eggTexCoord1i(unsigned int egg_id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTexCoordList.pushBack(egg_id);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void EggPlugin::eggTexture1i(int id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTextureId = id;
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTexture1i> Texture defined outside POLYGON!\n");
	}
}


unsigned int EggPlugin::eggFlags()
{
	return mFlags;
}


void EggPlugin::eggFlags(unsigned int set)
{
	mFlags = set;
}


unsigned int EggPlugin::eggGroupCenter(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_GROUP)
	{
		if (!mGroup)
		{
			fprintf(stderr, 
					"EggPlugin::eggGroupCenter> GROUP isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mGroup->center[0] = x;
			mGroup->center[1] = y;
			mGroup->center[2] = z;
		}
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggGroupCenter> Center defined outside GROUP!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagName(char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagPos> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			strncpy(mTag->name, name, 64);
			mTag->name[63] = 0;
		}
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagPos> Pos defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagPos(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagPos> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mTag->center[0] = x;
			mTag->center[1] = y;
			mTag->center[2] = z;
		}
	}
	else if (mTag) // HACK
	{
		mTag->center[0] = x;
		mTag->center[1] = y;
		mTag->center[2] = z;
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagPos> Pos defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagFlags> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mTag->flag = flags;
		}
	}
	else if (mTag) // HACK
	{
		mTag->flag = flags;
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagFlags> Flag defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagAddMesh(unsigned int mesh)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagAddMesh> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mTag->mesh.pushBack(mesh);
		}
	}
	else if (mTag) // HACK
	{
		mTag->mesh.pushBack(mesh);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagAddMesh> Mesh defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagAddSlave(unsigned int tag)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagAddSlave> BONEMTAG isn't allocated!\n");
      
			return PLUGIN_ERROR;
		}
		else 
		{
			mTag->slave.pushBack(tag);
		}
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagAddSlave> Slave defined outside BONEMTAG!\n");

		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggMeshTreeAddFrame(float x, float y, float z)
{
	if (mAnimation)
	{
		mBoneFrame = mEgg->BoneFrame(mEgg->BoneFrameAdd(x, y, z));    
		mAnimation->frame.pushBack(mBoneFrame->id);
   
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggTagRotate(float x, float y, float z)
{
	if (mTag)
	{
		mTag->rot[0] = x;
		mTag->rot[1] = y;
		mTag->rot[2] = z;
   
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggMeshTreeTag(unsigned int tag)
{
	if (mBoneFrame)
	{
		mBoneFrame->tag.pushBack(tag);
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggTextureStoreBuffer(unsigned char *image, 
											  unsigned int depth,
											  unsigned int width, 
											  unsigned int height,
											  egg_colormode_t type)
{
	EggTextureData *t;
	unsigned int size;


	if (!image || !depth || !width || !height)
		return PLUGIN_ERROR;

	size = width*height*depth;

	t = new EggTextureData();
 
	t->mWidth = width;
	t->mHeight = height;
	t->mType = type;
	t->mBitDepth = depth * 8;
	t->mImage = new unsigned char[size];
  
	memcpy(t->mImage, image, size);

	return eggTextureStore(t);
}


void EggPlugin::addModule(char *name)
{
	if (name && name[0])
		mModules.pushBack(name);
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

#ifdef UNIT_TEST_EGGPLUGIN

int runEggPluginUnitTest(int argc, char *argv[])
{
	EggPlugin ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[EggPlugin class test]\n");

	return runEggPluginUnitTest(argc, argv); 
  
	return 0;
}

#endif
