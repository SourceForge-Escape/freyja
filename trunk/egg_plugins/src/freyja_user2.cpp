/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Obj plug-in module
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
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include "EggPlugin.h"


extern "C" {

  int freyja_model__user2_check(char *filename);
  int freyja_model__user2_import(char *filename);
  int freyja_model__user2_export(char *filename);
}



int freyja_model__user2_check(char *filename)
{
   unsigned int len_f, len_e;
  char *ext = ".obj";

 
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

  return -1;
}


int freyja_model__user2_import(char *filename)
{
	FILE *f;
	char t;
	float x, y, z;
	int a, b, c;
	unsigned int i;

	f = fopen (filename, "rb");

	if (!f)
		return -1;

	// Start a new mesh
	eggBegin(FREYJA_MESH);

	while (fscanf(f, "%c", &t) != EOF && t == 'v')
	{
		// Start a new vertex group
		eggBegin(FREYJA_GROUP);

		fscanf(f, "%f %f %f", &x, &y, &z);
		eggVertexStore3f(x, y, z);

		eggEnd(); // FREYJA_GROUP
	}

	while (fscanf(f, "%c", &t) != EOF && t == 'f')
	{
		// Start a new polygon
		eggBegin(FREYJA_POLYGON);

		// Format doesn't have texels
		i = eggTexelStore2f(0.5, 0.5);
		eggTexel1i(i);
		i = eggTexelStore2f(0.5, 0.5);
		eggTexel1i(i);
		i = eggTexelStore2f(0.5, 0.5);
		eggTexel1i(i);
		
		fscanf(f, "%i %i %i", &a, &b, &c);

		eggVertex1i(a);
		eggVertex1i(b);
		eggVertex1i(c);

		eggTexture1i(0);

		eggEnd(); // FREYJA_POLYGON
	}

	eggEnd(); // FREYJA_MESH

	fclose(f);

	return 1;
}


int freyja_model__user2_export(char *filename)
{
  printf("freyja_model__obj_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
