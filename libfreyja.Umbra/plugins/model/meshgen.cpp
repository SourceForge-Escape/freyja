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
#include <freyja/FreyjaPlugin.h>

extern "C" {

	/* Linkage setup and interface requirements */
	void import_functions();
	void meshgen_import_functions();

	/* Support functions */
	void meshgen_cube(vec_t sz);

	void meshgen_cylinder(unsigned int major, unsigned int minor, 
						  vec_t height, vec_t radius);

	void meshgen_sphere(unsigned int major, unsigned int minor, 
						vec_t radius);

	void meshgen_strip(unsigned int count, vec_t sz);
}


void import_functions()
{
	meshgen_import_functions();
}


void meshgen_import_functions()
{
	freyjaPrintMessage("Importing new functions from meshgen.so");

	freyjaFunctionBegin("meshgen_cube");
	//freyjaArg("edge lenght", ARG_FLOAT, 8.0f);
	freyjaFunctionArgFloat("edge_lenght", 8.0f);
	freyjaFunctionEnd();

	freyjaFunctionBegin("meshgen_sphere");
	freyjaFunctionArgInt("major", 16);
	freyjaFunctionArgInt("minor", 16);
	freyjaFunctionArgFloat("radius", 8.0f);
	freyjaFunctionEnd();

	freyjaFunctionBegin("meshgen_strip");
	freyjaFunctionArgInt("count", 10);
	freyjaFunctionArgFloat("lenght", 8.0f);
	freyjaFunctionEnd();

	freyjaFunctionBegin("meshgen_cylinder");
	freyjaFunctionArgInt("major", 4);
	freyjaFunctionArgInt("minor", 16);
	freyjaFunctionArgFloat("height", 8.0f);
	freyjaFunctionArgFloat("radius", 4.0f);
	freyjaFunctionEnd();
}


