/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the User1 plug-in module
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
 * Mongoose - Created, used to open a lame model format from
 *            a shader demo  =)
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPluginABI.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

using namespace mstl;

extern "C" {

	int freyja_model__user1_check(char *filename);
	int freyja_model__user1_import(char *filename);
	int freyja_model__user1_export(char *filename);
}



typedef struct user1_vec_s
{
	float x, y, z;

} user1_vec_t;


typedef struct user1_vert_s
{
	user1_vec_t norm;	
	user1_vec_t pos;

} user1_vert_t;


typedef struct user1_poly_s
{
	user1_vert_t vertex[3];

} user1_poly_t;


//////////////////////////////////////////////////////////////////////


int freyja_model__user1_check(char *filename)
{
  unsigned int len_f, len_e;
  char *ext = ".user1";

 
  len_f = strlen(filename) - 1;
  len_e = strlen(ext) - 1;

  while (len_f+1 && len_e+1)
  {
    if (filename[len_f] == ext[len_e])
    {
      if (ext[len_e] == '.')
      {
        return 0;
      }
    }
    else if (filename[len_f] < ext[len_e])
      return -1;
    else if (filename[len_f] > ext[len_e])
      return 1;

    len_f--;
    len_e--;
  }

  return 1;
}


int freyja_model__user1_import(char *filename)
{
	FILE *f;
	int polygon_count, i, j;
	unsigned int v, t, index;
	user1_poly_t *polygon;
	Map<unsigned int, unsigned int> trans;


	f = fopen (filename, "rb");

	if (!f)
		return -1;

	fread(&polygon_count, sizeof(int), 1, f);
	polygon = new user1_poly_t[polygon_count];
	fread(polygon, sizeof(user1_poly_t) * polygon_count, 1, f);
	fclose (f);


	// Start a new mesh
	freyjaBegin(FREYJA_MESH);
		
	// Start a new vertex group
	//freyjaBegin(FREYJA_GROUP);

	index = 0;

	for (i = 0; i < polygon_count; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			v = freyjaVertexCreate3f(polygon[i].vertex[j].pos.x,
											 polygon[i].vertex[j].pos.y,
											 polygon[i].vertex[j].pos.z);
			
			trans.Add(index++, v);
			freyjaVertexNormal3f(v, 
										polygon[i].vertex[j].norm.x,
										polygon[i].vertex[j].norm.y,
										polygon[i].vertex[j].norm.z);
		}
	}

	//freyjaEnd(); // FREYJA_GROUP

	index = 0;

	for (i = 0; i < polygon_count; ++i)
	{
		// Start a new polygon
		freyjaBegin(FREYJA_POLYGON);

		// Format doesn't have texels
		t = freyjaTexCoordCreate2f(0.5, 0.5);
		freyjaPolygonAddTexCoord1i(i, t);
		t = freyjaTexCoordCreate2f(0.5, 0.5);
		freyjaPolygonAddTexCoord1i(i, t);
		t = freyjaTexCoordCreate2f(0.5, 0.5);
		freyjaPolygonAddTexCoord1i(i, t);
		
		freyjaPolygonAddVertex1i(i, trans[index++]);
		freyjaPolygonAddVertex1i(i, trans[index++]);
		freyjaPolygonAddVertex1i(i, trans[index++]);

		freyjaPolygonMaterial1i(0);

		freyjaEnd(); // FREYJA_POLYGON
	}
	

	freyjaEnd(); // FREYJA_MESH

	return 0;
}


int freyja_model__user1_export(char *filename)
{
	printf("freyja_model__user1_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}
