/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include <math.h>
#include <hel/math.h>
#include <mstl/Vector.h>
#include "FreyjaPluginABI.h"


using namespace mstl;

/* Mesh generation utils */
void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	int32 index;


	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 1.0));

	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON


	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	int32 index;


	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index =freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, -0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, -0.33, 0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoordCreate2f(0.0, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.0, 0.25));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.5));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.75));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 1.0));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.25));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.50));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.50));


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

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateCircleMesh(vec3_t origin, int32 count)
{
	Vector<long> vertices, texcoords;
	int32 i, index, center, centerUV;
	vec_t x, z, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.5, 0.5);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		freyjaTexCoordCreate2f(x, z);
		texcoords.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateConeMesh(vec3_t origin, vec_t height, int32 count)
{
	Vector<long> vertices, texcoords, texcoords2;
	int32 i, index, point, center, pointUV, centerUV;
	vec_t x, z, u, v, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoordCreate2f(0.75, 0.25);
	point = index;

	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateFunctionalSphereMesh(vec3_t origin, vec_t radius, 
										int32 count, int32 segments, 
										float(*gen)(int32 i, int32 j, int32 seg))
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	height = radius * 2.0;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			r = (*gen)(i, j, segments);
			r *= radius;

			index = freyjaVertexCreate3f(origin[0]+x*r, origin[1]+y*r, origin[2]+z*r);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 1; i < segments-1; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  int32 count, int32 segments)
{
	Vector<long> segVert, segTex, topTex, bottomTex;
	int32 i, j, index, index2, top, bottom, bt, tt;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;
	height = radius * 2.0;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
		y = height * ((float)i/(float)segments);
		v = 0.5 * ((float)i/(float)segments) + 0.5;

		r = sin(helDegToRad(180.0 * ((float)i/(float)segments)));
		(i > segments/2) ? y = height*(1.0-r)+y*r : y *= r;

		if (i == 0)
		{
			bottom = freyjaVertexCreate3f(origin[0], origin[1]+y, origin[2]);
			freyjaVertexNormal3f(bottom, 0.0, -1.0, 0.0);

			/* Padding, if iterator needs alignment */
			for (j = 0; j < count; ++j)
			{
				segVert.pushBack(-1);
				segTex.pushBack(-1);
			}
			continue;
		}
		else if (i == segments-1)
		{
			top = freyjaVertexCreate3f(origin[0], origin[1]+height, origin[2]);
			freyjaVertexNormal3f(top, 0.0, 1.0, 0.0);
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));

			u = 1.0 * ((float)j/(float)count);

			nx = x * r;
			ny = (1.0 - r) * (i < segments/2) ? -0.3 : 0.3; // soften the halves
			nz = z * r;

			x *= r * radius;
			z *= r * radius;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y, origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		
		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		index = freyjaTexCoordCreate2f(u, v);
		topTex.pushBack(index);
		index = freyjaTexCoordCreate2f(u+0.5, v);
		bottomTex.pushBack(index);
	}

	bt = freyjaTexCoordCreate2f(0.75, 0.25);
	tt = freyjaTexCoordCreate2f(0.25, 0.25);

	/* Tube */
	for (i = 0; i < segments-1; ++i)
	{
		if (i == 0)
		{
			index = count * 1;

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(bottomTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(bottomTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}

			continue;
		}
		else if (i == segments-2)
		{
			index = count * (i + 1);

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(topTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(topTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}
		}

		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								int32 count, int32 segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2, point, center, pointUV, centerUV;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoordCreate2f(0.75, 0.25);
	point = index;

	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertexCreate3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		/* Reuse top vertices for segment-th ring */
		if (i == segments)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices2[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				index =freyjaTexCoordCreate2f(u, v);
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices2[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices2[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices2[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices2[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}


void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							int32 count, int32 segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSectionLock();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertexCreate3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		/* Reuse top vertices for segment-th ring */
		if (i == segments)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices2[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSectionUnlock();
}
