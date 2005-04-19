/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This has the mesh generation utils.
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2004.10.09:
 * Mongoose: Created, from old Egg8 plugin
 ==========================================================================*/

#include <mstl/Vector.h>
#include <freyja8/EggPlugin9.h>

extern "C" {

	/* Linkage setup and interface requirements */
	void meshgen_import_functions();

	/* Support functions */
	void meshgen_cube(vec_t sz);

	void meshgen_cylinder(unsigned int major, unsigned int minor, 
						  vec_t height, vec_t radius);

	void meshgen_sphere(unsigned int major, unsigned int minor, 
						vec_t radius);

	void meshgen_strip(unsigned int count, vec_t sz);
}


void meshgen_import_functions()
{
	eggPrintMessage("Importing new functions from meshgen.so");

	eggImportFunctionBegin("meshgen_cube");
	//eggImportArg("edge lenght", ARG_FLOAT, 8.0f);
	eggImportArgFloat("edge_lenght", 8.0f);
	eggImportFunctionEnd();

	eggImportFunctionBegin("meshgen_sphere");
	eggImportArgUInt("major", 16);
	eggImportArgUInt("minor", 16);
	eggImportArgFloat("radius", 8.0f);
	eggImportFunctionEnd();

	eggImportFunctionBegin("meshgen_strip");
	eggImportArgUInt("count", 10);
	eggImportArgFloat("lenght", 8.0f);
	eggImportFunctionEnd();

	eggImportFunctionBegin("meshgen_cylinder");
	eggImportArgUInt("major", 4);
	eggImportArgUInt("minor", 16);
	eggImportArgFloat("height", 8.0f);
	eggImportArgFloat("radius", 4.0f);
	eggImportFunctionEnd();
}


// FIXME: Normals need fixing 
void meshgen_cube(vec_t sz)
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

	eggGroupCenter3f(sz / 2.0, sz / 2.0, sz / 2.0);

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



void meshgen_cylinder(unsigned int major, unsigned int minor, 
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



void meshgen_strip(unsigned int count, vec_t sz)
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

		if (i % 2)
			uv[0] = ((x-1)*2) / count;
		else
			uv[0] = (x*2) / count;
		
		uv[1] = y;

		//eggGenerateUVFromXYZ(xyz, uv+0, uv+1);
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


void meshgen_sphere(unsigned int major, unsigned int minor, vec_t radius)
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

