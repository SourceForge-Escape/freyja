/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
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
 ==============================================================*/

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "EggPlugin.h"


EggPlugin *EggPlugin::mEggPlugin = 0x0;


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
	t = eggTexelStore2f(0.0, 1.0);
	texel.Add(0, t);

	t = eggTexelStore2f(0.33, 1.0);
	texel.Add(1, t);

	t = eggTexelStore2f(0.66, 1.0);
	texel.Add(2, t);

	t = eggTexelStore2f(0.0, 0.66);
	texel.Add(3, t);

	t = eggTexelStore2f(0.33, 0.66);
	texel.Add(4, t);

	t = eggTexelStore2f(0.66, 0.66);
	texel.Add(5, t);

	t = eggTexelStore2f(0.0, 0.33);
	texel.Add(6, t);

	t = eggTexelStore2f(0.33, 0.33);
	texel.Add(7, t);

	t = eggTexelStore2f(0.66, 0.33);
	texel.Add(8, t);

	t = eggTexelStore2f(0.33, 0.0);
	texel.Add(9, t);

	t = eggTexelStore2f(0.66, 0.0);
	texel.Add(10, t);

	t = eggTexelStore2f(1.0, 0.66);
	texel.Add(11, t);

	t = eggTexelStore2f(1.0, 0.33);
	texel.Add(12, t);

	// Allocate quad faces using triangles...

	// 1: 0 1 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggTexel1i(texel[0]);
	eggVertex1i(vertex[1]);
	eggTexel1i(texel[1]);
	eggVertex1i(vertex[3]);
	eggTexel1i(texel[3]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 1: 2 1 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[2]);
	eggTexel1i(texel[4]);
	eggVertex1i(vertex[1]);
	eggTexel1i(texel[1]);
	eggVertex1i(vertex[3]);
	eggTexel1i(texel[3]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 2 : 4 5 7
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[4]);
	eggTexel1i(texel[1]);
	eggVertex1i(vertex[5]);
	eggTexel1i(texel[2]);
	eggVertex1i(vertex[7]);
	eggTexel1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 2 : 6 5 7
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggTexel1i(texel[5]);
	eggVertex1i(vertex[5]);
	eggTexel1i(texel[2]);
	eggVertex1i(vertex[7]);
	eggTexel1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 3 : 0 4 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggTexel1i(texel[3]);
	eggVertex1i(vertex[4]);
	eggTexel1i(texel[6]);
	eggVertex1i(vertex[3]);
	eggTexel1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 3 : 7 4 3
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[4]);
	eggVertex1i(vertex[3]);
	eggTexel1i(texel[7]);
	eggTexel1i(texel[6]);
	eggTexel1i(texel[4]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 4 : 0 1 4
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[0]);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[4]);
	eggTexel1i(texel[4]);
	eggTexel1i(texel[7]);
	eggTexel1i(texel[5]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 4 : 5 1 4
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[5]);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[4]);
	eggTexel1i(texel[8]);
	eggTexel1i(texel[7]);
	eggTexel1i(texel[5]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 5 : 3 7 2
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[3]);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[2]);
	eggTexel1i(texel[5]);
	eggTexel1i(texel[8]);
	eggTexel1i(texel[11]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 5 : 6 7 2
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggVertex1i(vertex[7]);
	eggVertex1i(vertex[2]);
	eggTexel1i(texel[12]);
	eggTexel1i(texel[8]);
	eggTexel1i(texel[11]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON
	
	// 6 : 1 2 5 
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[1]);
	eggVertex1i(vertex[2]);
	eggVertex1i(vertex[5]);
	eggTexel1i(texel[7]);
	eggTexel1i(texel[9]);
	eggTexel1i(texel[8]);
	eggTexture1i(0);
	eggEnd(); // FREYJA_POLYGON

	// 6 : 6 2 5
	eggBegin(FREYJA_POLYGON);
	eggVertex1i(vertex[6]);
	eggVertex1i(vertex[2]);
	eggVertex1i(vertex[5]);
	eggTexel1i(texel[10]);
	eggTexel1i(texel[9]);
	eggTexel1i(texel[8]);
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
			eggVertexNormalStore3f(v, 
								   (x * r0) / radius,
								   (y * r0) / radius, 
								   z0 / radius);
			vertices.pushBack(v);


			v = eggVertexStore3f(x * r1, y * r1, z1);
			eggVertexUVStore2f(v, j / (vec_t)minor, (i + 1) / (vec_t)major);
			eggVertexNormalStore3f(v, 
								   (x * r1) / radius,
								   (y * r1) / radius, 
								   z0 / radius);
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
#ifdef FIXME_MOVE_BACK_TO_EGG
	Vector <vec3_t *> normals, faceNormals;
	vec3_t *normal;
    vec3_t a, b, c, vA, vB;
	vec_t s;
	unsigned int i, v0, v1, v2, count;


	eggCriticalSection(EGG_WRITE_LOCK);
	
	vertexCount = eggGetNum(FREYJA_VERTEX);
	uvCount = eggGetNum(FREYJA_POLYGON) * 3;
	faceCount = eggGetNum(FREYJA_POLYGON); 

	eggIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		eggGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		eggGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		eggGetPolygon1u(FREYJA_VERTEX, 2, &v2);

		eggIterator(FREYJA_VERTEX, v0);
		eggGetVertex3f(a);
		eggIterator(FREYJA_VERTEX, v1);
		eggGetVertex3f(b);
		eggIterator(FREYJA_VERTEX, v2);
		eggGetVertex3f(c);

		/* Compute 2 vectors from the triangle face */		
		vA[0] = a[0] - b[0];
		vA[1] = a[1] - b[1];
		vA[2] = a[2] - b[2];

		vB[0] = b[0] - c[0];
		vB[1] = b[1] - c[1];
		vB[2] = b[2] - c[2];
		
		/* Compute normal for the face */
		normal = new vec3_t;
		normal[0] = vA[1] * vB[2] - vA[2] * vB[1];
		normal[1] = vA[2] * vB[0] - vA[0] * vB[2];
		normal[2] = vA[0] * vB[1] - vA[1] * vB[0];

		faceNormals.pushBack(normal);
	}


	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		egg_vertex_t *vertex = eggGetVertex(i);

		if (!vertex)
			continue;

		vertex->norm[0] = vertex->norm[1] = vertex->norm[2] = 0.0f;

		for (vertex->ref.start(); vertex->ref.forward(); vertex->ref.next())
		{
			normal = faceNormals[vertex->ref.currentIndex()];

			vertex->norm[0] += normal[0];
			vertex->norm[1] += normal[1];
			vertex->norm[2] += normal[2];
		}

		/* Average normals */
		normal = vertex->norm;
		normal[0] /= vertex->ref.size();
		normal[1] /= vertex->ref.size();
		normal[2] /= vertex->ref.size();

		/* Normalize */
		s = sqrt(normal[0] * normal[0] + 
				 normal[1] * normal[1] + 
				 normal[2] * normal[2]);

		normal[0] /= s;
		normal[1] /= s;
		normal[2] /= s;
    }

	faceNormals.erase();

	eggCriticalSection(EGG_WRITE_UNLOCK);