// FIXME: Normals need fixing 
void meshgen_cube(vec_t sz)
{
	unsigned int v, t;
	Map <unsigned int, unsigned int> texel;
	Map <unsigned int, unsigned int> vertex;


	// Start a new mesh
	freyjaBegin(FREYJA_MESH);

	// Start a new vertex group
	//freyjaBegin(FREYJA_GROUP);

	// Allocate vertices, FIXME normals are incorrect
	v = freyjaVertex3f(0.0, sz, 0.0);
	freyjaNormal3f(0.0, 1.0, 0.0);
	vertex.Add(0, v);

	v = freyjaVertex3f(sz, sz, 0.0);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(1, v);
	
	v = freyjaVertex3f(sz, 0.0, 0.0);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(2, v);

	v = freyjaVertex3f(0.0, 0.0, 0.0);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(3, v);

	v = freyjaVertex3f(0.0, sz, sz);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(4, v);

	v = freyjaVertex3f(sz, sz, sz);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(5, v);

	v = freyjaVertex3f(sz, 0.0, sz);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(6, v);

	v = freyjaVertex3f(0.0, 0.0, sz);
	freyjaNormal3f( 0.0, 1.0, 0.0);
	vertex.Add(7, v);

	//freyjaGroupCenter3f(sz / 2.0, sz / 2.0, sz / 2.0);

	//freyjaEnd(); //FREYJA_GROUP 


	// Mongoose 2002.03.02, Allocate texels
	t = freyjaTexCoord2f(0.0, 1.0);
	texel.Add(0, t);

	t = freyjaTexCoord2f(0.33, 1.0);
	texel.Add(1, t);

	t = freyjaTexCoord2f(0.66, 1.0);
	texel.Add(2, t);

	t = freyjaTexCoord2f(0.0, 0.66);
	texel.Add(3, t);

	t = freyjaTexCoord2f(0.33, 0.66);
	texel.Add(4, t);

	t = freyjaTexCoord2f(0.66, 0.66);
	texel.Add(5, t);

	t = freyjaTexCoord2f(0.0, 0.33);
	texel.Add(6, t);

	t = freyjaTexCoord2f(0.33, 0.33);
	texel.Add(7, t);

	t = freyjaTexCoord2f(0.66, 0.33);
	texel.Add(8, t);

	t = freyjaTexCoord2f(0.33, 0.0);
	texel.Add(9, t);

	t = freyjaTexCoord2f(0.66, 0.0);
	texel.Add(10, t);

	t = freyjaTexCoord2f(1.0, 0.66);
	texel.Add(11, t);

	t = freyjaTexCoord2f(1.0, 0.33);
	texel.Add(12, t);

	// Allocate quad faces using triangles...

	// 1: 0 1 3
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[0]);
	freyjaPolygonTexCoord1i(texel[0]);
	freyjaPolygonVertex1i(vertex[1]);
	freyjaPolygonTexCoord1i(texel[1]);
	freyjaPolygonVertex1i(vertex[3]);
	freyjaPolygonTexCoord1i(texel[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 1: 2 1 3
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[2]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonVertex1i(vertex[1]);
	freyjaPolygonTexCoord1i(texel[1]);
	freyjaPolygonVertex1i(vertex[3]);
	freyjaPolygonTexCoord1i(texel[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 2 : 4 5 7
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[4]);
	freyjaPolygonTexCoord1i(texel[1]);
	freyjaPolygonVertex1i(vertex[5]);
	freyjaPolygonTexCoord1i(texel[2]);
	freyjaPolygonVertex1i(vertex[7]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 2 : 6 5 7
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[6]);
	freyjaPolygonTexCoord1i(texel[5]);
	freyjaPolygonVertex1i(vertex[5]);
	freyjaPolygonTexCoord1i(texel[2]);
	freyjaPolygonVertex1i(vertex[7]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 3 : 0 4 3
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[0]);
	freyjaPolygonTexCoord1i(texel[3]);
	freyjaPolygonVertex1i(vertex[4]);
	freyjaPolygonTexCoord1i(texel[6]);
	freyjaPolygonVertex1i(vertex[3]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 3 : 7 4 3
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[7]);
	freyjaPolygonVertex1i(vertex[4]);
	freyjaPolygonVertex1i(vertex[3]);
	freyjaPolygonTexCoord1i(texel[7]);
	freyjaPolygonTexCoord1i(texel[6]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 4 : 0 1 4
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[0]);
	freyjaPolygonVertex1i(vertex[1]);
	freyjaPolygonVertex1i(vertex[4]);
	freyjaPolygonTexCoord1i(texel[4]);
	freyjaPolygonTexCoord1i(texel[7]);
	freyjaPolygonTexCoord1i(texel[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 4 : 5 1 4
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[5]);
	freyjaPolygonVertex1i(vertex[1]);
	freyjaPolygonVertex1i(vertex[4]);
	freyjaPolygonTexCoord1i(texel[8]);
	freyjaPolygonTexCoord1i(texel[7]);
	freyjaPolygonTexCoord1i(texel[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 5 : 3 7 2
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[3]);
	freyjaPolygonVertex1i(vertex[7]);
	freyjaPolygonVertex1i(vertex[2]);
	freyjaPolygonTexCoord1i(texel[5]);
	freyjaPolygonTexCoord1i(texel[8]);
	freyjaPolygonTexCoord1i(texel[11]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 5 : 6 7 2
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[6]);
	freyjaPolygonVertex1i(vertex[7]);
	freyjaPolygonVertex1i(vertex[2]);
	freyjaPolygonTexCoord1i(texel[12]);
	freyjaPolygonTexCoord1i(texel[8]);
	freyjaPolygonTexCoord1i(texel[11]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON
	
	// 6 : 1 2 5 
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[1]);
	freyjaPolygonVertex1i(vertex[2]);
	freyjaPolygonVertex1i(vertex[5]);
	freyjaPolygonTexCoord1i(texel[7]);
	freyjaPolygonTexCoord1i(texel[9]);
	freyjaPolygonTexCoord1i(texel[8]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	// 6 : 6 2 5
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertex[6]);
	freyjaPolygonVertex1i(vertex[2]);
	freyjaPolygonVertex1i(vertex[5]);
	freyjaPolygonTexCoord1i(texel[10]);
	freyjaPolygonTexCoord1i(texel[9]);
	freyjaPolygonTexCoord1i(texel[8]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaEnd(); // FREYJA_MESH
}



void meshgen_cylinder(unsigned int major, unsigned int minor, 
					  vec_t height, vec_t radius)
{
	Vector<unsigned int> vertices;
	unsigned int i, j, k, v;
	double majorStep = height / major;
	double minorStep = 2.0 * HEL_PI / minor;


	freyjaBegin(FREYJA_MESH);

	for (i = 0; i < major; ++i) 
	{
		float z0 = 0.5 * height - i * majorStep;
		float z1 = z0 - majorStep;


		//freyjaBegin(FREYJA_GROUP);

		// start a new tri strip 
		for (j = 0; j <= minor; ++j) 
		{      
			double a = j * minorStep;
			float x = radius * cos(a);
			float y = radius * sin(a);

			v = freyjaVertex3f(x, y, z0);
			freyjaTexCoord2f(j / (vec_t)minor, i / (vec_t)major);
			freyjaNormal3f(x / radius, y / radius, 0.0f);
			vertices.pushBack(v);

			v = freyjaVertex3f(x, y, z1);
			freyjaTexCoord2f(j / (vec_t)minor, (i + 1) / (vec_t)major);
			freyjaNormal3f(x / radius, y / radius, 0.0f);
			vertices.pushBack(v);
		}

		//freyjaEnd(); // FREYJA_GROUP

		/* Generate mesh faces from tristrip data */
		for (k = (j*2), j = 2; j < k; ++j)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonVertex1i(vertices[j-2]);
			freyjaPolygonVertex1i(vertices[j-1]);
			freyjaPolygonVertex1i(vertices[j]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}

		vertices.clear();
	}

	freyjaEnd(); // FREYJA_MESH
}



void meshgen_strip(unsigned int count, vec_t sz)
{
	Vector<unsigned int> vertices;
	vec3_t xyz;
	vec2_t uv;
	vec_t x, y;
	unsigned int i, v;


	freyjaBegin(FREYJA_MESH);
	//freyjaBegin(FREYJA_GROUP);

	// Allocate vertices and texels
	for (i = 0, x = y = 0.0f; i < count; ++i)
	{		
		v = freyjaVertex3f(x*sz, y*sz, 0.0f);
		freyjaNormal3f( 0.0f, 0.0f, 1.0f);
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

		//freyjaGenerateUVFromXYZ(xyz, uv+0, uv+1);
		freyjaTexCoord2f(uv[0], uv[1]);
	}

	//freyjaEnd(); // FREYJA_GROUP

	// Generate mesh faces
	for (i = 2; i < count; ++i)
	{
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonVertex1i(vertices[i-2]);
		freyjaPolygonVertex1i(vertices[i-1]);
		freyjaPolygonVertex1i(vertices[i]);
		freyjaPolygonMaterial1i(0);
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH
}


void meshgen_sphere(unsigned int major, unsigned int minor, vec_t radius)
{
	Vector<unsigned int> vertices;
	unsigned int i, j, k, v;
	vec_t x, y;
	double majorStep = (HEL_PI / major);
	double minorStep = (2.0 * HEL_PI / minor);
	vec_t nx, ny, nz, ns;


	freyjaBegin(FREYJA_MESH);

	for (i = 0; i < major; ++i) 
	{
		double a = i * majorStep;
		double b = a + majorStep;
		double r0 = radius * sin(a);
		double r1 = radius * sin(b);
		vec_t z0 = radius * cos(a);
		vec_t z1 = radius * cos(b);


		//freyjaBegin(FREYJA_GROUP);

		/* Start a new tri strip */
		for (j = 0; j <= minor; ++j) 
		{
			double c = j * minorStep;

			x = cos(c);
			y = sin(c);

			v = freyjaVertex3f(x * r0, y * r0, z0);
			freyjaTexCoord2f(j / (vec_t)minor, i / (vec_t)major);
			nx = (x * r0) / radius;
			ny = (y * r0) / radius;
			nz = z0 / radius;
			ns = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;
			// printf("1: %f %f %f\n", nx, ny, nz);
			freyjaNormal3f(nx, ny, nz);
			vertices.pushBack(v);


			v = freyjaVertex3f(x * r1, y * r1, z1);
			freyjaTexCoord2f(j / (vec_t)minor, (i + 1) / (vec_t)major);
			nx = (x * r1) / radius;
			ny = (y * r1) / radius;
			nz = z0 / radius;
			ns = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;
			// printf("2: %f %f %f\n", nx, ny, nz);
			freyjaNormal3f(nx, ny, nz);
			vertices.pushBack(v);
		}

		//freyjaGroupCenter3f(0.0f, 0.0f, 0.0f);
		//freyjaEnd(); // FREYJA_GROUP 


		// Generate mesh faces from tristrip data
		for (k = (j*2), j = 2; j < k; ++j)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonVertex1i(vertices[j-2]);
			freyjaPolygonVertex1i(vertices[j-1]);
			freyjaPolygonVertex1i(vertices[j]);
			freyjaPolygonTexCoord1i(vertices[j-2]);
			freyjaPolygonTexCoord1i(vertices[j-1]);
			freyjaPolygonTexCoord1i(vertices[j]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}

		vertices.clear();
	}

	freyjaEnd(); // FREYJA_MESH
}