#endif
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


unsigned int eggTexelStore2f(float s, float t)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTexelStore2f(s, t);

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


void eggTexel1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexel1i(egg_id);
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


unsigned int eggTagRotateQuaternion4f(vec_t x, vec_t y, vec_t z, vec_t w)
{
	Quaternion q = Quaternion(w, x, y, z);
	vec_t heading, bank, attitude;
	
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
	addModule("quakemap");
	addModule("ut");
	addModule("user1");
	addModule("grn");
	
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
		return mEgg->getCount(Egg::VERTEX);
		break;
	case FREYJA_TEXEL:
		return mEgg->getCount(Egg::TEXCOORD);
		break;
	case FREYJA_MESH:
		return mEgg->getCount(Egg::MESH);
		break;
	case FREYJA_GROUP:
		return mEgg->getCount(Egg::GROUP);
		break;
	case FREYJA_POLYGON:
		return mEgg->getCount(Egg::POLYGON);
		break;
	case FREYJA_BONE_TAG:
		return mEgg->getCount(Egg::BONETAG);
		break;
	case FREYJA_BONE_FRAME:
		return mEgg->getCount(Egg::BONEFRAME);
		break;
	case FREYJA_MESHTREE_ANIM:
		return mEgg->getCount(Egg::ANIMATION);
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggIterator(egg_plugin_t type, int item)
{
	Vector<egg_vertex_t *> *vertex;
	Vector<egg_texcoord_t *> *texcoord;
	Vector<egg_mesh_t *> *mesh;
	Vector<egg_group_t *> *group;
	Vector<egg_polygon_t *> *polygon;
	Vector<egg_tag_t *> *tag;
	Vector<egg_boneframe_t *> *boneframe;


	switch (type)
	{
	case FREYJA_VERTEX:
		vertex = mEgg->getVertexList();
    
		if (!vertex)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			vertex->start();
			break;
		case FREYJA_LIST_NEXT:
			vertex->next();
			break;
		default:
			vertex->mIndex = item;
		}

		if (vertex->forward())
		{
			return vertex->currentIndex();
		}
		break;
	case FREYJA_TEXEL:
		texcoord = mEgg->getTexCoordList();
    
		if (!texcoord)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			texcoord->start();
			break;
		case FREYJA_LIST_NEXT:
			texcoord->next();
			break;
		default:
			texcoord->mIndex = item;
		}

		if (texcoord->forward())
		{
			return texcoord->currentIndex();
		}
		break;
	case FREYJA_MESH:
		mesh = mEgg->getMeshList();
    
		if (!mesh)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mesh->start();
			break;
		case FREYJA_LIST_NEXT:
			mesh->next();
			break;
		default:
			mesh->mIndex = item;
		}

		if (mesh->forward())
			return mesh->currentIndex();
		break;
	case FREYJA_GROUP:
		group = mEgg->getGroupList();
    
		if (!group)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			group->start();
			break;
		case FREYJA_LIST_NEXT:
			group->next();
			break;
		default:
			group->mIndex = item;
		}

		if (group->forward())
			return group->currentIndex();
		break;
	case FREYJA_POLYGON:
		polygon = mEgg->getPolygonList();
    
		if (!polygon)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			polygon->start();
			break;
		case FREYJA_LIST_NEXT:
			polygon->next();
			break;
		default:
			polygon->mIndex = item;
		}

		if (polygon->forward())
			return polygon->currentIndex();
		break;
	case FREYJA_BONE_TAG:
		tag = mEgg->getTagList();
    
		if (!tag)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			tag->start();
			break;
		case FREYJA_LIST_NEXT:
			tag->next();
			break;
		default:
			tag->mIndex = item;
		}

		if (tag->forward())
			return tag->currentIndex();
		break;
	case FREYJA_BONE_FRAME:
		boneframe = mEgg->getBoneFrameList();
    
		if (!boneframe)
			return PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			boneframe->start();
			break;
		case FREYJA_LIST_NEXT:
			boneframe->next();
			break;
		default:
			boneframe->mIndex = item;
		}

		if (boneframe->forward())
			return boneframe->currentIndex();
		break;
	case FREYJA_MESHTREE_ANIM:
		printf("EggPlugin::eggIterator> %s Not implemented, %s:%i\n", 
			   "FREYJAMANIMATION_ANIM", __FILE__, __LINE__);
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetVertex(float *xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->getVertexList();

	if (!vertex || !vertex->forward())
		return PLUGIN_ERROR;
 
	vert = vertex->current();

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
	egg_texcoord_t *tex;



	polygon_lst = mEgg->getPolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = polygon_lst->current();

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
	case FREYJA_TEXEL:
		tex = mEgg->getTexCoord(polygon->vertex[item]);
    
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


unsigned int EggPlugin::eggGetPolygon(egg_plugin_t type, int item, 
									  float *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texcoord_t *tex;



	polygon_lst = mEgg->getPolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = polygon_lst->current();

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
	case FREYJA_TEXEL:
		tex = mEgg->getTexCoord(polygon->vertex[item]);
    
		if (!tex)
			return PLUGIN_ERROR;

		value[0] = tex->uv[0];
		value[1] = tex->uv[1];
    
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
	case FREYJA_TEXEL:
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
	case FREYJA_BONE_TAG:
		if (mTag)
			return mTag->id;
		break;
	case FREYJA_BONE_FRAME:
		if (mBoneFrame)
			return mBoneFrame->id;
		break;
	case FREYJA_MESHTREE_ANIM:
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
		*x = mTag->rotation.mVec[0];
		*y = mTag->rotation.mVec[1];
		*z = mTag->rotation.mVec[2];
   
		return 0;
	}

	return PLUGIN_ERROR;
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


	if (!mEgg->checkFile(filename))
	{
		if (!mEgg->loadFile(filename))
			return 0;
		else
			return -2;
	}

	printf("[EggPlugin module loader invoked]\n");

	for (mModules.start(); mModules.forward(); mModules.next())
	{
		name = mModules.current();

		sprintf(module_filename, "%slibfreyja_loader-%s.so", mPluginDir, name);
		sprintf(module_import, "freyja_model__%s_import", name);
		sprintf(module_check, "freyja_model__%s_check", name);

		if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
		{
			fprintf(stderr, "\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				fprintf (stderr, "\tERROR: %s\n", error);
				//return -5;
			}

			//return -1;
			continue;
		}
		else
		{
			printf("\tModule '%s' opened.\n", module_filename);
    
			check = (int (*)(char *filename))dlsym(handle, module_check);

			if ((error = dlerror()) != NULL)  
			{
				fprintf (stderr, "\tERROR: %s\n", error);
				//return -5;
				dlclose(handle);
				continue;
			}

			import = (int (*)(char * filename))dlsym(handle, module_import);

			if ((error = dlerror()) != NULL)  
			{
				fprintf (stderr, "\tERROR: %s\n", error);
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

	printf("[EggPlugin module loader sleeps now]\n");

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
		mStack.Push(FREYJA_MESH);
		mMesh = mEgg->newMesh();
		mEgg->addMesh(mMesh);
		break;
	case FREYJA_GROUP:
		mStack.Push(FREYJA_GROUP);

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
		mStack.Push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexelList.clear();

		if (!mMesh)
		{
			fprintf(stderr, "EggPlugin::eggEnd> Polygon defined outside MESH!");
		}
		break;
	case FREYJA_BONE_TAG:
		mStack.Push(FREYJA_BONE_TAG);
		mTag = mEgg->addTag(0.0, 0.0, 0.0, 0x00);
		//mEgg->addTag(mTag);
		mTag->name[0] = 0;
		break;
	case FREYJA_BONE_FRAME:
		mStack.Push(FREYJA_BONE_FRAME);
		break;
	case FREYJA_MESHTREE_ANIM:
		mStack.Push(FREYJA_MESHTREE_ANIM);
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


	switch (mStack.Pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexelList, mTextureId);

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

	mTextureFiles.pushBack(strdup(filename));
	return mTextureFiles.size() - 1;
}

int EggPlugin::eggGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;


	if (mTextureFiles.empty() || index > mTextureFiles.size())
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


	if (mTextures.empty())
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


unsigned int EggPlugin::eggTexelStore2f(float u, float v)
{
	return mEgg->addTexCoord(u, v);
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


	if (!vert)
		return;

	for (vert->weights.start(); vert->weights.forward(); vert->weights.next())
	{
		vWeight = vert->weights.current();

		if (vWeight)
		{
			if (weight <= 0.0) // Remove weight
			{
				delete vWeight;
				vert->weights.assign(vert->weights.currentIndex(), 0x0);
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
			emptySlot = vert->weights.currentIndex();
		}
	}

	if (weight <= 0.0) // Don't add dead weight ( remove requested )
		return;

	if (total + weight > 1.0)  // Just give a warning for now
		printf("WARNING: Weight overflow not handled here %s:%d\n", 
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
	if (mStack.Peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(egg_id);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void EggPlugin::eggTexel1i(unsigned int egg_id)
{
	if (mStack.Peek() == FREYJA_POLYGON)
	{
		mTexelList.pushBack(egg_id);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTexel1i> Texel defined outside POLYGON!\n");
	}
}


void EggPlugin::eggTexture1i(int id)
{
	if (mStack.Peek() == FREYJA_POLYGON)
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
	if (mStack.Peek() == FREYJA_GROUP)
	{
		if (!mGroup)
		{
			fprintf(stderr, 
					"EggPlugin::eggGroupCenter> GROUP isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mGroup->center = Vector3d(x, y, z);
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
	if (mStack.Peek() == FREYJA_BONE_TAG)
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
	if (mStack.Peek() == FREYJA_BONE_TAG)
	{
		if (!mTag)
		{
			fprintf(stderr, 
					"EggPlugin::eggTagPos> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			mTag->position = Vector3d(x, y, z);
		}
	}
	else if (mTag) // HACK
	{
		mTag->position = Vector3d(x, y, z);
	}
	else
	{
		fprintf(stderr, 
				"EggPlugin::eggTagPos> Pos defined outside BONETAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagFlags(unsigned int flags)
{
	if (mStack.Peek() == FREYJA_BONE_TAG)
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
	if (mStack.Peek() == FREYJA_BONE_TAG)
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
	if (mStack.Peek() == FREYJA_BONE_TAG)
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
		mBoneFrame = mEgg->getBoneFrame(mEgg->addBoneFrame(x, y, z));    
		mAnimation->frame.pushBack(mBoneFrame->id);
   
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggTagRotate(float x, float y, float z)
{
	if (mTag)
	{
		mTag->rotation = Vector3d(x, y, z);
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
	unsigned int sz;


	if (!image || !depth || !width || !height)
		return  PLUGIN_ERROR;

	sz = width*height*depth;

	t = new EggTextureData();
 
	t->mWidth = width;
	t->mHeight = height;
	t->mType = type;
	t->mBitDepth = depth * 8;
	t->mImage = new unsigned char[sz];
  
	memcpy(t->mImage, image, sz);

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
